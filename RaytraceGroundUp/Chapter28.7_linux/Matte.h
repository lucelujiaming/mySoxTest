#ifndef __MATTE__
#define __MATTE__

#include "Material.h"
#include "Lambertian.h"

//----------------------------------------------------------------------------- class Matte
// Matte材质。对于环境光反射和漫反射都采用完全漫反射Lambertian反射
class Matte: public Material {

    public:

        Matte(void);

        Matte(const Matte& m);

        virtual Material*
        clone(void) const;

        Matte& 
        operator= (const Matte& rhs);

        ~Matte(void);
        // 设置环境光反射系数
        void
        set_ka(const float k);
        // 设置漫反射系数
        void
        set_kd(const float k);
        // 设置材质颜色值。参数为RGBColor类型。
        void
        set_cd(const RGBColor c);
        // 设置材质颜色值。参数为RGB三个值。
        void
        set_cd(const float r, const float g, const float b);
        // 设置材质颜色值。参数为RGB灰度值。
        void
        set_cd(const float c);
        // 计算环境光照，并遍历全部光源以计算直接漫反射光照。
        // 从而返回材质的颜色。
        void
        set_sampler(Sampler* sp);               // any type of sampling
        virtual RGBColor
        shade(ShadeRec& sr);
        // 计算区域光照
        virtual RGBColor
        area_light_shade(ShadeRec& sr);

        virtual RGBColor
        get_Le(ShadeRec& sr) const;

        virtual RGBColor
        path_shade(ShadeRec& sr);

        virtual RGBColor
        global_shade(ShadeRec& sr);
    private:
        Lambertian*        ambient_brdf;    // 环境光反射
        Lambertian*        diffuse_brdf;    // 漫反射
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection
// 设置环境光反射系数
inline void
Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object
// 设置漫反射系数
inline void
Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGBColor类型。
inline void
Matte::set_cd(const RGBColor c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGB三个值。
inline void
Matte::set_cd(const float r, const float g, const float b) {
    ambient_brdf->set_cd(r, g, b);
    diffuse_brdf->set_cd(r, g, b);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGB灰度值。
inline void
Matte::set_cd(const float c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}

#endif
