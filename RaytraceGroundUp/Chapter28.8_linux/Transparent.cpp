#include "Transparent.h"

// ---------------------------------------------------------------- default constructor

Transparent::Transparent (void)
    :    Phong(),
        reflective_brdf(new PerfectSpecular),
        specular_btdf(new PerfectTransmitter)
{}


// ---------------------------------------------------------------- copy constructor

Transparent::Transparent(const Transparent& tm)
    :     Phong(tm) {

    if(tm.reflective_brdf)
        reflective_brdf = tm.reflective_brdf->clone();
    else
        reflective_brdf = NULL;

    if(tm.specular_btdf)
        specular_btdf = tm.specular_btdf->clone();
    else
        specular_btdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

Transparent&
Transparent::operator= (const Transparent& rhs) {
    if (this == &rhs)
        return (*this);

    Phong::operator=(rhs);

    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }

    if (rhs.reflective_brdf)
        reflective_brdf = rhs.reflective_brdf->clone();

    if (specular_btdf) {
        delete specular_btdf;
        specular_btdf = NULL;
    }

    if (rhs.specular_btdf)
        specular_btdf = rhs.specular_btdf->clone();

    return (*this);
}


// ---------------------------------------------------------------- clone

Transparent*
Transparent::clone(void) const {
    return (new Transparent(*this));
}


// ---------------------------------------------------------------- destructor

Transparent::~Transparent(void) {
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
    if (specular_btdf) {
        delete specular_btdf;
        specular_btdf = NULL;
    }
}


// ------------------------------------------------------------------------------------ shade

RGBColor
Transparent::shade(ShadeRec& sr) {
    RGBColor L(Phong::shade(sr));  // direct illumination

    Vector3D wo = -sr.ray.d;
    Vector3D wi;
    RGBColor fr = reflective_brdf->sample_f(sr, wo, wi);    // computes wi
    // 反射光线
    Ray reflected_ray(sr.hit_point, wi);

    // 当出现全内反射时， 
    if (specular_btdf->tir(sr)) { // test total internal reflection
        // 辐射度仅通过反射光线的返回值实现数量上的增加，且无需乘以BRDF以及cosθi。
        L += sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1);
        // 此时， 也无需令BRDF使用kr=1这一条件以及正常情况下(无全内反射现象)的kr值。

        // kr = 1.0
    }
    else {
        Vector3D wt;
        RGBColor ft = specular_btdf->sample_f(sr, wo, wt); // computes wt
        Ray transmitted_ray(sr.hit_point, wt);
        // 计算反射光线
        L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1)
                * fabs(sr.normal * wi);
        // 计算透射
        L += ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, sr.depth + 1)
                * fabs(sr.normal * wt);
    }

    return (L);
}
