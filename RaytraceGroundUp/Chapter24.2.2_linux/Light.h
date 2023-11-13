#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"
#include "Ray.h"

class ShadeRec;

//-------------------------------------------------------------------- class Light

class Light {

    public:

        Light(void);

        Light(const Light& ls);

        Light&
        operator= (const Light& rhs);

        virtual Light*
        clone(void) const = 0;

        virtual
        ~Light(void);
        // 返回碰撞点处的光线方向
        virtual Vector3D
        get_direction(ShadeRec& sr) = 0;
        // 返回入射辐射度
        virtual RGBColor
        L(ShadeRec& sr);

        virtual bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const = 0;

        virtual bool   casts_shadows(void) const;

        virtual void
        set_shadows(bool val);

        virtual float
        G(const ShadeRec& sr) const;

        virtual float
        pdf(const ShadeRec& sr) const;

    private:

        // 各光源可开启/关闭相应的阴影投射操作。
        bool    shadows;    // does the light cast shadows or not
        // 基类中不使用这两个成员变量，是因为对于区域光照和环境光照，
        // 光源缩放系数，也就是亮度值ls位于对应光照所使用的发射材质中，
        // 光源颜色值color也是这样。
        // float        ls;
        // RGBColor    color;

};


//-------------------------------------------------------------------- casts_shadows

inline bool
Light::casts_shadows() const {
    return shadows;
}


//-------------------------------------------------------------------- set_shadows

inline void
Light::set_shadows(bool _s) {
    shadows = _s;
}


// ---------------------------------------------------------------------- G
// explained on page 333
// Chapter 13.10.2：面积模型中的几何参数项。
inline float
Light::G(const ShadeRec& sr) const {
    return 1.0;
}


// ---------------------------------------------------------------------- pdf
// Chapter 13.11.3：用于Monte Carlo积分的概率密度函数
inline float
Light::pdf(const ShadeRec& sr) const {
    return 1.0;
}

#endif
