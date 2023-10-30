#ifndef FRESNELREFLECTOR_H
#define FRESNELREFLECTOR_H


//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


// this implements perfect specular reflection for indirect illumination
// with reflective materials

#include "BRDF.h"
#include "Normal.h"
// 定义一个BRDF用于处理反射计算
class FresnelReflector: public BRDF
{
    public:

        FresnelReflector(void);

        ~FresnelReflector(void);

        virtual FresnelReflector*
        clone(void) const;
        // 设置反射系数
        void
        set_kr(const float k);

        void
        set_eta_in(const float ei);

        void
        set_eta_out(const float eo);

        void
        set_cr(const RGBColor& c);
        // 设置反射颜色值
        void
        set_cr(const float r, const float g, const float b);
        // 设置反射颜色值
        void
        set_cr(const float c);
        // 返回BRDF比例系数。
        // 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
        // 如果不包含delta()函数，将返回BRDF自身。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 用于计算反射光线的方向，并且返回计算出来的颜色值。
        // 参数wi；用于返回最终的方向，因而不可为常量。
        // 顾名思义，相关方向将通过对BRDF采样计算得到。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const;
        // 返回双半球反射系数。 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
        // 求解Fresnel方程
        virtual float
        fresnel(const ShadeRec& sr) const;
    private:

        float        kr;            // 反射系数：reflection coefficient
        RGBColor     cr;            // 反射颜色值：the reflection colour
        float        eta_in;        // 入射介质折射率
        float        eta_out;       // 折射介质折射率
};


// -------------------------------------------------------------- set_kr

inline void
FresnelReflector::set_kr(const float k) {
    kr = k;
}


// -------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const RGBColor& c) {
    cr = c;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float r, const float g, const float b) {
    cr.r = r; cr.g = g; cr.b = b;
}


// ---------------------------------------------------------------- set_cr

inline void
FresnelReflector::set_cr(const float c) {
    cr.r = c; cr.g = c; cr.b = c;
}


// -------------------------------------------------------------- set_eta_in

inline void
FresnelReflector::set_eta_in(const float ei) {
    eta_in = ei;
}


// -------------------------------------------------------------- set_eta_out

inline void
FresnelReflector::set_eta_out(const float eo) {
    eta_out = eo;
}
#endif // FRESNELREFLECTOR_H
