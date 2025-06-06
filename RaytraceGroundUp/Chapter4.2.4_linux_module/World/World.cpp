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


// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
    :      background_color(black),
        tracer_ptr(NULL)
{}

World::~World(void) {

    if(tracer_ptr) {
        delete tracer_ptr;
        tracer_ptr = NULL;
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
    // 各像素的采样数量均为平方级别，1即针对某一整数n，num_samples=n * n。
    int n=(int)sqrt((float) vp.num_samples);
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
            // P54: 基于光线源点的(xw，y\w)坐标可分别表示为：
            //    x = vp.s * (c - 0.5 * (vp.hres - 1.0));
            //    y = vp.s * (r - 0.5 * (vp.vres - 1.0));
            // 该式的推导过程可参见练习3.13。
            //    ray.o = Point3D(x, y, zw);
            //    pixel_color = tracer_ptr->trace_ray(ray);
            // 下面显示了均匀采样的实现过程，该程序代码在前一章的基础上进行了相应的修改，
            // 其中，在视平面内存储了一定数量的像素采样点。这里所采用的均匀采样模式提供了相同的采样
            // 行和采样列，因此，各像素的采样数量均为平方级别，即针对某一整数n，num_samples = n x n。
            // 多次采样。
            for(int p=0; p<n; p++)  //up pixel 
            {
                for(int q=0; q<n; q++) {  //across pixel 
                    // 使用随机数加上q以后除以n来把随机数限制在栅格中实现抖动采样。
                    pp.x = vp.s*(c-0.5*vp.hres+(q+rand_float())/n);
                    pp.y = vp.s*(r-0.5*vp.vres+(p+rand_float())/n);
                    ray.o = Point3D(pp.x, pp.y, zw);
                    // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                    // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                    // 同时，还将返回各像素的颜色值。
                    // 但此处采取了一种间接的调用方案， 
                    // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                    pixel_color += tracer_ptr->trace_ray(ray);
                }
            }
            // 计算采样的平均值。
            pixel_color /= vp.num_samples; // average the colors
            // 循环体中最后一行代码将调用display_pixel() 函数以在窗口中显示像素。
            // 这将会把RGBColor转换为当前计算机支持的显示格式。
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

    // printf("%d %d %d\n", (int)(mapped_color.r * 255), (int)(mapped_color.g * 255), (int)(mapped_color.b * 255));
    // paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
    //                          (int)(mapped_color.g * 255),
    //                          (int)(mapped_color.b * 255));
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
    // vp.set_hres(200);
    // vp.set_vres(200);
    // vp.set_pixel_size(1.0);
    // vp.set_gamma(1.0);
    // background_color = black;
    // tracer_ptr = new MultipleObjects(this);
    // Sphere * sphere = new Sphere;
    // sphere->set_center(100,100,30);
    // sphere->set_radius(85.0);
    // sphere->set_color(RGBColor(1,0,0));
    // add_object(sphere);
    // 设置图像的分辨率和像素尺寸。
    vp.set_hres(400);
    vp.set_vres(400);
    vp.set_pixel_size(1);
    vp.set_gamma(1.0);
    background_color = blue;

    // 
    // tracer_ptr = new SingleSphere(this);
    // sphere.set_center(0.0);
    // sphere.set_radius(200);

    tracer_ptr = new MultipleObjects(this);
    Sphere *sphere_ptr = new Sphere;
    sphere_ptr->set_center(-10, -40, 0);
    sphere_ptr->set_radius(100.0);
    sphere_ptr->set_color(1.0, 0.0, 0.0);
    add_object(sphere_ptr);

//    sphere_ptr = new Sphere(Vector3D(0, 60, 0), 80.0);

    sphere_ptr = new Sphere;
    sphere_ptr->set_center(0, 60, 0);
    sphere_ptr->set_radius(80.0);
    sphere_ptr->set_color(1.0, 1.0, 0.0);
    add_object(sphere_ptr);

    Plane *plane_ptr = new Plane;
    plane_ptr->a = Vector3D(0.0);
    plane_ptr->n = Vector3D(0.6, 0.3, 0.7);
    plane_ptr->set_color(0.0, 0.30, 0.0);
    add_object(plane_ptr);
}

