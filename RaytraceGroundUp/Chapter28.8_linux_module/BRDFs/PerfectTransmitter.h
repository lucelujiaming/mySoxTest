//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#ifndef __PERFECT_TRANSMITTER__
#define __PERFECT_TRANSMITTER__

// this implements perfect specular transmission for transparent materials

#include "BTDF.h"

class PerfectTransmitter: public BTDF {
    public:
    
        PerfectTransmitter(void);
        
        PerfectTransmitter(const PerfectTransmitter& pt);
        
        virtual PerfectTransmitter*
        clone(void);
        
        ~PerfectTransmitter(void);
        
        PerfectTransmitter&                            
        operator= (const PerfectTransmitter& rhs);
        
        void
        set_kt(const float k);
        
        void
        set_ior(const float eta);
        // 检测全内反射
        bool                                                    
        tir(const ShadeRec& sr) const;
        // 返回BTDF比例系数。
        // 针对于折射材质模拟计算，返回计算出来的颜色值。
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // 用于计算折射光线的方向，并且返回计算出来的颜色值。
        // 参数wt；用于返回折射的方向，因而不可为常量。
        // 返回黑色。因为不存在BRDF随机采样。
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const;
        // 返回双半球反射系数。 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
                
    private:
        float    kt;         // 界面间的透射通量面片：transmission coefficient
        // 简单透明材质使用了单折射率，这对于渲染空气中的透明对象已然足够了。
        float    ior;        // 折射率：index of refraction
};


// -------------------------------------------------------------- set_kt

inline void
PerfectTransmitter::set_kt(const float k) {
    kt = k;
}

// -------------------------------------------------------------- set_ior

inline void
PerfectTransmitter::set_ior(const float eta) {
    ior = eta;
}

#endif
