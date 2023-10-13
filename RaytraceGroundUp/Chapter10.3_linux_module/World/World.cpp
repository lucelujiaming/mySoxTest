#include "stdafx.h"
// this file contains the definition of the World class

// #include "wxraytracer.h"

#include "Constants.h"

// utilities

#include "Vector3D.h"
#include "Point2D.h"
#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"

#include "World.h"

// build functions
#include "MultipleObjects.h"
#include "RGBColor.h"
#include "Sphere.h"

#include <fstream>
#include <iostream>
using namespace std;
extern ofstream out;

#include "NRooks.h"
#include "Pinhole.h"
#include "MultiJittered.h"
// #include "RayCast.h"
#include "Ambient.h"
#include "ThinLens.h"

// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
    :      background_color(black),
        tracer_ptr(NULL),
        camera_ptr(NULL)
{}

World::~World(void) {

    if(tracer_ptr) {
        delete tracer_ptr;
        tracer_ptr = NULL;
    }

    if (camera_ptr) {
        delete camera_ptr;
        camera_ptr = NULL;
    }    
}

//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis
// 函数render_scene()负责渲染场景。
// 
void World::render_scene() const
{
    RGBColor pixel_color;
    Ray ray;
    // 在该例子中，视平面被设置在zw=100处。当然，这只是一个临时性的设置方案。
    double zw = 200.0;
    // double x, y;
	// 因为需要映射到三维的半球体，因此上，这里修改为Point3D
    Point3D sp;                                 // sample point in [0, 1] x [0, 1]
    Point2D pp;                                 // sample point on a pixel

    ray.d = Vector3D(0, 0, -1);
    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vp.vres; r++) {
        for (int c = 0; c < vp.hres; c++) {
            pixel_color = background_color;
            
            for(int j=0; j < vp.num_samples; j++) {
			    // 返回存储于采样器对象中的下一个采样点，映射到半球体。
			    // 其中sp.z不使用。
                sp = vp.sampler_ptr->sample_hemisphere();
                pp.x = vp.s*(c-0.5*vp.hres + sp.x);
                pp.y = vp.s*(r-0.5*vp.vres + sp.y);
                ray.o = Point3D(pp.x, pp.y, zw);
	            // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
	            // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
	            // 同时，还将返回各像素的颜色值。
	            // 但此处采取了一种间接的调用方案， 
	            // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                pixel_color += tracer_ptr->trace_ray(ray);
            }

            pixel_color /= vp.num_samples; // average the colors
            // 循环体中最后一行代码将调用display_pixel() 函数以在窗口中显示像素。
            // 这将会把RGBColor转换为当前计算机支持的显示格式。
            display_pixel(r, c, pixel_color);
        }
    }
}

// 投影视图。
void World::render_perspective(void) const {
    RGBColor pixel_color;
    Ray ray;

    int eye = 500;   // 视见点
    int d = 400;     // 视平面距离
    int s = 1;       // 像素尺寸
    
    // open_window(hres, vres);
    ray.o = Point3D(0.0, 0.0, eye);
    
    for(int r=0; r < vp.vres; r++) { // up
        for(int c=0; c<=vp.hres; c++) { //across
            // 计算公式参见P114
            ray.d = Vector3D(s*(c-0.5*(vp.hres-1.0) ),
                        s*(r-0.5*(vp.vres-1.0)), -d);
            ray.d.normalize();
            pixel_color=tracer_ptr->trace_ray(ray) ;
            display_pixel(r, c, pixel_color);
        }
    }
}

// ------------------------------------------------------------------ clamp
// explained on page 274
RGBColor
World::max_to_one(const RGBColor& c) const {

    float max_value = max(c.r, max(c.g, c.b));

    if (max_value > 1.0)
        return (c / max_value);
    else
        return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one
// explained on page 275
RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {

    RGBColor c(raw_color);

    if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
        c.r = 1.0; c.g = 0.0; c.b = 0.0;
    }

    return (c);
}


// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function
// explained on page 72
// display_pixel() 函数将各个像素的颜色值转换为显示器可支持的颜色。
// 这一过程涉及到3个步骤：色调映射、gamma值修正以及整数映射。
//   
void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {

    RGBColor mapped_color;

    if (vp.show_out_of_gamut)
        mapped_color = clamp_to_color(raw_color);
    else
        mapped_color = max_to_one(raw_color);
    // 显示器的亮度值通常与工作电压呈非线性关系， 因而gamma值修正往往是必要的。
    if (vp.gamma != 1.0)
        mapped_color = mapped_color.powc(vp.inv_gamma);

    //have to start from max y coordinate to convert to screen coordinates
    int x = column;
    int y = vp.vres - row - 1;

    // paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
    //                          (int)(mapped_color.g * 255),
    //                          (int)(mapped_color.b * 255));
    // cout << (int)(mapped_color.r * 255) << " " << (int)(mapped_color.g * 255) << " " << (int)(mapped_color.b * 255) << endl;
    out << (int)(mapped_color.r * 255) << " " << (int)(mapped_color.g * 255) << " " << (int)(mapped_color.b * 255) << endl;

}

// 这个函数将在光线与场景中的全部对象之间进行相交测试，并返回一个Shade Rec对象。
// 函数显示了将最近对象间颜色值存储于Shade Rec对象中的计算过程。
ShadeRec World::hit_bare_bones_objects(const Ray & ray) const
{
   ShadeRec sr (*this);
   double t;
   double tmin = kHugeValue;
   int num_objects = objects.size();
   for(int i = 0; i < num_objects; i++)
   {   
      // 代码并未定义特定的几何对象类型，因而适用于几何对象继承层次结构中的任意对象类型，
      // 并实现了基于hit() 函数的公共接口。
      if(objects[i]->hit(ray,t,sr) && t < tmin)
      {   
         sr.hit_an_object = true;
         tmin = t;
         // 考虑到对象数量的增加，应适当地采用不同的颜色值描述相关对象，
         sr.color = objects[i]->get_color();
      }   
   }   
   return sr; 
}

// 函数hit_objects()将在光线与场景中的全部对象之间进行相交测试，并返回一个ShadeRec对象。
ShadeRec
World::hit_objects(const Ray& ray) {

    ShadeRec    sr(*this);
    double      t;
    Normal normal;
    Point3D local_hit_point;
    double      tmin            = kHugeValue;
    int         num_objects     = objects.size();

    for (int j = 0; j < num_objects; j++)
        // 代码并未定义特定的几何对象类型，因而适用于几何对象继承层次结构中的任意对象类型，
        // 并实现了基于hit()函数的公共接口。
        // 考虑到对象数量的增加，应适当地采用不同的颜色值描述相关对象，并将其存储于GeometricObject对象中。
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            sr.hit_an_object    = true;
            tmin                = t;
            // sr.material_ptr     = objects[j]->get_material();
            sr.hit_point        = ray.o + t * ray.d;
            normal              = sr.normal;
            local_hit_point = sr.local_hit_point;
            // 将最近对象间颜色值存储于ShadeRec对象中的计算过程。
            sr.color = objects[j]->get_color();
        }

    if(sr.hit_an_object) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return(sr);
}

// #include "BuildRedSphere.cpp"
void World::build()
{
    int num_samples = 100;

    // 设置图像的分辨率和像素尺寸。
    vp.set_hres(400);
    vp.set_vres(400);
    vp.set_pixel_size(0.05);
    Sampler * objSampler = new MultiJittered(num_samples);
    vp.set_sampler(objSampler);
    // vp.set_gamma(1.0);
    vp.set_max_depth(0);

    tracer_ptr = new MultipleObjects(this);
    background_color = white;

    Ambient* ambient_ptr = new Ambient;
    ambient_ptr->scale_radiance(0.5);
    set_ambient_light(ambient_ptr);

    ThinLens* thin_lens_ptr=new ThinLens;
    thin_lens_ptr->set_sampler(new MultiJittered(num_samples));
	// We can not see anything from this position and direction
    // thin_lens_ptr->set_eye(0, 6, 50);
    // thin_lens_ptr->set_lookat(0, 6, 0);
    thin_lens_ptr->set_eye(300, 400, 500);
    thin_lens_ptr->set_lookat(0, 0, -50);
	// We can only see part
    thin_lens_ptr->set_view_distance(20.0);
    thin_lens_ptr->set_focal_distance(74.0);
    // PineHole 
    // thin_lens_ptr->set_lens_radius(0.0);
    // Default Setting 
    // thin_lens_ptr->set_lens_radius(1.0);
    // Blur Setting, 调大这个值会加强失焦效果，就好像你戴了一副远视镜一样。 
    thin_lens_ptr->set_lens_radius(5.0);
    thin_lens_ptr->compute_uvw();
    set_camera(thin_lens_ptr) ;

    Sphere *sphere_ptr = new Sphere;
    sphere_ptr->set_center(-10, -40, 0);
    sphere_ptr->set_radius(100.0);
    sphere_ptr->set_color(1.0, 0.0, 0.0);
    add_object(sphere_ptr);

    sphere_ptr = new Sphere;
    sphere_ptr->set_center(0, 60, 0);
    sphere_ptr->set_radius(200.0);
    sphere_ptr->set_color(1.0, 1.0, 0.0);
    add_object(sphere_ptr);

    Plane *plane_ptr = new Plane;
    plane_ptr->a = Vector3D(0.0);
    plane_ptr->n = Vector3D(0.6, 0.3, 0.7);
    plane_ptr->set_color(0.0, 0.30, 0.0);
    add_object(plane_ptr);

}

