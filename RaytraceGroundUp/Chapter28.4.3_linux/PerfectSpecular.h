#ifndef __PERFECT_SPECULAR__
#define __PERFECT_SPECULAR__

// this implements perfect specular reflection for indirect illumination
// with reflective materials 

#include "BRDF.h"
#include "Normal.h"
#include "Constants.h"
// 完全镜面BRDF。
class PerfectSpecular: public BRDF {

    public:

        PerfectSpecular(void);

        ~PerfectSpecular(void);

        virtual PerfectSpecular*
        clone(void) const;
        // 设置反射系数
        void
        set_kr(const float k);
        // 设置反射颜色值
        void
        set_cr(const RGBColor& c);
        // 设置反射颜色值
        void
        set_cr(const float r, const float g, const float b);
        // 设置反射颜色值
        void
        set_cr(const float c);
        // 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 用于计算反射光线的方向，并且返回计算出来的颜色值。
        // 参数wi；用于返回最终的方向，因而不可为常量。
        // 顾名思义，相关方向将通过对BRDF采样计算得到。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
        // 用于计算反射光线的方向，并且返回计算出来的颜色值。
        // 使用Monte Calo积分实现。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;

    private:
        float        kr;            // 反射系数：reflection coefficient
        RGBColor     cr;            // 反射颜色值：the reflection colour
};


// -------------------------------------------------------------- set_kr

inline void
PerfectSpecular::set_kr(const float k) {
    kr = k;
}


// -------------------------------------------------------------- set_cr

inline void
PerfectSpecular::set_cr(const RGBColor& c) {
    cr = c;
}


// ---------------------------------------------------------------- set_cr

inline void
PerfectSpecular::set_cr(const float r, const float g, const float b) {
    cr.r = r; cr.g = g; cr.b = b;
}


// ---------------------------------------------------------------- set_cr

inline void
PerfectSpecular::set_cr(const float c) {
    cr.r = c; cr.g = c; cr.b = c;
}

#endif
