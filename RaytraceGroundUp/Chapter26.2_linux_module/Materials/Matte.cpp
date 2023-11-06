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


// ---------------------------------------------------------------------- set_sampler

void
Matte::set_sampler(Sampler* s_ptr) {
    diffuse_brdf->set_sampler(s_ptr);
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
                // 判断这条光线是否位于阴影中。如果是环境光源，则必为真。
                in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
            }
            // 如果光线位于阴影中，就计算这个光源产生的漫反射光照。
            if (!in_shadow) 
            {
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
    // 计算环境光照。参见公式14.10
    // 计算方法为由公式13.20给出的双半球反射系数(ka * cd)乘以
    // 由公式14.1给出的入射环境光辐射度(ls * cl)
    RGBColor     L             = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int         num_lights    = sr.w.lights.size();
    // 遍历全部光源以计算直接漫反射光照。
    for (int j = 0; j < num_lights; j++) {
        Vector3D     wi         = sr.w.lights[j]->get_direction(sr);
        // 判断是否可见。
        float         ndotwi     = sr.normal * wi;
        // 如果可见，
        if (ndotwi > 0.0) {
            bool in_shadow = false;
            // 判断是否参与阴影计算
            if (sr.w.lights[j]->casts_shadows()) {
                // 如果参与阴影计算，就构建一条以碰撞点为起点，指向这个光源的光线。
                Ray shadow_ray(sr.hit_point, wi);
                // 判断这条光线是否位于阴影中。
                in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr); 
            }
            // 如果光线位于阴影中，就计算这个光源产生的区域光照。
            // 计算基于式(18.3)
            if (!in_shadow)
            {
                // L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
                RGBColor diffColor = diffuse_brdf->f(sr, wo, wi);
                RGBColor radianceColor = sr.w.lights[j]->L(sr);
                float    gFactor = sr.w.lights[j]->G(sr);
                float    pdf = sr.w.lights[j]->pdf(sr);
                RGBColor calcResult = diffColor * radianceColor * gFactor * gFactor / pdf;
                L += calcResult;
            }
        }
    }

    return (L);
}


// ---------------------------------------------------------------- path_shade
// 需要注意的是，该函数不显式地计算直接光照。
RGBColor
Matte::path_shade(ShadeRec& sr) {
    Vector3D     wo = -sr.ray.d;
    Vector3D     wi;
    float         pdf;
    RGBColor     f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
    float         ndotwi = sr.normal * wi;
    // 创建折射光线。
    Ray         reflected_ray(sr.hit_point, wi);
    // 使用折射光线递归调用。
    return (f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * ndotwi / pdf);
}

// ---------------------------------------------------------------- global_shade

RGBColor
Matte::global_shade(ShadeRec& sr) {
    RGBColor L;
    // 当depth值为0时，计算直接光照。
    if (sr.depth == 0) {
        L = area_light_shade(sr);
    }

    Vector3D     wo = -sr.ray.d;
    Vector3D     wi;
    float         pdf;
    RGBColor     f = diffuse_brdf->sample_f(sr, wo, wi, pdf);
    float         ndotwi = sr.normal * wi;
    // 创建折射光线。
    Ray         reflected_ray(sr.hit_point, wi);
    // 使用折射光线递归调用。
    L += f * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * ndotwi / pdf;

    return (L);
}


// ---------------------------------------------------------------- get_Le

RGBColor
Matte::get_Le(ShadeRec& sr) const {
    return (black);
}
