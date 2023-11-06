#ifndef SV_MATTE_H
#define SV_MATTE_H

#include "Material.h"
#include "SV_Lambertian.h"

//----------------------------------------------------------------------------- class SV_Matte
// Matte材质。对于环境光反射和漫反射都采用完全漫反射Lambertian反射
// 将纹理整合至基于空间变化的BRDF的相关材质中，其属性与位置相关，
// 因而可通过存储纹理而非RGB颜色值加以实现。
// 这里，BRDF和材质的名称以字母SV_开始。
class SV_Matte: public Material {
    public:

        SV_Matte(void);

        SV_Matte(const SV_Matte& m);

        virtual Material*
        clone(void) const;

        SV_Matte&
        operator= (const SV_Matte& rhs);

        ~SV_Matte(void);
        // 设置环境光反射系数
        void
        set_ka(const float k);
        // 设置漫反射系数
        void
        set_kd(const float k);

        void
        set_cd(Texture* t_ptr);
        // 设置材质颜色值。
        // 通过纹理指针存储了颜色值

        void
        set_sampler(Sampler* sp);               // any type of sampling

        virtual RGBColor
        shade(ShadeRec& sr);
        // 计算区域光照
        virtual RGBColor
        area_light_shade(ShadeRec& sr);

        virtual RGBColor
        path_shade(ShadeRec& sr);

        virtual RGBColor
        global_shade(ShadeRec& sr);
    private:

        SV_Lambertian*        ambient_brdf;    // 环境光反射
        SV_Lambertian*        diffuse_brdf;    // 漫反射
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection
// 设置环境光反射系数
inline void
SV_Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object
// 设置漫反射系数
inline void
SV_Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd

// 设置纹理，通过纹理指针存储了颜色值。
inline void
SV_Matte::set_cd(Texture* t_ptr) {
    // 环境和漫反射BRDF使用相同的纹理。
    ambient_brdf->set_cd(t_ptr);
    diffuse_brdf->set_cd(t_ptr);
}

#endif // SV_MATTE_H
