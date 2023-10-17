#include "Phong.h"

// ---------------------------------------------------------------- default constructor

Phong::Phong (void)
    :    Material(),
        // 环境光反射初始化为完全漫反射Lambertian反射
        ambient_brdf(new Lambertian),
        // 漫反射初始化为完全漫反射Lambertian反射
        diffuse_brdf(new Lambertian),
        specular_brdf(new GlossySpecular)
{}


// ---------------------------------------------------------------- copy constructor

Phong::Phong(const Phong& m)
    :     Material(m)
{
    if(m.ambient_brdf)
    {
        ambient_brdf = m.ambient_brdf->clone();
    }
    else 
    {
        ambient_brdf = NULL;
    }

    if(m.diffuse_brdf)
    {
        diffuse_brdf = m.diffuse_brdf->clone();
    }
    else  
    {
        diffuse_brdf = NULL;
    }

    if(m.specular_brdf)
    {
        specular_brdf = m.specular_brdf->clone();
    }
    else 
    {
        specular_brdf = NULL;
    }
}


// ---------------------------------------------------------------- clone

Material*
Phong::clone(void) const {
    return (new Phong(*this));
}


// ---------------------------------------------------------------- assignment operator

Phong&
Phong::operator= (const Phong& rhs) {

    if (this == &rhs)
        return (*this);

    Material::operator=(rhs);

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (rhs.ambient_brdf)
    {
        ambient_brdf = rhs.ambient_brdf->clone();
    }
        
    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }

    if (rhs.diffuse_brdf) {
        diffuse_brdf = rhs.diffuse_brdf->clone();
    }
    if (specular_brdf) {
        delete specular_brdf;
        specular_brdf = NULL;
    }

    if (rhs.specular_brdf) {
        specular_brdf = rhs.specular_brdf->clone();
    }
    return (*this);
}


// ---------------------------------------------------------------- destructor

Phong::~Phong(void) {

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }

    if (specular_brdf) {
        delete specular_brdf;
        specular_brdf = NULL;
    }
}


// ---------------------------------------------------------------- shade
// explained on page 285
// 计算环境光照，并遍历全部光源以计算直接漫反射光照和镜面高光反射光照。
// 从而返回材质的颜色。
// 参见公式15.8
RGBColor
Phong::shade(ShadeRec& sr) {

    Vector3D     wo           = -sr.ray.d;
    // 计算环境光照。参见公式14.10
    // 计算方法为由公式13.20给出的双半球反射系数(ka * cd)乘以
    // 由公式14.1给出的入射环境光辐射度(ls * cl)
    RGBColor     L            = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int         num_lights    = sr.w.lights.size();
    // 遍历全部光源以计算直接漫反射光照。
    for (int j = 0; j < num_lights; j++) {
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        // 判断是否可见。
        float ndotwi = sr.normal * wi;
        // 如果可见，
        if (ndotwi > 0.0) {
            // 计算公式参见15.7和15.8的第二部分。
            L += (    diffuse_brdf->f(sr, wo, wi)    // 漫反射光照
                    + specular_brdf->f(sr, wo, wi))  // 镜面高光反射光照
                        * sr.w.lights[j]->L(sr) * ndotwi;
        }
    }

    return (L);
}


// ---------------------------------------------------------------- get_Le

RGBColor
Phong::get_Le(ShadeRec& sr) const{
    return (black);
}
