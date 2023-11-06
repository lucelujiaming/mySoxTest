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
// �������ڷ�����㡣���������ڲ��������ײ�����Ϻ�ʹ��ʽ(28.1) ~ʽ(28.3)���м��� ��
// BRDF�а�������ͬ�ĺ����Ҹ���ʽ(28.4)����͸����㡣ע�⣬�ú�������������ȫ�ڷ��䡣
float
FresnelReflector::fresnel(const ShadeRec& sr) const {
    Normal normal(sr.normal);
    // P455��cos��i=n * ��0
    float ndotd = -normal * sr.ray.d;
    float eta;

    // ���cos_thetaiС���㣬˵���Ƿ��ߺ����䷽��λ�ڽ��ʵ�ͬһ�ࡣ
    // �����ˮ���·����Ϲ۲졣
    // ִ�е��ò�����
    if (ndotd < 0.0) {
        normal = -normal;
        eta = eta_out / eta_in;
    }
    else
    {
        eta = eta_in / eta_out;
    }

    // ����cos��i
    float cos_theta_i         = -normal * sr.ray.d;
    // ����cos��t
    float temp                 = 1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta);
    float cos_theta_t         = sqrt (1.0 - (1.0 - cos_theta_i * cos_theta_i) / (eta * eta));
    // �μ���ʽ(28.1)
    float r_parallel         = (eta * cos_theta_i - cos_theta_t) / (eta * cos_theta_i + cos_theta_t);
    // �μ���ʽ(28.2)
    float r_perpendicular     = (cos_theta_i - eta * cos_theta_t) / (cos_theta_i + eta * cos_theta_t);
    // �μ���ʽ(28.3)
    float kr                 = 0.5 * (r_parallel * r_parallel + r_perpendicular * r_perpendicular);

    return (kr);
}


// ---------------------------------------------------------- f

// ����ڷ�������Լ�������-���������ģ����㣬���ؼ����������ɫֵ��
// ���غ�ɫ����Ϊ�������λ�ھ��淴�䷽���ϣ�������BRDF���������
RGBColor
FresnelReflector::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ----------------------------------------------------------------------------------------- sample_f
// this computes wi: the direction of perfect mirror reflection
// it's called from from the functions Reflective::shade and Transparent::shade.
// the fabs in the last statement is for transparency
// explained on page 500
// ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
RGBColor
FresnelReflector::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wr) const {
    // �μ���ʽ24.2
    float ndotwo = sr.normal * wo;
    wr = -wo + 2.0 * sr.normal * ndotwo;
    return (fresnel(sr) * white / fabs(sr.normal * wr));
}

// ---------------------------------------------------------- rho
RGBColor
FresnelReflector::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
