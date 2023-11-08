// This file contains the definition of the Spherical class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Spherical.h"

// ----------------------------------------------------------------------------- default constructor

Spherical::Spherical(void)
    :    Camera(),
        psi_max(180.0) // is this a good default value?
{}


// ----------------------------------------------------------------------------- copy constructor

Spherical::Spherical(const Spherical& c)
    :    Camera(c),
        psi_max(c.psi_max)
{}


// ----------------------------------------------------------------------------- clone

Camera*
Spherical::clone(void) const {
    return (new Spherical(*this));
}


// ----------------------------------------------------------------------------- assignment operator

Spherical&
Spherical::operator=(const Spherical& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator=(rhs);

    psi_max = rhs.psi_max;

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

Spherical::~Spherical(void) {}


// ----------------------------------------------------------------------------- ray direction
// explained on page 192
Vector3D
Spherical::ray_direction(    const Point2D&    pp,
                            const int         hres,
                            const int         vres,
                            const float     s    ) const {

    // compute the normalised device coordinates
    // 为了更好地执行任务， 首先：
    // 1. 将采样点的视见(观察) 坐标转换为标准的设备坐标。
    //    参见公式11.1和图11.5
    Point2D pn( 2.0 / (s * hres) * pp.x, 2.0 / (s * vres) * pp.y);

    // compute the angles lambda and phi in radians
    // 把像素点的标准设备坐标定义方位分离角lambda以及极角psi
    // 转换为角度。
    float lambda     = pn.x * lambda_max * PI_ON_180;
    float psi         = pn.y * psi_max * PI_ON_180;

    // compute the regular azimuth and polar angles
    // 像素点的标准设备坐标定义方位分离角lambda以及极角psi
    // 与球体坐标角度phi和theta之间存在下列简单的计算关系。
    //    参见公式11.5
    float phi         = PI - lambda;
    float theta     = 0.5 * PI - psi;

    float sin_phi     = sin(phi);
    float cos_phi     = cos(phi);
    float sin_theta = sin(theta);
    float cos_theta = cos(theta);

    // equation 11.6
    // 通过将lambda和psi与式(11.5)中的标准球体坐标角度相关联，
    // 并利用r=1)，可计算光线方向为：
    //    参见公式11.6
    Vector3D dir     = sin_theta * sin_phi * u + cos_theta * v + sin_theta * cos_phi * w;
    return (dir);
}


// ----------------------------------------------------------------------------- render_stereo

void
Spherical::render_stereo(const World& w, float x, int pixel_offset) {

        // not implemented yet
}


// ----------------------------------------------------------------------------- render scene
// explained on page 191
void
Spherical::render_scene(const World& wr) {

    RGBColor    L;
    ViewPlane    vp(wr.vp);
    int         hres        = vp.hres;
    int         vres         = vp.vres;
    float        s             = vp.s;
    Ray            ray;
    int         depth         = 0;
    Point2D     sp;                     // 视平面采样器对象中的采样点：sample point in [0, 1] X [0, 1]
    Point2D     pp;                     // 根据采样器对象中的采样点计算出来的像素点：sample point on the pixel

    ray.o = eye;

    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vres; r++)        // up
        for (int c = 0; c < hres; c++) {    // across
            L = black;
            // 多次采样。
            for (int j = 0; j < vp.num_samples; j++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = s * (c - 0.5 * hres + sp.x);
                pp.y = s * (r - 0.5 * vres + sp.y);
                // 4. 计算光线方向。
                ray.d = ray_direction(pp, hres, vres, s);
                // L += wr.tracer_ptr->trace_ray(ray, depth);
                // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                // 同时，还将返回各像素的颜色值。
                // 但此处采取了一种间接的调用方案， 
                // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
                L += wr.tracer_ptr->trace_ray(ray);
            }
            // 计算采样的平均值。
            L /= vp.num_samples;
            // 处理曝光强度系数，默认为1。
            L *= exposure_time;
            // 循环体中最后一行代码将调用display_pixel() 函数以在窗口中显示像素。
            // 这将会把RGBColor转换为当前计算机支持的显示格式。
            wr.display_pixel(r, c, L);
        }
}
