#include "stdafx.h"
#include "Matte.h"

// ---------------------------------------------------------------- default constructor

Matte::Matte (void)
    :    Material(),
        // 环境光反射初始化为完全漫反射Lambertian反射
        ambient_brdf(new Lambertian),
        // 漫反射初始化为完全漫反射Lambertian反射
        diffuse_brdf(new Lambertian)
{}


// ---------------------------------------------------------------- copy constructor

Matte::Matte(const Matte& m)
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
}


// ---------------------------------------------------------------- clone

Material*
Matte::clone(void) const {
    return (new Matte(*this));
}


// ---------------------------------------------------------------- assignment operator

Matte&
Matte::operator= (const Matte& rhs) {

    if (this == &rhs)
        return (*this);

    Material::operator=(rhs);

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (rhs.ambient_brdf) {
        ambient_brdf = rhs.ambient_brdf->clone();
    }
        
    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }

    if (rhs.diffuse_brdf) {
        diffuse_brdf = rhs.diffuse_brdf->clone();
    }

    return (*this);
}


// ---------------------------------------------------------------- destructor

Matte::~Matte(void) {

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
}


// ---------------------------------------------------------------- shade
// explained on page 271
// 计算环境光照，并遍历全部光源以计算直接漫反射光照。
// 从而返回材质的颜色。
// 参见公式14.10
RGBColor
Matte::shade(ShadeRec& sr) {

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
            bool in_shadow = false;
            // 判断是否参与阴影计算
            if (sr.w.lights[j]->casts_shadows()) {
                // 如果参与阴影计算，就构建一条以碰撞点为起点，指向这个光源的光线。
                Ray shadowRay(sr.hit_point, wi);
                // 判断这条光线是否撞到物体。如果是环境光源，则必为真。
                in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
            }
            // 如果光源可以照射到物体。就计算这个光源产生的漫反射光照。
            if (!in_shadow) {
                // 计算公式参见14.6和14.10的第二部分。
                L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
            }
        }
    }
    return (L);
}


// ---------------------------------------------------------------- area_light_shade
// explained on page 332
// 计算区域光照
RGBColor
Matte::area_light_shade(ShadeRec& sr) {

    Vector3D     wo             = -sr.ray.d;
    RGBColor     L             = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int         num_lights    = sr.w.lights.size();

    for (int j = 0; j < num_lights; j++) {
        Vector3D     wi         = sr.w.lights[j]->get_direction(sr);
        float         ndotwi     = sr.normal * wi;
    
        if (ndotwi > 0.0) {
            bool in_shadow = false;

            if (sr.w.lights[j]->casts_shadows()) {
                Ray shadow_ray(sr.hit_point, wi);
                in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr); 
            }

            if (!in_shadow) 
                L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
        }
    }

    return (L);
}


// ---------------------------------------------------------------- get_Le

RGBColor
Matte::get_Le(ShadeRec& sr) const {

    return (black);
}
