// This file contians the definition of the class BRDF

#include "stdafx.h"
#include "Constants.h"
#include "BRDF.h"

// ---------------------------------------------------------- default constructor

BRDF::BRDF(void) {}


BRDF::BRDF(const BRDF& brdf) {}


// --------------------------------------------------------------- assignment operator

BRDF&
BRDF::operator= (const BRDF& rhs) {

    if (this == &rhs)
        return (*this);

    return (*this);
}


// ---------------------------------------------------------- destructor

BRDF::~BRDF(void) {}


// ------------------------------------------------------------------------ f
// 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
RGBColor
BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------------ sample_f
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
RGBColor
BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------------ sample_f
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
// 使用Monte Calo积分实现。
RGBColor
BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
    return (black);
}


// ------------------------------------------------------------------------ rho    
// 返回双半球反射系数。
RGBColor
BRDF::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
