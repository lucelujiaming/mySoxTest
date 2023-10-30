#include "PerfectSpecular.h"


// ---------------------------------------------------------- default constructor

PerfectSpecular::PerfectSpecular(void)
    :    BRDF(),
        kr(0.0),
        cr(1.0)
{}


// ---------------------------------------------------------- destructor

PerfectSpecular::~PerfectSpecular(void) {}


// ---------------------------------------------------------------------- clone

PerfectSpecular*
PerfectSpecular::clone(void) const {
    return (new PerfectSpecular(*this));
}


// ---------------------------------------------------------- f
// 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
// 返回黑色。因为反射光线位于镜面反射方向上，不存在BRDF随机采样。
RGBColor
PerfectSpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ---------------------------------------------------------- sample_f
// this computes wi: the direction of perfect mirror reflection
// it's called from from the functions Reflective::shade and Transparent::shade.
// the fabs in the last statement is for transparency
// explained on page 500
// 返回BRDF比例系数。
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
RGBColor
PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const {
    // 参见公式24.2
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;
    // why is this fabs? 
    // kr would be a Fresnel term in a Fresnel reflector、
    // for transparency when ray hits inside surface?, if so it should go in Chapter 24
    return (kr * cr / fabs(sr.normal * wi)); 
}                                             


// ---------------------------------------------------------- sample_f
// this version of sample_f is used with path tracing
// it returns ndotwi in the pdf
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
// 使用Monte Calo积分实现。
RGBColor
PerfectSpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
    // 参见公式24.2
    float ndotwo = sr.normal * wo;
    wi = -wo + 2.0 * sr.normal * ndotwo;
    pdf = fabs(sr.normal * wi);
    return (kr * cr);
}


// ---------------------------------------------------------- rho
// 返回双半球反射系数。
RGBColor
PerfectSpecular::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
