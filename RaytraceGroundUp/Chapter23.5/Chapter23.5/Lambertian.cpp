#include "stdafx.h"
#include "Lambertian.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Lambertian::Lambertian(void)
    :   BRDF(),
        kd(0.0),
        cd(0.0)
{}


// ---------------------------------------------------------------------- copy constructor

Lambertian::Lambertian(const Lambertian& lamb)
    :   BRDF(lamb),
        kd(lamb.kd),
        cd(lamb.cd)
{
}


// ---------------------------------------------------------------------- clone

Lambertian*
Lambertian::clone(void) const {
    return (new Lambertian(*this));
}


// ---------------------------------------------------------------------- destructor

Lambertian::~Lambertian(void) {}


// ---------------------------------------------------------------------- assignment operator

Lambertian&
Lambertian::operator= (const Lambertian& rhs) {
    if (this == &rhs)
        return (*this);
        
    BRDF::operator= (rhs);
    
    kd = rhs.kd;
    cd = rhs.cd;
    
    return (*this);
}


// ---------------------------------------------------------------------- f

RGBColor
Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    // pd可表示为RGB颜色值， 因而可按如下方式表示：
    //   pd = kd * cd / π
    // 其中，kd表示漫反射系数，是一个0 ~ 1的数字。ca为漫反射颜色值。
    // 这使得用户可以快速地修改物体的反射光数据，并为相关的编码过程提供了方便性。
    // 参见公式13.19
    return (kd * cd * invPI);
}



// ---------------------------------------------------------------------- rho

RGBColor
Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
    // 参见公式13.20
    return (kd * cd);
}
