#ifndef __AMBIENTOCCLUDER__
#define __AMBIENTOCCLUDER__

#include "Light.h"
#include "Sampler.h"
/* 环境遮挡光源。 */
class AmbientOccluder: public Light {

    public:
        AmbientOccluder(void);

        AmbientOccluder(const AmbientOccluder& a);

        virtual Light*
        clone(void) const;

        AmbientOccluder&
        operator= (const AmbientOccluder& rhs);

        virtual
        ~AmbientOccluder(void);

        void
        scale_radiance(const float b);
        /* 设置光源颜色值。参数为RGB灰度值。 */
        void
        set_color(const float c);
        /* 设置光源颜色值。参数为RGBColor类型。  */
        void
        set_color(const RGBColor& c);
        /* 设置光源颜色值。参数为RGB三个值。*/
        void
        set_color(const float r, const float g, const float b);

        void
        set_min_amount(int n);
        /* 返回各条阴影光线的方向。*/
        virtual Vector3D
        get_direction(ShadeRec& sr);
        /* 设置采样器对象sp  */
        void
        set_sampler(Sampler* s_ptr);
        /* 返回入射辐射度  */
        virtual RGBColor
        L(ShadeRec& s);
        /* 测试阴影是否被某一个对象遮挡。 */
        bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const;

        virtual bool
        casts_shadows(void) const;
        /* 开启或关闭阴影。  */
        virtual void
        set_shadows(bool _s);

        virtual float
        G(const ShadeRec& sr) const;

        virtual float
        pdf(const ShadeRec& sr) const;

    private:
        /* 光源缩放系数，也就是亮度值。   */
        float        ls;
        // 光源颜色值。
        RGBColor    color;
        /* 阴影减弱因子。  */
        float        min_amount;
        Vector3D    u, v, w;
        /* 采样器指针  */
        Sampler*    sampler_ptr;
};


// ------------------------------------------------------------------------------- scale_radiance

inline void
AmbientOccluder::scale_radiance(const float b) {
    ls = b;
}

// ------------------------------------------------------------------------------- set_color
/*
 * 当光线方向被阻挡时， 可使用数据成员min amount以返回最小的颜色值(非黑色) 。
 * 与返回黑色值相比，增加了着色结果的真实感。
 */
inline void
AmbientOccluder::set_min_amount(int n) {
    min_amount = n;
}

// ------------------------------------------------------------------------------- set_color
/*
 * 设置光源颜色值。参数为RGB灰度值。
 */
inline void
AmbientOccluder::set_color(const float c) {
    color.r = c; color.g = c; color.b = c;
}


// ------------------------------------------------------------------------------- set_color
/*
 * 设置光源颜色值。参数为RGBColor类型。
 */
inline void
AmbientOccluder::set_color(const RGBColor& c) {
    color = c;
}


// ------------------------------------------------------------------------------- set_color
/*
 * 设置光源颜色值。参数为RGB三个值。
 */
inline void
AmbientOccluder::set_color(const float r, const float g, const float b) {
    color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------------- casts_shadows

inline bool
AmbientOccluder::casts_shadows() const {

    return Light::casts_shadows();
}


// ---------------------------------------------------------------------- set_shadows
/*
 * 开启或关闭阴影。
 */
inline void
AmbientOccluder::set_shadows(bool _s) {

    return Light::set_shadows(_s);
}

#endif
