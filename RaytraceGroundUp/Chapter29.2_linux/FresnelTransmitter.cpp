//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "FresnelTransmitter.h"

// ------------------------------------------------------------------- default constructor

FresnelTransmitter::FresnelTransmitter(void)
    :     BTDF(),
        eta_in(1.0),
        eta_out(1.0)
{}


// ------------------------------------------------------------------- copy constructor

FresnelTransmitter::FresnelTransmitter(const FresnelTransmitter& ft)
    :     BTDF(ft),
        eta_in(ft.eta_in),
        eta_out(ft.eta_out)
{}


// ------------------------------------------------------------------- clone

FresnelTransmitter*
FresnelTransmitter::clone(void) {
    return (new FresnelTransmitter(*this));
}


// ------------------------------------------------------------------- destructor

FresnelTransmitter::~FresnelTransmitter(void) {}



// ------------------------------------------------------------------- assignment operator

FresnelTransmitter&
FresnelTransmitter::operator= (const FresnelTransmitter& rhs) {
    if (this == &rhs)
        return (*this);

    eta_in = rhs.eta_in;
    eta_out = rhs.eta_out;

    return (*this);
}


// ----------------------------------------------------------------------------------------- fresnel

float
FresnelTransmitter::fresnel(const ShadeRec& sr) const {
    Normal normal(sr.normal);
    float ndotd = -normal * sr.ray.d;
    float eta;

    if (ndotd < 0.0) {
        normal = -normal;
        eta = eta_out / eta_in;
    }
    else
        eta = eta_in / eta_out;

    float cos_theta_i         = -normal * sr.ray.d;
    float temp                 = 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
    float cos_theta_t         = sqrt (1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta));
    float r_parallel         = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
    float r_perpendicular     = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
    float kr                 = 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

    return (1.0 - kr);
}


// ------------------------------------------------------------------- tir
// tests for total internal reflection
// 检测全内反射。
// P455：根据第15章所讨论的反射定律以及Snell定律，
//       可以进一步推导出折射光线t的计算公式，
//       但这里将直接引用其计算结果。
bool
FresnelTransmitter::tir(const ShadeRec& sr) const {
    Vector3D wo(-sr.ray.d);
    float cos_thetai = sr.normal * wo;
    float eta = eta_in / eta_out;

    if (cos_thetai < 0.0)
        eta = 1.0 / eta;

    return (1.0 - (1.0 - cos_thetai * cos_thetai) / (eta * eta) < 0.0);
}


// ------------------------------------------------------------------- f
// 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
// 返回黑色。因为反射光线位于镜面反射方向上，不存在BRDF随机采样。
RGBColor
FresnelTransmitter::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------- sample_f
// this computes the direction wt for perfect transmission
// and returns the transmission coefficient
// this is only called when there is no total internal reflection
// 该函数计算透视光线的方向并将其返回至参数wt中且仅适用于非全内反射条件下。
RGBColor
FresnelTransmitter::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wt) const {

    Normal n(sr.normal);
    // 参见公式24.2
    float cos_thetai = n * wo;
    float eta = eta_in / eta_out;
    // 如果cos_thetai小于零，说明是法线和入射方向位于介质的同一侧。
    // 例如从水面下方向上观察。
    // 执行倒置操作。
    if (cos_thetai < 0.0) {            // transmitted ray is outside
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
    return (fresnel(sr) / (eta * eta) * white / fabs(sr.normal * wt));
}


// ------------------------------------------------------------------- rho

RGBColor
FresnelTransmitter::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}


