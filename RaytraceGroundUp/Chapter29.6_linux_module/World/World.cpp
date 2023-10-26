#include "stdafx.h"
// this file contains the definition of the World class

// #include "wxraytracer.h"

#include <stdlib.h>
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
#include "RayCast.h"
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
#include "FishEye.h"
#include "Spherical.h"
#include "StereoCamera.h"
#include "PointLight.h"
#include "Matte.h"
#include "Phong.h"

#include "AmbientOccluder.h"

#include "MyRectangle.h"
#include "Emissive.h"
#include "AreaLight.h"
#include "AreaLighting.h"

#include "SphereConcave.h"
#include "EnvironmentLight.h"

#include "Triangle.h"
#include "Disk.h"
#include "Rectangle.h"
#include "OpenCylinder.h"
#include "Torus.h"

#include "OpenCylinderConvex.h"
#include "OpenCylinderConcave.h"
#include "OpenCylinderPartConcave.h"
#include "OpenCylinderPartConvex.h"

#include "SpherePartConvex.h"
#include "SpherePartConcave.h"

#include "TorusPartConvex.h"
#include "TorusPartConcave.h"
#include "TorusPart.h"

#include "SolidCylinder.h"

#include "Instance.h"

#include "BeveledBox.h"
#include "BeveledWedge.h"

#include "Annulus.h"
#include "AnnulusPart.h"


#include "Grid.h"
#include "SmoothMeshTriangle.h"

#include "Whitted.h"
#include "Reflective.h"

#include "GlossyReflector.h"

#include "PathTrace.h"

#include "Transparent.h"
#include "Dielectric.h"

#include "GlassOfWater.h"
#include "FishBowl.h"

// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
    :      background_color(black),
        tracer_ptr(NULL),
        // 环境光照被默认初始化为Ambient类型
        ambient_ptr(new Ambient),
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
    
    // 作为默认渲染函数，光线方向指向Z轴的反方向，垂直于XY平面。
    // 因为视平面被设置在zw=100处，且平行于XY平面。
    // 光线方向指向Z轴的反方向相当于从视平面发出一道一道光线。
    ray.d = Vector3D(0, 0, -1);
    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vp.vres; r++) {
        for (int c = 0; c < vp.hres; c++) {
            pixel_color = background_color;
            // 多次采样。
            for(int j=0; j < vp.num_samples; j++) {
                // 返回存储于采样器对象中的下一个采样点，映射到半球体。
                // 其中sp.z不使用。
                sp = vp.sampler_ptr->sample_hemisphere();
                pp.x = vp.s*(c-0.5*vp.hres + sp.x);
                pp.y = vp.s*(r-0.5*vp.vres + sp.y);
                
                // 使用计算出来的XY坐标和默认的视平面的Z坐标位置作为光线起点。
                ray.o = Point3D(pp.x, pp.y, zw);
                // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                // 同时，还将返回各像素的颜色值。
                // 但此处采取了一种间接的调用方案， 
                // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                pixel_color += tracer_ptr->trace_ray(ray);
            }
            // 计算采样的平均值。
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
            // P114：参见公式8.1
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
// 使用全局色阶映射因子解决溢色问题。
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
// 把溢出值设定为指定颜色用来辨别色溢位置。
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
    // 解决溢色问题。
    // 如果需要辨别色溢位置，
    if (vp.show_out_of_gamut)
    {
        // 把溢出值设定为指定颜色用来辨别色溢位置。
        mapped_color = clamp_to_color(raw_color);
    }
    // 如果不需要辨别色溢位置，
    else
    {
        // 使用全局色阶映射因子解决溢色问题。
        mapped_color = max_to_one(raw_color);
    }
    // 显示器的亮度值通常与工作电压呈非线性关系， 因而gamma值修正往往是必要的。
    if (vp.gamma != 1.0)
        mapped_color = mapped_color.powc(vp.inv_gamma);

    //have to start from max y coordinate to convert to screen coordinates
    int x = column;
    int y = vp.vres - row - 1;

    // paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
    //                          (int)(mapped_color.g * 255),
    //                          (int)(mapped_color.b * 255));
	// cout << "raw_color : " << (raw_color.r) << " " << (raw_color.g) << " " << (raw_color.b) << endl;
	// cout << "mapped_color : " << (mapped_color.r) << " " << (mapped_color.g) << " " << (mapped_color.b) << endl;
	// cout << "mapped_color.r * 255 : " << (int)(mapped_color.r * 255) << " " << (int)(mapped_color.g * 255) << " " << (int)(mapped_color.b * 255) << endl;
	
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
    // 首先把tmin初始化为一个非常大的值。
    double      tmin            = kHugeValue;
    int         num_objects     = objects.size();

    for (int j = 0; j < num_objects; j++)
    {
        // 代码并未定义特定的几何对象类型，因而适用于几何对象继承层次结构中的任意对象类型，
        // 并实现了基于hit()函数的公共接口。
        // 考虑到对象数量的增加，应适当地采用不同的颜色值描述相关对象，并将其存储于GeometricObject对象中。
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            sr.hit_an_object    = true;
            // 如果找到一个比当前的tmin更靠近视点的t，就把tmin更新为这个值。
            tmin                = t;
            //  We can use material now
            // Lujiaming uncomment at 230913
            // 记下材质指针。
            sr.material_ptr     = objects[j]->get_material();
            // 算出世界坐标系中的碰撞点坐标。
            sr.hit_point        = ray.o + t * ray.d;
            // 我们使用局部变量保存法线和局部碰撞点坐标。
            normal              = sr.normal;
            // 这个sr.local_hit_point是objects[j]计算出来的碰撞点坐标。
            local_hit_point = sr.local_hit_point;
            // 将最近对象间颜色值存储于ShadeRec对象中的计算过程。
            sr.color = objects[j]->get_color();
        }
    }
    // 在循环结束以后，把前面保存的法线和objects[j]计算出来的碰撞点存回ShadeRec对象中。
    if(sr.hit_an_object) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return(sr);
}

double randf()
{
    return (double)(rand()/(double)RAND_MAX);
}

// #include "BuildRedSphere.cpp"
void World::build()
{
	//construct view plane， integrator， camera， and lights
	// int num_spheres=100000;
	int num_samples = 1;
	vp.set_hres(600) ;
	vp.set_vres(400) ;
	vp.set_samples(num_samples) ;
	vp.set_max_depth(10) ;

    tracer_ptr=new Whitted(this) ;
    GlossyReflector* reflective_ptr1=new GlossyReflector;
	reflective_ptr1->set_ka(0.25) ;
	reflective_ptr1->set_kd(0.5) ;
	reflective_ptr1->set_cd(0.75, 0.75, 0) ;
	reflective_ptr1->set_ks(0.15) ;
	reflective_ptr1->set_exp(100) ;
	reflective_ptr1->set_kr(0.75) ;
	reflective_ptr1->set_cr(white) ;

    PointLight *light_ptr=new PointLight();
    light_ptr->set_location(50, 50, 1);
    light_ptr->scale_radiance(3.0);
    add_light(light_ptr);

	Phong * phong_ptr = new Phong;
	phong_ptr->set_ka(0.25);
	phong_ptr->set_kd(0.65);
	phong_ptr->set_cd(1, 1, 0);

    Dielectric*dielectric_ptr=new Dielectric;
    dielectric_ptr->set_ks(0.2) ;

    dielectric_ptr->set_exp(2000) ;
    dielectric_ptr->set_eta_in(1.5) ;
    dielectric_ptr->set_eta_out(1.0) ;
    dielectric_ptr->set_cf_in(1.0) ;
    dielectric_ptr->set_cf_out(1.0) ;

    FishBowl* fishbowl_ptr1= new FishBowl() ;
    fishbowl_ptr1->set_material(dielectric_ptr) ;
    add_object(fishbowl_ptr1) ;

	// 设定相机
	Pinhole* pinhole_ptr = new Pinhole;
	pinhole_ptr->set_eye(0, 0, 500);
	pinhole_ptr->set_lookat(0, 0, -50);
	pinhole_ptr->set_view_distance(8000);
	pinhole_ptr->compute_uvw();
	set_camera(pinhole_ptr);

}

