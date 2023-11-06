#include "Dielectric.h"

// ---------------------------------------------------------------- default constructor

Dielectric::Dielectric (void)
    :    Phong(),
        fresnel_brdf(new FresnelReflector),
        fresnel_btdf(new FresnelTransmitter)
{}


// ---------------------------------------------------------------- copy constructor

Dielectric::Dielectric(const Dielectric& dm)
    :     Phong(dm) {

    if(dm.fresnel_brdf)
        fresnel_brdf = dm.fresnel_brdf->clone();
    else
        fresnel_brdf = NULL;

    if(dm.fresnel_btdf)
        fresnel_btdf = dm.fresnel_btdf->clone();
    else
        fresnel_btdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

Dielectric&
Dielectric::operator= (const Dielectric& rhs) {
    if (this == &rhs)
        return (*this);

    Phong::operator=(rhs);

    if (fresnel_brdf) {
        delete fresnel_brdf;
        fresnel_brdf = NULL;
    }

    if (rhs.fresnel_brdf)
        fresnel_brdf = rhs.fresnel_brdf->clone();

    if (fresnel_btdf) {
        delete fresnel_btdf;
        fresnel_btdf = NULL;
    }

    if (rhs.fresnel_btdf)
        fresnel_btdf = rhs.fresnel_btdf->clone();

    return (*this);
}


// ---------------------------------------------------------------- clone

Dielectric*
Dielectric::clone(void) const {
    return (new Dielectric(*this));
}


// ---------------------------------------------------------------- destructor

Dielectric::~Dielectric(void) {
    if (fresnel_brdf) {
        delete fresnel_brdf;
        fresnel_brdf = NULL;
    }
    if (fresnel_btdf) {
        delete fresnel_btdf;
        fresnel_btdf = NULL;
    }
}


// ------------------------------------------------------------------------------------ shade

RGBColor
Dielectric::shade(ShadeRec& sr) {
    RGBColor L(Phong::shade(sr));

    Vector3D     wi;
    Vector3D     wo(-sr.ray.d);
    RGBColor     fr = fresnel_brdf->sample_f(sr, wo, wi);      // computes wi
    // 反射光线
    Ray         reflected_ray(sr.hit_point, wi);
    float         t;
    RGBColor     Lr, Lt;
    float         ndotwi =  sr.normal * wi;
    // 如果存在全内反射，
    // 则该函数将对位于介质内部或外部的反射光线进行跟踪。
    if(fresnel_btdf->tir(sr)) {                                // total internal reflection
        // 在对光线实施跟踪计算之前，首先需要确定该光线位于介质的内部还是外部，
        if (ndotwi < 0.0) {
            // reflected ray is inside
            // 计算在内部的反射光线
            Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1);
            // 这将直接决定相应的过滤颜色值。
            L += cf_in.powc(t) * Lr;                           // inside filter color
        }
        else {
            // reflected ray is outside
            // 计算在外部的反射光线
            Lr = sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1);   // kr = 1
            // 这将直接决定相应的过滤颜色值。
            L += cf_out.powc(t) * Lr;                       // outside filter color
        }
    }
    // 如果不存在全内反射，
    // 则代码将对一条反射光线以及一条透射光线进行跟踪计算，且相关光线将位于介质的内部或外部。
    else {                                                     // no total internal reflection
        Vector3D wt;
        RGBColor ft = fresnel_btdf->sample_f(sr, wo, wt);      // computes wt
        Ray transmitted_ray(sr.hit_point, wt);
        float ndotwt = sr.normal * wt;
        // 在对光线实施跟踪计算之前，首先需要确定该光线位于介质的内部还是外部，这将直接决定相应的过滤颜色值。
        if (ndotwi < 0.0) {
            // reflected ray is inside
            // 计算在内部的反射光线
            Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
            // 这将直接决定相应的过滤颜色值。
            L += cf_in.powc(t) * Lr;                         // inside filter color

            // transmitted ray is outside
            // 计算在外部的透射
            Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
            // 这将直接决定相应的过滤颜色值。
            L += cf_out.powc(t) * Lt;                       // outside filter color
        }
        else {
            // reflected ray is outside
            // 计算在外部的反射光线
            Lr = fr * sr.w.tracer_ptr->trace_ray(reflected_ray, t, sr.depth + 1) * fabs(ndotwi);
            // 这将直接决定相应的过滤颜色值。
            L += cf_out.powc(t) * Lr;                        // outside filter color

            // transmitted ray is inside
            // 计算在内部的透射
            Lt = ft * sr.w.tracer_ptr->trace_ray(transmitted_ray, t, sr.depth + 1) * fabs(ndotwt);
            // 这将直接决定相应的过滤颜色值。
            L += cf_in.powc(t) * Lt;                         // inside filter color
        }
    }

    return (L);
}
