#include "stdafx.h"
#include "Reflective.h"

// ---------------------------------------------------------------- default constructor

Reflective::Reflective (void)
    :    Phong(),
        reflective_brdf(new PerfectSpecular)
{}


// ---------------------------------------------------------------- copy constructor

Reflective::Reflective(const Reflective& rm)
    :     Phong(rm) {
    
    if(rm.reflective_brdf)
        reflective_brdf = rm.reflective_brdf->clone(); 
    else  
        reflective_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

Reflective& 
Reflective::operator= (const Reflective& rhs) {
    if (this == &rhs)
        return (*this);
        
    Phong::operator=(rhs);
    
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }

    if (rhs.reflective_brdf)
        reflective_brdf = rhs.reflective_brdf->clone();

    return (*this);
}

// ---------------------------------------------------------------- clone

Reflective*                                        
Reflective::clone(void) const {
    return (new Reflective(*this));
}    

// ---------------------------------------------------------------- destructor

Reflective::~Reflective(void) {
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
}

// ------------------------------------------------------------------------------------ shade 
// explained on page 501
RGBColor
Reflective::shade(ShadeRec& sr) {    
    // 直接光照。
    RGBColor L(Phong::shade(sr));  // direct illumination
    
    Vector3D wo = -sr.ray.d;
    Vector3D wi;    
    RGBColor fr = reflective_brdf->sample_f(sr, wo, wi); 
    // 反射光线的起点为碰撞点，方向为反射方向。
    Ray reflected_ray(sr.hit_point, wi); 
    // 累加反射次数。
    reflected_ray.depth = sr.depth + 1;
    
    // 调用sr.w.tracer_ptr->trace_ray完成递归。
    L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);
                    
    return (L);
}



