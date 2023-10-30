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


// ---------------------------------------------------------------------- set_sampler

void
Lambertian::set_sampler(Sampler* s_ptr) {
	sampler_ptr = s_ptr;
	sampler_ptr->map_samples_to_hemisphere(1);
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



// ---------------------------------------------------------------------- sample_f

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in path_shade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution
RGBColor
Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {

    // 构造正交基向量且w=n，
	Vector3D w = sr.normal;
	Vector3D v = Vector3D(0.0034, 1, 0.0071) ^ w;
	v.normalize();
	Vector3D u = v ^ w;

    // 并于随后对呈余弦分布且法线居中的半球模型进行采样。
    // 这里，需要在Lambertian类型BRDF Matte::diffuse_brdf中定义一个(继承)采样器对象，
    // 且采样点呈余弦分布。
	Point3D sp = sampler_ptr->sample_hemisphere();
	wi = sp.x * u + sp.y * v + sp.z * w;
	wi.normalize();
    // 根据式(18.6)计算pdf=cosθi/π
	pdf = sr.normal * wi * invPI;
    // 返回kd * cd/π。
	return (kd * cd * invPI);
}



// ---------------------------------------------------------------------- rho

RGBColor
Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
    // 参见公式13.20
    return (kd * cd);
}
