// This file contains the definition of the Pinhole class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Pinhole.h"
#include <math.h>

// ----------------------------------------------------------------------------- default constructor

Pinhole::Pinhole(void)
    :    Camera(),
        d(500),
        zoom(1.0)
{}


// ----------------------------------------------------------------------------- copy constructor

Pinhole::Pinhole(const Pinhole& c)
    :     Camera(c),
        d(c.d),
        zoom(c.zoom)
{}


// ----------------------------------------------------------------------------- clone

Camera*
Pinhole::clone(void) const {
    return (new Pinhole(*this));
}


// ----------------------------------------------------------------------------- assignment operator

Pinhole&
Pinhole::operator= (const Pinhole& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator= (rhs);

    d         = rhs.d;
    zoom    = rhs.zoom;

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

Pinhole::~Pinhole(void) {}


// ----------------------------------------------------------------------------- get_direction

Vector3D
Pinhole::get_direction(const Point2D& p) const {

    Vector3D dir = p.x * u + p.y * v - d * w;
    dir.normalize();
    
    return(dir);
}


// ----------------------------------------------------------------------------- render_stereo
// explained on page 209
void
Pinhole::render_stereo(const World& w, float x, int pixel_offset) {

    RGBColor    L;
    Ray            ray;
    ViewPlane    vp             = w.vp;
    int         depth         = 0;
    Point2D     sp;                    // sample point in [0, 1] X [0, 1]
    Point2D     pp;                    // sample point on the pixel

    vp.s /= zoom;
    ray.o = eye;

    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vp.vres; r++)              // up
        for (int c = 0; c < vp.hres; c++) {        // across
            L = black;
            // 多次采样。
            for (int j = 0; j < vp.num_samples; j++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - 0.5 * vp.hres + sp.x) + x;    // asymmetric view frustum
                pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
                ray.d = get_direction(pp);
                // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                // 同时，还将返回各像素的颜色值。
                // 但此处采取了一种间接的调用方案， 
                // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                // 对于我们现在拥有两个Tracer来说，
                // trace_ray(ray, depth)和trace_ray(ray)的实现相同。因此上，调用哪一个都可以。
                L += w.tracer_ptr->trace_ray(ray, depth);
            }
            // 计算采样的平均值。
            L /= vp.num_samples;
            // 处理曝光强度系数，默认为1。
            L *= exposure_time;    
            // 循环体中最后一行代码将调用display_pixel() 函数以在窗口中显示像素。
            // 这将会把RGBColor转换为当前计算机支持的显示格式。
            w.display_pixel(r, c + pixel_offset, L);
        }
}


// ----------------------------------------------------------------------------- render_scene

void
Pinhole::render_scene(const World& w) {

    RGBColor    L;
    ViewPlane    vp(w.vp);
    Ray            ray;
    int         depth = 0;     // recursion depth
    Point2D     sp;            // sample point in [0, 1] x [0, 1]
    Point2D     pp;            // sample point on a pixel
    int n = (int)sqrt((float)vp.num_samples);

    vp.s /= zoom;
    ray.o = eye;

    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vp.vres; r++)              // up
        for (int c = 0; c < vp.hres; c++) {        // across

            L = black;
            for (int j = 0; j < vp.num_samples; j++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
                pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
                ray.d = get_direction(pp);
                // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                // 同时，还将返回各像素的颜色值。
                // 但此处采取了一种间接的调用方案， 
                // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                // L += w.tracer_ptr->trace_ray(ray, depth);
                L += w.tracer_ptr->trace_ray(ray);
            }
            // 计算采样的平均值。
            L /= vp.num_samples;
            // 处理曝光强度系数，默认为1。
            L *= exposure_time;
            // 循环体中最后一行代码将调用display_pixel() 函数以在窗口中显示像素。
            // 这将会把RGBColor转换为当前计算机支持的显示格式。
            w.display_pixel(r, c, L);
        }
}
