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
    Point2D     sp;                 // sample point in [0, 1] X [0, 1]
    Point2D     pp;                    // sample point on the pixel

    vp.s /= zoom;
    ray.o = eye;

    for (int r = 0; r < vp.vres; r++)            // up
        for (int c = 0; c < vp.hres; c++) {        // across

            L = black;
            for (int j = 0; j < vp.num_samples; j++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - 0.5 * vp.hres + sp.x) + x;    // asymmetric view frustum
                pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
                ray.d = get_direction(pp);
                // L += w.tracer_ptr->trace_ray(ray, depth);
                L += w.tracer_ptr->trace_ray(ray);
            }

            L /= vp.num_samples;
            L *= exposure_time;    
            w.display_pixel(r, c + pixel_offset, L);
        }
}


// ----------------------------------------------------------------------------- render_scene

void
Pinhole::render_scene(const World& w) {

    RGBColor    L;
    ViewPlane    vp(w.vp);
    Ray            ray;
    int         depth = 0;    // recursion depth
    Point2D     sp;            // sample point in [0, 1] x [0, 1]
    Point2D     pp;            // sample point on a pixel
    int n = (int)sqrt((float)vp.num_samples);

    vp.s /= zoom;
    ray.o = eye;

    for (int r = 0; r < vp.vres; r++)            // up
        for (int c = 0; c < vp.hres; c++) {        // across

            L = black;
            for (int j = 0; j < vp.num_samples; j++) {
                // 返回ViewPlane中存储于采样器对象中的下一个采样点，映射到单位矩形。
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - 0.5 * vp.hres + sp.x);
                pp.y = vp.s * (r - 0.5 * vp.vres + sp.y);
                ray.d = get_direction(pp);
                // L += w.tracer_ptr->trace_ray(ray, depth);
                L += w.tracer_ptr->trace_ray(ray);
            }

            L /= vp.num_samples;
            L *= exposure_time;
            w.display_pixel(r, c, L);
        }
}
