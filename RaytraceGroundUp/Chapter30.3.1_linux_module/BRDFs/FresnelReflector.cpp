//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.

#include "Constants.h"

#include "FresnelReflector.h"

// ---------------------------------------------------------- default constructor

FresnelReflector::FresnelReflector(void)
    :     BRDF(),
        kr(0.0),
        cr(1.0)
{}

// ---------------------------------------------------------- destructor

FresnelReflector::~FresnelReflector(void) {}


// ---------------------------------------------------------------------- clone

FresnelReflector*
FresnelReflector::clone(void) const {
    return (new FresnelReflector(*this));
}


// ----------------------------------------------------------------------------------------- fresnel
// 函数用于反射计算。待光线与内部表面的碰撞检测完毕后，使用式(28.1) ~式(28.3)进行计算 。
// BRDF中包含了相同的函数且根据式(28.4)进行透射计算。注意，该函数并不适用于全内反射。
float
FresnelReflector::fresnel(const ShadeRec& sr) const {
    Normal normal(sr.normal);
    // P455：cosθi=n * ω0
    float ndotd = -normal * sr.ray.d;
    float eta;

    // 如果cos_thetai小于零，说明是法线和入射方向位于介质的同一侧。
    // 例如从水面下方向上观察。
    // 执行倒置操作。
    if (ndotd < 0.0) {
        normal = -normal;
        eta = eta_out / eta_in;
    }
    else
    {
        eta = eta_in / eta_out;
    }

    // 计算cosθi
    float cos_theta_i         = -normal * sr.ray.d;
    // 计算cosθt
    float temp                 = 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
    float cos_theta_t         = sqrt (1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta));
    // 参见公式(28.1)
    float r_parallel         = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
    // 参见公式(28.2)
    float r_perpendicular     = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
    // 参见公式(28.3)
    float kr                 = 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

    return (kr);
}


// ---------------------------------------------------------- f

// 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
// 返回黑色。因为反射光线位于镜面反射方向上，不存在BRDF随机采样。
RGBColor
FresnelReflector::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ----------------------------------------------------------------------------------------- sample_f
// this computes wi: the direction of perfect mirror reflection
// it's called from from the functions Reflective::shade and Transparent::shade.
// the fabs in the last statement is for transparency
// explained on page 500
// 用于计算反射光线的方向，并且返回计算出来的颜色值。
RGBColor
FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const {
    // 参见公式24.2
    float ndotwo = sr.normal * wo;
    wr = -wo + 2.0 * sr.normal * ndotwo;
    return (fresnel(sr) * white / fabs(sr.normal * wr));
}

// ---------------------------------------------------------- rho
RGBColor
FresnelReflector::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
