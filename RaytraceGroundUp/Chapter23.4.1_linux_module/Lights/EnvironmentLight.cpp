#include "stdafx.h"
#include "EnvironmentLight.h"

// ---------------------------------------------------------------- default constructor

EnvironmentLight::EnvironmentLight(void)
    :     Light(),
        sampler_ptr(NULL),
        material_ptr(NULL),
        u(), v(), w(),
        wi()
{}    


// ---------------------------------------------------------------- copy constructor

EnvironmentLight::EnvironmentLight(const EnvironmentLight& el)
    :     Light(el),
        u(el.u), v(el.v), w(el.w),
        wi(el.wi)
{
    if(el.sampler_ptr)
        sampler_ptr = el.sampler_ptr->clone(); 
    else  sampler_ptr = NULL;

    if(el.material_ptr)
        material_ptr = el.material_ptr->clone(); 
    else  material_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Light*
EnvironmentLight::clone(void) const {
    return (new EnvironmentLight(*this));
}


// ---------------------------------------------------------------- destructor

EnvironmentLight::~EnvironmentLight(void) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }
}


// --------------------------------------------------------------- assignment operator

EnvironmentLight&
EnvironmentLight::operator= (const EnvironmentLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator=(rhs);

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (rhs.sampler_ptr)
        sampler_ptr = rhs.sampler_ptr->clone();

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }

    if (rhs.material_ptr)
        material_ptr = rhs.material_ptr->clone();

    return (*this);
}


// --------------------------------------------------------------- get_direction

// 返回各条阴影光线的方向。
Vector3D
EnvironmentLight::get_direction(ShadeRec& sr) {
    // 首先构造(u，v，w)。
    // 1. w直接使用法线方向。
    w = sr.normal;
    // 2. u和v位于法线平面。
    //    u等于向上的方向，这里就是Vector3D(0.0072, 1.0, 0.0034)，和w的叉乘。
    //    参见Chapter17.1结尾处中的公式：
    //           v = w × up / ||w × up||
    //    2.1. 计算w × up
    v = Vector3D(0.0034, 1, 0.0071) ^ w;
    //    2.2. 做归一化，除以||w × up||。
    v.normalize();
    // 3. 参见Chapter17.1结尾处中的公式：
    //         u = v × w
    u = v ^ w;
    Point3D sp = sampler_ptr->sample_hemisphere();
    // 该函数只是简单地将局部变量的相关分量投影至(u，v，w)上。
    // 参见公式17.4
    wi = sp.x * u + sp.y * v + sp.z * w;

    return (wi);
}


// ---------------------------------------------------------------------- set_sampler
// Environment Light类和Ambient Occluder类具有某些共同特征。
// 首先，二者具有相同的set_sampler()函数，并将采样点按照余弦分布的方式映射至半球模型上。
void
EnvironmentLight::set_sampler(Sampler* s_ptr) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // 设置采样器对象sp(在build() 函数中加以构造)，
    sampler_ptr = s_ptr;
    // 随后以余弦分布(e=1)的方式将采样映射至半球模型之上。
    sampler_ptr->map_samples_to_hemisphere(1);
}


// --------------------------------------------------------------- L

RGBColor
EnvironmentLight::L(ShadeRec& sr) {
    return (material_ptr->get_Le(sr));
}


// ---------------------------------------------------------------------- in_shadow
// not sure if this is correct, copied it from poitn light...
bool
EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    return false; // can an environment light even be in shadow?
}


// ---------------------------------------------------------------- pdf
// The following function is not in the book.
// It uses Equation 18.6
float
EnvironmentLight::pdf(const ShadeRec& sr) const {

    return (sr.normal * wi * invPI);
}
