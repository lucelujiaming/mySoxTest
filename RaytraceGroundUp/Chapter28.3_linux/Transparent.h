#ifndef TRANSPARENT_H
#define TRANSPARENT_H

#include "Phong.h"
#include "PerfectSpecular.h"
#include "PerfectTransmitter.h"

// 材质类Transparent继承于Phong类， 因而可采用镜面高光渲染透明对象。
// 当使用有向点光源时，将有助于提高透明对象的视觉效果，
// 但该过程仍然会产生反射材质所出现的不一致性。

// 鉴于材质类Transparent实现了完全镜面反射和透射，
// 且在后续小节中将仅使用有向点光源，因而可采用Whitted跟踪器。
class Transparent : public Phong
{
    public:

        Transparent(void);

        Transparent(const Transparent& tm);

        Transparent&
        operator= (const Transparent& rhs);

        virtual Transparent*
        clone(void) const;

        ~Transparent(void);

        void
        set_kr(const float k);

        void
        set_cr(const RGBColor& c);

        void
        set_cr(const float r, const float g, const float b);

        void
        set_cr(const float c);

        void
        set_kt(const float k);

        void
        set_ior(const float i);

        virtual RGBColor
        shade(ShadeRec& s);

    private:
        // 包含了一个BRDF用于处理反射计算，
        PerfectSpecular* reflective_brdf;
        // 包含了一个BTDF用于处理透明度计算
        PerfectTransmitter* specular_btdf;
        // 以及3个继承于Phong材质类的BRDF。
};

// ---------------------------------------------------------------- set_kr

inline void
Transparent::set_kr(const float k) {
    reflective_brdf->set_kr(k);
}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const RGBColor& c) {
    reflective_brdf->set_cr(c);

}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const float r, const float g, const float b) {
    reflective_brdf->set_cr(r, g, b);
}


// ---------------------------------------------------------------- set_cr

inline void
Transparent::set_cr(const float c) {
    reflective_brdf->set_cr(c);
}

// ---------------------------------------------------------------- set_kt

inline void
Transparent::set_kt(const float k) {
    specular_btdf->set_kt(k);
}

// ---------------------------------------------------------------- set_ior

inline void
Transparent::set_ior(const float i) {
    specular_btdf->set_ior(i);
}

#endif
