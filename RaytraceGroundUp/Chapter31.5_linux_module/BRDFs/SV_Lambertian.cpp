#include "SV_Lambertian.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

SV_Lambertian::SV_Lambertian(void)
    :   BRDF(),
        kd(0.0),
        // 默认为纯色纹理
        cd(new ConstantColor)
{}


// ---------------------------------------------------------------------- copy constructor

SV_Lambertian::SV_Lambertian(const SV_Lambertian& lamb)
    :   BRDF(lamb),
        kd(lamb.kd),
        cd(lamb.cd)
{}


// ---------------------------------------------------------------------- clone

SV_Lambertian*
SV_Lambertian::clone(void) const {
    return (new SV_Lambertian(*this));
}


// ---------------------------------------------------------------------- destructor

SV_Lambertian::~SV_Lambertian(void) {}


// ---------------------------------------------------------------------- assignment operator

SV_Lambertian&
SV_Lambertian::operator= (const SV_Lambertian& rhs) {
    if (this == &rhs)
        return (*this);

    BRDF::operator= (rhs);

    kd = rhs.kd;
    cd = rhs.cd;

    return (*this);
}


// ---------------------------------------------------------------------- set_sampler

void
SV_Lambertian::set_sampler(Sampler* s_ptr) {
    sampler_ptr = s_ptr;
    sampler_ptr->map_samples_to_hemisphere(1);
}

// ---------------------------------------------------------------------- f

RGBColor
SV_Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    // pd可表示为RGB颜色值， 因而可按如下方式表示：
    //   pd = kd * cd / π
    // 其中，kd表示漫反射系数，是一个0 ~ 1的数字。ca为漫反射颜色值。
    // 这使得用户可以快速地修改物体的反射光数据，并为相关的编码过程提供了方便性。
    // 参见公式13.19
    // 漫反射颜色值通过纹理指针返回
    return (kd * (cd->get_color(sr)) * invPI);
}



// ---------------------------------------------------------------------- sample_f

// this generates a direction by sampling the hemisphere with a cosine distribution
// this is called in path_shade for any material with a diffuse shading component
// the samples have to be stored with a cosine distribution

RGBColor
SV_Lambertian::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {

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
    // 漫反射颜色值通过纹理指针返回
    return (kd * (cd->get_color(sr)) * invPI);
}


// ---------------------------------------------------------------------- rho

RGBColor
SV_Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
    // 参见公式13.20
    // 漫反射颜色值通过纹理指针返回
    return (kd * (cd->get_color(sr)));
}

