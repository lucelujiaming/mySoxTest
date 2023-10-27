#ifndef __GLOSSYSPECULAR__
#define __GLOSSYSPECULAR__

#include "BRDF.h"
#include "Sampler.h"
// 光照镜面反射
class GlossySpecular: public BRDF {

    public:

        GlossySpecular(void);

        GlossySpecular(const GlossySpecular& gloss);

        virtual GlossySpecular*
        clone(void) const;

        ~GlossySpecular(void);

        GlossySpecular&
        operator= (const GlossySpecular& rhs);
        // 针对于反射材质模拟计算，返回计算出来的颜色值。 
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 该函数用于模拟光泽反射。参见Chapter25.1。
        // 用于计算反射光线的方向，并且返回计算出来的颜色值。
        // 使用Monte Calo积分实现。
        // 参数wi；用于返回最终的方向，因而不可为常量。
        // 顾名思义，相关方向将通过对BRDF采样计算得到。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
        // 返回双半球反射系数。 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
        // 设置镜面反射系数
        void
        set_ks(const float ks);
        // 设置Phong指数
        void
        set_exp(const float _e);
        // 设置镜面高光颜色值。参数为RGBColor类型。
        void
        set_cs(const RGBColor& c);
        // 设置镜面高光颜色值。参数为RGB三个值。
        void
        set_cs(const float r, const float g, const float b);
        // 设置镜面高光颜色值。参数为RGB灰度值。
        void
        set_cs(const float c);
        // 设置采样器和Phong指数。
        void
        set_sampler(Sampler* sp, const float exp);                // any type of sampling
        // 设置Phong指数，并且设置采样器为多重抖动采样。
        void
        set_samples(const int num_samples, const float exp);    // multi jittered sampling
        // 设置法线方向。未实现。
        void
        set_normal(const Normal& n);

    private:
        // 镜面反射系数
        float        ks;                // specular reflection coefficient
        // Phong指数(P102)。
        // 因为半球体采样点的表面密度值d与极角θ之间的关系为：
        //     d = cos(θ)^exp
        // 这个exp用于控制极角θ变大时，d值的下降速度。
        // exp的值越大，下降的速度也越快。
        // 这有助于用户方便地控制镜面高光的尺寸。
        float        exp;               // specular exponent
        // 镜面高光颜色值
        RGBColor     cs;                // specular color
        // 高光采用半球采样。 
		// 虽然在父类BRDF中已经定义了这个成员变量。但是如果在子类中不定义这个成员变量。
		// 就无法编译通过。让人十分不解。
        Sampler*    sampler_ptr;    // for use in sample_f
};


// -------------------------------------------------------------- set_ks
// 设置镜面反射系数
inline void
GlossySpecular::set_ks(const float k) {
    ks = k;
}

// -------------------------------------------------------------- set_exp
// 设置Phong指数
inline void
GlossySpecular::set_exp(const float e) {
    exp= e;
}


// -------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGBColor类型。
inline void
GlossySpecular::set_cs(const RGBColor& c) {
    cs = c;
}


// ---------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGB三个值。
inline void
GlossySpecular::set_cs(const float r, const float g, const float b) {
    cs.r = r; cs.g = g; cs.b = b;
}


// ---------------------------------------------------------------- set_cs
// 设置镜面高光颜色值。参数为RGB灰度值。
inline void
GlossySpecular::set_cs(const float c) {
    cs.r = c; cs.g = c; cs.b = c;
}

#endif
