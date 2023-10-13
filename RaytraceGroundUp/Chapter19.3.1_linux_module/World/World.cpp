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

#include "SphereConcave.h"
#include "EnvironmentLight.h"

#include "Triangle.h"

// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
    :      background_color(black),
        tracer_ptr(NULL),
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
                sp = vp.sampler_ptr->sample_hemisphere();
                pp.x = vp.s*(c-0.5*vp.hres + sp.x);
                pp.y = vp.s*(r-0.5*vp.vres + sp.y);
                ray.o = Point3D(pp.x, pp.y, zw);
                pixel_color += tracer_ptr->trace_ray(ray);
            }

            pixel_color /= vp.num_samples; // average the colors

            display_pixel(r, c, pixel_color);
        }
    }
}

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
            //  We can use material now
            // Lujiaming uncomment at 230913
            sr.material_ptr     = objects[j]->get_material();
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

    int num_samples=256;

    vp.set_hres(400);
    vp.set_vres(400);
    vp.set_samples(num_samples);
    
    tracer_ptr=new RayCast(this);
    MultiJittered* sampler_ptr=new MultiJittered(num_samples);
    
    Emissive*emissive_ptr=new Emissive;
    emissive_ptr->set_ce(1.0, 1.0, 0.5);
    // emissive_ptr->set_brightness(1.0) ;

    Triangle * triangle_ptr1 = new Triangle(Point3D(125, 125, 135), 
                Point3D(51, 52, 53), Point3D(91, 92, 93));
    add_object(triangle_ptr1);


    EnvironmentLight* light_ptr=new EnvironmentLight;
    light_ptr->set_material(emissive_ptr);
    light_ptr->set_sampler(new MultiJittered(num_samples) ) ;
    light_ptr->set_shadows(true);
    add_light(light_ptr) ;

     Pinhole* pinhole_ptr = new Pinhole;
     pinhole_ptr->set_eye(25, 20, 45);
     pinhole_ptr->set_lookat(0, 1, 0);
     pinhole_ptr->set_view_distance(5000);
     pinhole_ptr->compute_uvw();
     set_camera(pinhole_ptr);

}

