#include "stdafx.h"
#include "Emissive.h"

// ---------------------------------------------------------------- default constructor

Emissive::Emissive(void)
    :    Material(),
        ls(40.0),
        ce(white)
{}


// ---------------------------------------------------------------- copy constructor

Emissive::Emissive(const Emissive& m)
    :     Material(m),
        ls(m.ls),
        ce(m.ce)
{}


// ---------------------------------------------------------------- clone

Material*
Emissive::clone(void) const {
    return (new Emissive(*this));
}


// ---------------------------------------------------------------- assignment operator

Emissive&
Emissive::operator= (const Emissive& rhs) {

    if (this == &rhs)
        return (*this);

    Material::operator=(rhs);

    return (*this);

}


// ---------------------------------------------------------------- destructor

Emissive::~Emissive(void) {

}


// ---------------------------------------------------------------- shade
// shade()����ʹ���Է�����ʿ����������͵Ĺ��߸��������ʹ�ã�
// ����Ray Cast��������Whitted��������
RGBColor
Emissive::shade(ShadeRec& sr) {
    // ���⣬������������������淨��λ��ͬһ��ʱ������������Le=ls * ce��
    if (-sr.normal * sr.ray.d > 0.0)
        return (ls * ce);
    // ���� ���������غ�ɫֵ��
    else
        return black;
}

// ---------------------------------------------------------------- area_light_shade
// explained on page 332
// ������Ҫǿ����һ���ǣ��������߻�μ����ߵ������ײ��λ��������߱�����ʱ��
// Area Lighting::trace_ray() �����øú������洢��sr�еķ��߱�ʾ��ǰ��ײ���ϵķ��ߡ�
// �����������������յ�ֱ����Ⱦ���룬�������������ϵ�ֱ�ӹ��շ���(��Area Light�ദ��)��
RGBColor
Emissive::area_light_shade(ShadeRec& sr) {
    // ���⣬������������������淨��λ��ͬһ��ʱ������������Le=ls * ce��
    if (-sr.normal * sr.ray.d > 0.0)
        return (ls * ce);
    // ���� ���������غ�ɫֵ��
    else
        return black;
}


// ---------------------------------------------------------------- get_Le
// ����Emissive::get_Le()Ҳ�����ط�������ls * ce��
// ���������Դ�е�L()����(�μ������嵥18.10)������á�
RGBColor
Emissive::get_Le(ShadeRec& sr) const {
    // ���⣬������������������淨��λ��ͬһ��ʱ������������Le=ls * ce��
    if (-sr.normal * sr.ray.d > 0.0)
        return (ls * ce);
    // ���� ���������غ�ɫֵ��
    else
        return black;
}
