#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "BRDF.h"
// 光照完全漫反射
class Lambertian: public BRDF {

    public:

        Lambertian(void);

        Lambertian(const Lambertian& lamb);

        virtual Lambertian*
        clone(void) const;

        ~Lambertian(void);

        Lambertian&
        operator= (const Lambertian& rhs);
        // 返回BRDF比例系数。
        // 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
        // 如果不包含delta()函数，将返回BRDF自身。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

        // 返回双半球反射系数。 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
        // 设置环境光反射系数
        void
        set_ka(const float ka);
        // 设置漫反射系数
        void
        set_kd(const float kd);
        // 设置漫反射颜色值。参数为RGBColor类型。
        void
        set_cd(const RGBColor& c);
        // 设置漫反射颜色值。参数为RGB三个值。
        void
        set_cd(const float r, const float g, const float b);
        // 设置漫反射颜色值。参数为RGB灰度值。
        void
        set_cd(const float c);

    private:

        float        kd;    // 漫反射系数：diffuse reflection coefficient
        RGBColor     cd;    // 漫反射颜色值：diffuse color
};


// -------------------------------------------------------------- set_ka
// 设置环境光反射系数
inline void
Lambertian::set_ka(const float k) {
    kd = k;
}


// -------------------------------------------------------------- set_kd
// 设置漫反射系数
inline void
Lambertian::set_kd(const float k) {
    kd = k;
}


// -------------------------------------------------------------- set_cd
// 设置漫反射颜色值。参数为RGBColor类型。
inline void
Lambertian::set_cd(const RGBColor& c) {
    cd = c;
}


// ---------------------------------------------------------------- set_cd
// 设置漫反射颜色值。参数为RGB三个值。
inline void
Lambertian::set_cd(const float r, const float g, const float b) {
    cd.r = r; cd.g = g; cd.b = b;
}


// ---------------------------------------------------------------- set_cd
// 设置漫反射颜色值。参数为RGB灰度值。
inline void
Lambertian::set_cd(const float c) {
    cd.r = c; cd.g = c; cd.b = c;
}

#endif
