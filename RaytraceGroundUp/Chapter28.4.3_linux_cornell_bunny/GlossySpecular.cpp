#include "stdafx.h"
#include "GlossySpecular.h"
#include "MultiJittered.h"
#include "Constants.h"
#include "Maths.h"

// ---------------------------------------------------------------------- default constructor

GlossySpecular::GlossySpecular(void)
    :     ks(0.0),
        cs(1.0),
        sampler_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

GlossySpecular::GlossySpecular(const GlossySpecular& gloss)
    :    BRDF(gloss),
        ks(gloss.ks),
        cs(gloss.cs),
        exp(gloss.exp)
{
    // need to do a deep copy of the sampler
    if(gloss.sampler_ptr != NULL) {
        sampler_ptr = gloss.sampler_ptr->clone();
    }
}


// ---------------------------------------------------------------------- clone

GlossySpecular*
GlossySpecular::clone(void) const {
    return (new GlossySpecular(*this));
}


// ---------------------------------------------------------------------- destructor

GlossySpecular::~GlossySpecular(void) {}


// ---------------------------------------------------------------------- assignment operator

GlossySpecular&
GlossySpecular::operator= (const GlossySpecular& rhs) {

    if (this == &rhs)
        return (*this);
        
    BRDF::operator= (rhs);
    
    ks = rhs.ks;
    cs = rhs.cs;
    exp = rhs.exp;

    // need to do a deep copy of the sampler
    if(rhs.sampler_ptr != NULL) {
        sampler_ptr = rhs.sampler_ptr->clone();
    }
    
    return (*this);
}

// ---------------------------------------------------------------------- set_sampler
// this allows any type of sampling to be specified in the build functions
// 设置采样器和Phong指数。
void
GlossySpecular::set_sampler(Sampler* sp, const float exp) {

    sampler_ptr = sp;
    sampler_ptr->map_samples_to_hemisphere(exp);
}


// ---------------------------------------------------------------------- set_samples
// this sets up multi-jittered sampling using the number of samples
// 设置Phong指数，并且设置采样器为多重抖动采样并被实现为光泽反射的相关材质加以调用。
void
GlossySpecular::set_samples(const int num_samples, const float exp) {

    sampler_ptr = new MultiJittered(num_samples);
    // 使用余弦指数分布映射到半球模型上。
    sampler_ptr->map_samples_to_hemisphere(exp);
}


// ----------------------------------------------------------------------------------- f
// no sampling here: just use the Phong formula
// this is used for direct illumination only
// explained on page 284
// 针对于反射材质模拟计算，返回计算出来的颜色值。 
RGBColor
GlossySpecular::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {

    RGBColor    L;
    float        ndotwi = sr.normal * wi;
    // 使用公式15.4通过sr和wi计算出相应的法线，计算出反射方向r。
    Vector3D     r(-wi + 2.0 * sr.normal * ndotwi); // mirror reflection direction
    // 使用wo计算r * wo。
    float        rdotwo = r * wo;

    if (rdotwo > 0.0)
    {
        // 使用公式15.7计算出L。
        L = ks * cs * pow(rdotwo, exp);
    }

    return (L);
}


// ----------------------------------------------------------------------------------- sample_f
// this is used for indirect illumination
// 该函数用于模拟光泽反射。参见Chapter25.1。
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
// 使用Monte Calo积分实现。
RGBColor
GlossySpecular::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {

    float ndotwo = sr.normal * wo;
    // 使用公式15.4通过sr和wo计算出相应的法线，计算出反射方向r。
    Vector3D r = -wo + 2.0 * sr.normal * ndotwo;    // direction of mirror reflection
    // w平行于r
    Vector3D w = r;
    // u和v位于法线平面
    Vector3D u = Vector3D(0.00424, 1, 0.00764) ^ w;
    u.normalize();
    Vector3D v = u ^ w;
    // 获得半球模型
    // 返回存储于采样器对象中的下一个采样点，映射到半球体。
    // 因为，通常情况下，我们需要在局部坐标系中计算光线的方向，
    // 并在随后计算该光线在世界坐标系中的方向。
    Point3D sp = sampler_ptr->sample_hemisphere();
    // 参见公式25.8
    wi = sp.x * u + sp.y * v + sp.z * w;            // reflected ray direction
    // 如果位于切平面下方。
    if (sr.normal * wi < 0.0)                         // reflected ray is below tangent plane
        wi = -sp.x * u - sp.y * v + sp.z * w;
    // 参见公式25.5。
    float phong_lobe = pow((float)(r * wi), (float)exp);
    // 添加公式25.3中的cos项。
    pdf = phong_lobe * (sr.normal * wi);
    // 返回反射辐射度。参见公式25.3。
    return (ks * cs * phong_lobe);
}

// ---------------------------------------------------------------------- rho
// 返回双半球反射系数。
RGBColor
GlossySpecular::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
