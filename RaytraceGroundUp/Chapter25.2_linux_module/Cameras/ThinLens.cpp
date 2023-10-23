// This file contains the definition of the ThinLens class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "ThinLens.h"

// ----------------------------------------------------------------------------- default constructor

ThinLens::ThinLens(void)
    :    Camera(),
        lens_radius(1.0),
        d(500),
        f(75.0),
        zoom(1.0),
        sampler_ptr(NULL)
{}


// ----------------------------------------------------------------------------- copy constructor

ThinLens::ThinLens(const ThinLens& c)
    :    Camera(c),
        lens_radius(c.lens_radius),
        d(c.d),
        f(c.f),
        zoom(c.zoom)
{
    // need to do a deep copy of the sampler
    if(c.sampler_ptr != NULL) {
        sampler_ptr = c.sampler_ptr->clone();
    }
}


// ----------------------------------------------------------------------------- clone

Camera*
ThinLens::clone(void) const {
    return (new ThinLens(*this));
}


// ----------------------------------------------------------------------------- assignment operator

ThinLens&
ThinLens::operator=(const ThinLens& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator=(rhs);

    lens_radius = rhs.lens_radius;
    d            = rhs.d;
    f            = rhs.f;
    zoom        = rhs.zoom;

    // need to do a deep copy of the sampler
    if(rhs.sampler_ptr != NULL) {
        sampler_ptr = rhs.sampler_ptr->clone();
    }

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

ThinLens::~ThinLens(void) {

    delete sampler_ptr;
}


// ----------------------------------------------------------------------------- set_sampler

void
ThinLens::set_sampler(Sampler* sp) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // 采样分布将由所使用的采样对象类型
    // 当采用随机采样方案、抖动采样方案或多重抖动采样方案时，透镜表面上的光线源点将呈随机分布状态。
    // 最终结果为：随着透镜半径的增加，渲染图像将包含更多的噪点，但却可以快速地演示景深效果
    sampler_ptr = sp;
    // 函数仍将采样点映射至相应的单位圆上。
    sampler_ptr->map_samples_to_unit_disk();
}


// ----------------------------------------------------------------------------- ray direction
// explained on page 173
Vector3D
ThinLens::ray_direction(const Point2D& pixel_point, const Point2D& lens_point) const {
    // 根据三角形相似原理，参见公式10-1和10-2
    Point2D p(pixel_point.x * f / d, pixel_point.y * f / d);    // hit point on focal plane
    // 参见公式10-3
    Vector3D dir = (p.x - lens_point.x) * u + (p.y - lens_point.y) * v - f * w;
    // 计算出来的向量并非单位向量，因而在执行光线跟踪计算之前，有必要对其进行规范化操作。
    dir.normalize();

    return (dir);
}


// ----------------------------------------------------------------------------- render_stereo

void
ThinLens::render_stereo(const World& w, float x, int pixel_offset) {

        // not implemented yet
}


// ----------------------------------------------------------------------------- render scene

// 这个函数的逻辑和World::render_scene不一样的地方，也是每一个投影算法的实现原理。
// 就是我们根据上面计算出来的XY坐标，使用不同的投影公式计算投影后的XY坐标。
// 并且根据穿过镜头前的采样点和穿过镜头后的采样点计算光线方向。步骤如下：
void
ThinLens::render_scene(const World& w) {

    RGBColor    L;
    Ray            ray;
    ViewPlane    vp(w.vp);
    int         depth         = 0;

    Point2D sp;            // 视平面采样器对象中的采样点：sample point in [0, 1] X [0, 1]
    Point2D pp;            // 根据采样器对象中的采样点计算出来的像素点：sample point on a pixel
    Point2D dp;            // 透镜采样器对象中的采样点：sample point on unit disk
    Point2D lp;            // 透镜中的采样点：sample point on lens

    vp.s /= zoom;

    // 函数的主要工作都体现在for循环体内，该循环体将负责计算各像素的颜色值。
    // 在该函数中，场景将在窗口的左下角处被逐行渲染。
    // 但前者可将像素的颜色值写入一个离屏数组中，并可缓冲输出数据。
    for (int r = 0; r < vp.vres; r++)            // up
        for (int c = 0; c < vp.hres; c++) {        // across

            L = black;
            // 多次采样。
            for (int n = 0; n < vp.num_samples; n++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - vp.hres / 2.0 + sp.x);
                pp.y = vp.s * (r - vp.vres / 2.0 + sp.y);
                
                // 下面的逻辑就是和World::render_scene不一样的地方，也是每一个投影算法的实现原理。
                // 就是我们根据上面计算出来的XY坐标，使用不同的投影公式计算投影后的XY坐标。
                // 并且根据穿过镜头前的采样点和穿过镜头后的采样点计算光线方向。步骤如下：
                // 1. 返回透镜自己存储于采样器对象中的下一个采样点，映射到单位圆。
                dp = sampler_ptr->sample_unit_disk();
                // 2. 乘以镜头半径，得到镜头中的采样点。
                lp = dp * lens_radius;
                // 3. 使用镜头中的采样点计算光线起点
                ray.o = eye + lp.x * u + lp.y * v;
                // 4. 计算光线方向。
                ray.d = ray_direction(pp, lp);
                // L += w.tracer_ptr->trace_ray(ray, depth);
                // 待光线的源点和投射方向计算完毕后， 函数trace_ray() 将被调用。
                // 该函数是光线跟踪器中的核心函数，并负责对当前场景实施光线跟踪，
                // 同时，还将返回各像素的颜色值。
                // 但此处采取了一种间接的调用方案， 
                // 即对该函数通过指向Tracer对象的tracer_ptr指针加以调用。
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
