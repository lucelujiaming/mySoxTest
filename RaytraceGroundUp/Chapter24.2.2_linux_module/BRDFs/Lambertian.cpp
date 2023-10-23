#include "stdafx.h"
#include "Lambertian.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Lambertian::Lambertian(void)
    :   BRDF(),
        kd(0.0),
        cd(0.0)
{}


// ---------------------------------------------------------------------- copy constructor

Lambertian::Lambertian(const Lambertian& lamb)
    :   BRDF(lamb),
        kd(lamb.kd),
        cd(lamb.cd)
{
}


// ---------------------------------------------------------------------- clone

Lambertian*
Lambertian::clone(void) const {
    return (new Lambertian(*this));
}


// ---------------------------------------------------------------------- destructor

Lambertian::~Lambertian(void) {}


// ---------------------------------------------------------------------- assignment operator

Lambertian&
Lambertian::operator= (const Lambertian& rhs) {

    if (this == &rhs)
        return (*this);
        
    BRDF::operator= (rhs);
    
    kd = rhs.kd;
    cd = rhs.cd;
    
    return (*this);
}


// ---------------------------------------------------------------------- f

RGBColor
Lambertian::f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const {
    // pd�ɱ�ʾΪRGB��ɫֵ�� ����ɰ����·�ʽ��ʾ��
    //   pd = kd * cd / ��
    // ���У�kd��ʾ������ϵ������һ��0 ~ 1�����֡�caΪ��������ɫֵ��
    // ��ʹ���û����Կ��ٵ��޸�����ķ�������ݣ���Ϊ��صı�������ṩ�˷����ԡ�
    // �μ���ʽ13.19
    return (kd * cd * invPI);
}


// ---------------------------------------------------------------------- rho

RGBColor
Lambertian::rho(const ShadeRec& sr, const Vector3D& wo) const {
    // �μ���ʽ13.20
    return (kd * cd);
}
