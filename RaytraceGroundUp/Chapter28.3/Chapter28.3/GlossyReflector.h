#ifndef GLOSSYREFLECTOR_H
#define GLOSSYREFLECTOR_H

//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "Phong.h"
#include "GlossySpecular.h"
// 光泽反射将统一处理镜面高光和镜面反射计算，进而可以消除二者的不一致性。
class GlossyReflector: public Phong {
    public:

        GlossyReflector(void);

        GlossyReflector(const GlossyReflector& gr);

        GlossyReflector&
        operator= (const GlossyReflector& rhs);

        virtual GlossyReflector*
        clone(void) const;

        ~GlossyReflector(void);
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
        // 设置Phong指数
        void
        set_exponent(const float e);
        // 设置Phong指数，并且设置采样器。
        void
        set_samples(const int num_samples, const float exp);

        virtual RGBColor
        shade(ShadeRec& s);

        virtual RGBColor
        area_light_shade(ShadeRec& sr);

    private:
        // 封装了一个光照镜面反射
        GlossySpecular* glossy_specular_brdf;
};


// ---------------------------------------------------------------- set_kr

inline void
GlossyReflector::set_kr(const float k) {
    glossy_specular_brdf->set_ks(k);
}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const RGBColor& c) {
    glossy_specular_brdf->set_cs(c);

}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const float r, const float g, const float b) {
    glossy_specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cr

inline void
GlossyReflector::set_cr(const float c) {
    // 直接调用光照镜面反射的set_cs函数。
    glossy_specular_brdf->set_cs(c);
}

// -------------------------------------------------------------- set_exp

inline void
GlossyReflector::set_exponent(const float e) {
    // 直接调用光照镜面反射的set_exp函数。
    glossy_specular_brdf->set_exp(e);
}

// -------------------------------------------------------------- set_samples

inline void
GlossyReflector::set_samples(const int num_samples, const float exp) {
    // 直接调用光照镜面反射的set_samples函数。
    glossy_specular_brdf->set_samples(num_samples, exp);
}

#endif // GLOSSYREFLECTOR_H
