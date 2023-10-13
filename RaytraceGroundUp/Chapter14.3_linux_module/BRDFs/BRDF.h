#ifndef __BRDF__
#define __BRDF__

// This file contains the declaration of the base class BRDF

#include <math.h>

#include "RGBColor.h"
#include "Vector3D.h"
#include "ShadeRec.h"

#include "Sampler.h"

// 待BRDF对象实现了基本的材质反射机制后， 各材质对象均包含至少一个BRDF。
// 各类均具有空间不变性特征。
// 各个BRDF类均定义了3个成员函数：f()函数，sample_f()函数以及rho()函数，
// 每个函数将返回一个RGB颜色值。
class BRDF {

    public:

        BRDF(void);

        BRDF(const BRDF& object);

        virtual BRDF*
        clone(void) const = 0;

        ~BRDF(void);
        // 针对于反射材质以及漫反射-漫反射光线模拟计算。 
        // 如果不包含delta()函数，将返回BRDF自身。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 用于计算反射光线的方向。
        // 参数wi；用于返回最终的方向，因而不可为常量。
        // 顾名思义，相关方向将通过对BRDF采样计算得到。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;

        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;

    protected:

        BRDF&
        operator= (const BRDF& rhs);
        // 这也是BRDF基类包含一个采样器对象指针的原因。
        Sampler* sampler_ptr;
};

#endif
