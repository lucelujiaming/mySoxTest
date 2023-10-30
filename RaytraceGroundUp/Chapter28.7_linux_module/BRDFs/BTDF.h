//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#ifndef __BTDF__
#define __BTDF__

#include <math.h>

#include "Constants.h"
#include "Vector3D.h"
#include "ShadeRec.h"
// 这里所讨论的简单透明材质使用了单折射率n=1in，
// 这对于渲染空气(m out=1) 中的透明对象已然足够了。
// 另外，通过使用适当的n值，该方案还可渲染其他类型的透明对象，例如水中的气泡。
class BTDF {
    public:
    
        BTDF(void);
        
        BTDF(const BTDF& btdf);
        
        virtual BTDF*
        clone(void) = 0;
        
        BTDF&                            
        operator= (const BTDF& rhs);
        
        virtual
        ~BTDF(void);
        // 返回BTDF比例系数。
        // 针对于折射材质模拟计算，返回计算出来的颜色值。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 用于计算折射光线的方向，并且返回计算出来的颜色值。
        // 参数wt；用于返回折射的方向，因而不可为常量。
        // 顾名思义，相关方向将通过对BTDF采样计算得到。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;
        // 返回双半球折射系数。 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
};

#endif
