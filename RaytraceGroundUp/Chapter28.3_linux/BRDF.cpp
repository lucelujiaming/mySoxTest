// This file contians the definition of the class BRDF

#include "stdafx.h"
#include "Constants.h"
#include "BRDF.h"

// ---------------------------------------------------------- default constructor

BRDF::BRDF(void) {}


BRDF::BRDF(const BRDF& brdf) {}


// --------------------------------------------------------------- assignment operator

BRDF&
BRDF::operator= (const BRDF& rhs) {

    if (this == &rhs)
        return (*this);

    return (*this);
}


// ---------------------------------------------------------- destructor

BRDF::~BRDF(void) {}


// ------------------------------------------------------------------------ f
// ����ڷ�������Լ�������-���������ģ����㣬���ؼ����������ɫֵ��
RGBColor
BRDF::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------------ sample_f
// ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
RGBColor
BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const {
    return (black);
}


// ------------------------------------------------------------------------ sample_f
// �ú�������ģ������䡣�μ�Chapter25.1��
// ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
// ʹ��Monte Calo����ʵ�֡�
RGBColor
BRDF::sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const {
    return (black);
}


// ------------------------------------------------------------------------ rho    
// ����˫������ϵ����
RGBColor
BRDF::rho(const ShadeRec& sr, const Vector3D& wo) const {
    return (black);
}
