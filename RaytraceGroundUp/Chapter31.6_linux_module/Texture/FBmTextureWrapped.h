#ifndef FBMTEXTUREWRAPPED_H
#define FBMTEXTUREWRAPPED_H

#include "Texture.h"
#include "LatticeNoise.h"
// 对FBm Texture：：get_color() 函数稍加修改即可生成另一组迥然不同的纹理图像， 包括两方面内容。首先， 可将噪声函数的振幅乘以一个大于1的值以扩大其数值范围； 随后， 可通过floor() 函数将结果值截取至[0，1]范围内，该方法等同于程序清单31.3和程序清单31.6中计算fx、fy、fz时所使用的方法。这使得噪声值多次往复于[0，1]范围内，且当噪声值从1降至0时，纹理中将生成山脊状图案。程序清单31.14显示了纹理类Wrapped FBm Texture中的get_color() 函数的代码， 其中， 若expansion number 值足够大， 则噪声值将位于[0， 1] 范围内且无需使用minvalue和maxvalue继续缩放操作。

class FBmTextureWrapped : public Texture
{
    public:

        FBmTextureWrapped(void);

        FBmTextureWrapped(LatticeNoise* ln_ptr);

        FBmTextureWrapped(const FBmTextureWrapped& ft);

        virtual FBmTextureWrapped*
        clone(void) const;

        virtual
        ~FBmTextureWrapped(void);

        void
        set_noise(LatticeNoise* ln_ptr);

        void
        set_color(const RGBColor& c);

        void
        set_color(const float r, const float g, const float b);

        void
        set_color(const float c);

        void
        set_min_value(const float mi);

        void
        set_max_value(const float ma);

        void
        set_expansion_number(const float en);

        virtual RGBColor
        get_color(const ShadeRec& sr) const;

    protected:

        FBmTextureWrapped&
        operator= (const FBmTextureWrapped& rhs);

    private:

        LatticeNoise*        noise_ptr;        RGBColor               color;        float                  min_value, max_value;    // scaling factors
        float               expansion_number;       // expand the amplitude of the noise function};


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_noise(LatticeNoise* ln_ptr) {
    noise_ptr = ln_ptr;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const RGBColor& c) {
    color = c;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const float r, const float g, const float b) {
    color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTextureWrapped::set_color(const float c) {
    color.r = c; color.g = c; color.b = c;
}


// ---------------------------------------------------------------- set_min_value

inline void
FBmTextureWrapped::set_min_value(const float mi) {
    min_value = mi;
}


// ---------------------------------------------------------------- set_max_value

inline void
FBmTextureWrapped::set_max_value(const float ma) {
    max_value = ma;
}


// ---------------------------------------------------------------- set_expansion_number

inline void
FBmTextureWrapped::set_expansion_number(const float en) {
    expansion_number = en;
}
#endif // FBMTEXTUREWRAPPED_H
