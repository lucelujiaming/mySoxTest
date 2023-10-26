//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "PerfectTransmitter.h"

// ------------------------------------------------------------------- default constructor

PerfectTransmitter::PerfectTransmitter(void)
    :     BTDF(),
        kt(0.0), 
        ior(1.0)
{}


// ------------------------------------------------------------------- copy constructor

PerfectTransmitter::PerfectTransmitter(const PerfectTransmitter& pt)
    :     BTDF(pt),
        kt(pt.kt), 
        ior(pt.ior)
{}


// ------------------------------------------------------------------- clone

PerfectTransmitter* 
PerfectTransmitter::clone(void) {
    return (new PerfectTransmitter(*this));
}


// ------------------------------------------------------------------- destructor

PerfectTransmitter::~PerfectTransmitter(void) {}



// ------------------------------------------------------------------- assignment operator
        
PerfectTransmitter&                            
PerfectTransmitter::operator= (const PerfectTransmitter& rhs) {
    if (this == &rhs)
        return (*this);
        
    kt = rhs.kt;
    ior = rhs.ior;

    return (*this);
}


// ------------------------------------------------------------------- tir
// tests for total internal reflection
// 检测全内反射。
// P455：根据第15章所讨论的反射定律以及Snell定律，
//       可以进一步推导出折射光线t的计算公式，
//       但这里将直接引用其计算结果。
bool                                                    
PerfectTransmitter::tir(const ShadeRec& sr) const {
    Vector3D wo(-sr.ray.d); 
    float cos_thetai = sr.normal * wo;  
    float eta = ior;
    
    if (cos_thetai < 0.0) 
        eta = 1.0 / eta; 
    // 参见公式27.4和公式27.5
    return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}    


// ------------------------------------------------------------------- f

RGBColor
PerfectTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------- sample_f
// this computes the direction wt for perfect transmission
// and returns the transmission coefficient
// this is only called when there is no total internal reflection
// 该函数计算透视光线的方向并将其返回至参数wt中且仅适用于非全内反射条件下。
RGBColor
PerfectTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const {
    
    Normal n(sr.normal);
    // P455：cosθi=n * ω0
    float cos_thetai = n * wo;
    float eta = ior;    
    // 如果cos_thetai小于零，说明是法线和入射方向位于介质的同一侧。
    // 例如从水面下方向上观察。
    // 执行倒置操作。
    if (cos_thetai < 0.0) {          // transmitted ray is outside     
        cos_thetai = -cos_thetai;
        n = -n;                      // reverse direction of normal
        eta = 1.0 / eta;             // invert ior 
    }

    // 计算cosθt。参见公式27.4
    float temp = 1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta);
    float cos_theta2 = sqrt(temp);
    // 参见公式27.3
    wt = -wo / eta - (cos_theta2 - cos_thetai / eta) * n;   
    // 参见公式27.10
    // 由于本章所讨论的透射方案不涉及颜色计算，因此返回值将乘以白色值以显示为RGB颜色。
    return (kt / (eta * eta) * white / fabs(sr.normal * wt));
}

// ------------------------------------------------------------------- rho

RGBColor
PerfectTransmitter::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}


