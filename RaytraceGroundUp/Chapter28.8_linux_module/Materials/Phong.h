#ifndef __PHONG__
#define __PHONG__

#include "Material.h"
#include "Lambertian.h"
#include "GlossySpecular.h"
#include "Phong.h"

//----------------------------------------------------------------------------- class Phong
// Phong材质类
class Phong: public Material {

    public:

        Phong(void);

        Phong(const Phong& m);

        virtual Material*
        clone(void) const;

        Phong& 
        operator= (const Phong& rhs);

        ~Phong(void);
        // 设置环境光反射系数
        void
        set_ka(const float k);
        // 设置漫反射系数
        void
        set_kd(const float k);
        // 设置镜面反射系数
        void
        set_ks(const float ks);
        // 设置材质颜色值。参数为RGBColor类型。
        void
        set_cd(const RGBColor c);
        // 设置材质颜色值。参数为RGB三个值。
        void
        set_cd(const float r, const float g, const float b);
        // 设置材质颜色值。参数为RGB灰度值。
        void
        set_cd(const float c);
        // 设置镜面高光颜色值。参数为RGBColor类型。
        void
        set_cs(const RGBColor c);
        // 设置镜面高光颜色值。参数为RGB三个值。
        void
        set_cs(const float r, const float g, const float b);
        // 设置镜面高光颜色值。参数为RGB灰度值。
        void
        set_cs(const float c);
        // 设置Phong指数
        void
        set_exp(const float _e);
        // 计算环境光照，并遍历全部光源以计算直接漫反射光照和镜面高光反射光照。
        // 从而返回材质的颜色。
        virtual RGBColor
        shade(ShadeRec& sr);

        virtual RGBColor
        get_Le(ShadeRec& sr) const;

    private:
        Lambertian*        ambient_brdf;    // 环境光反射
        Lambertian*        diffuse_brdf;    // 漫反射
        GlossySpecular*    specular_brdf;   // 镜面高光反射
};


// ---------------------------------------------------------------- set_ka
// this sets Phong::kd
// there is no Phong::ka data member because ambient reflection
// is diffuse reflection
// 设置环境光反射系数
inline void
Phong::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Phong::kd, but for a different Phong object
// 设置漫反射系数
inline void
Phong::set_kd(const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_ks
// 设置镜面反射系数
inline void
Phong::set_ks(const float ks) {
    specular_brdf->set_ks(ks);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGBColor类型。
inline void
Phong::set_cd(const RGBColor c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGB三个值。
inline void
Phong::set_cd(const float r, const float g, const float b) {
    ambient_brdf->set_cd(r, g, b);
    diffuse_brdf->set_cd(r, g, b);
    specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cd
// 设置材质颜色值。参数为RGB灰度值。
inline void
Phong::set_cd(const float c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGBColor类型。
inline void
Phong::set_cs(const RGBColor c) {
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGB三个值。
inline void
Phong::set_cs(const float r, const float g, const float b) {
    specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGB灰度值。
inline void
Phong::set_cs(const float c) {
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_exp
// 设置Phong指数
inline void
Phong::set_exp(const float _e) {
    specular_brdf->set_exp(_e);
}

#endif
