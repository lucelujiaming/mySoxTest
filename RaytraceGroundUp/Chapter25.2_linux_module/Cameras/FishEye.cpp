// This file contains the definition of the FishEye class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "FishEye.h"

// ----------------------------------------------------------------------------- default constructor

FishEye::FishEye(void)
    :    Camera(),
        psi_max(180.0) // is this a good default value?
{}


// ----------------------------------------------------------------------------- copy constructor

FishEye::FishEye(const FishEye& c)
    :    Camera(c),
        psi_max(c.psi_max)
{}


// ----------------------------------------------------------------------------- clone

Camera*
FishEye::clone(void) const {
    return (new FishEye(*this));
}


// ----------------------------------------------------------------------------- assignment operator

FishEye&
FishEye::operator=(const FishEye& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator=(rhs);

    psi_max = rhs.psi_max;

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

FishEye::~FishEye(void) {}


// ----------------------------------------------------------------------------- ray direction
// explained on page 188
// 通过观察图11.3(a)显示的鱼眼投影系统的横截面以及相应的渲染窗口。
// 针对窗口内的某一给定点，将计算其投射光线与视见方向之间的角度w。
// 该角度值可视为该点与窗口中心位置之间的直线距离r的函数。
// 最终结果为：窗口中心位置处的同心圆将映射为w的常量值(如图11.3(b)所示)。
Vector3D
FishEye::ray_direction( const Point2D&    pp,
                        const int        hres,
                        const int        vres,
                        const float     s,
                        float&            r_squared) const {
    // compute the normalized device coordinates
    // 为了更好地执行任务， 首先：
    // 1. 将采样点的视见(观察) 坐标转换为标准的设备坐标。
    //    参见公式11.1和图11.5
    Point2D pn(2.0 / (s * hres) * pp.x, 2.0 / (s * vres) * pp.y);
    // 针对视平面上的各个采样点，可执行下列操作：
    // 2. 计算距离值r；
    //    参见公式11.2
    r_squared = pn.x * pn.x + pn.y * pn.y;
    // 3. 如果像素位于圆内，则计算w(r)；
    if (r_squared <= 1.0) {
        // 4. 在世界坐标系统内计算光线方向。
        float r         = sqrt(r_squared);
        // 参见公式11.3
		// 使用PI_ON_180把角度转换为弧度。
        float psi         = r * psi_max * PI_ON_180;
        float sin_psi     = sin(psi);
        float cos_psi     = cos(psi);
		// alpha为标准设备坐标以及直线r与x，轴之间的角度α。
		// 这里需要计算α角的正弦值和余弦值，如下所示：
        float sin_alpha = pn.y / r;
        float cos_alpha = pn.x / r;
        // 因此，d可按如下方式加以计算：
		// 参见公式11.4
		// 需要注意的是，该向量的计算结果为单位向量。
        Vector3D dir     = sin_psi * cos_alpha * u +  sin_psi * sin_alpha * v - cos_psi * w;
        return (dir);
    }
    else
        return (Vector3D(0.0));
}


// ----------------------------------------------------------------------------- render_stereo

void
FishEye::render_stereo(const World& w, float x, int pixel_offset) {

        // not implemented yet
}


// ----------------------------------------------------------------------------- render scene
// explained on page 187
void
FishEye::render_scene(const World& wr) {

    RGBColor    L;
    ViewPlane    vp(wr.vp);
    int         hres        = vp.hres;
    int         vres         = vp.vres;
    float        s             = vp.s;
    Ray            ray;
    int         depth         = 0;
    Point2D     sp;                     // sample point in [0, 1] X [0, 1]
    Point2D     pp;                        // sample point on the pixel
    float        r_squared;                // sum of squares of normalised device coordinates

    ray.o = eye;

    for (int r = 0; r < vres; r++)        // up
        for (int c = 0; c < hres; c++) {    // across

            L = black;
            for (int j = 0; j < vp.num_samples; j++) {
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = s * (c - 0.5 * hres + sp.x);
                pp.y = s * (r - 0.5 * vres + sp.y);
				// 这个类的关键就是如何计算光线方向。
				// 鱼眼镜头会改变光线方向，把平行光线映射到一个圆锥上。
                ray.d = ray_direction(pp, hres, vres, s, r_squared);

                if (r_squared <= 1.0)
                {
                      // L += wr.tracer_ptr->trace_ray(ray, depth);
                      L += wr.tracer_ptr->trace_ray(ray);
                }
            }

            L /= vp.num_samples;
            L *= exposure_time;
            wr.display_pixel(r, c, L);
        }
}
