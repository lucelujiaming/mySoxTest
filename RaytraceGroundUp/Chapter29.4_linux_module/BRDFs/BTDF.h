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
// 这里所讨论的简单透明材质使用了单折射率n=1in， 这对于渲染空气(m out=1) 中的透明对象已然足够了。
// 另外，通过使用适当的n值，该方案还可渲染其他类型的透明对象，例如水中的气泡。
// BTDF类定义了一个tir() 函数用以检测全内反射，以及一个fresnel() 函数用来求解Fresnel方程。
// 另外，该类还包含了一些辅助函数，例如函数f() 、sample_f()以及rho()。
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
                
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;
        
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
};

#endif
