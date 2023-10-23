#include "stdafx.h"
#include "PointLight.h"
#include "World.h"

// ---------------------------------------------------------------------- default constructor

PointLight::PointLight (void)
    :    Light(),
        ls(1.0),
        color(1.0),
        location()
{}


// ---------------------------------------------------------------------- copy constructor

PointLight::PointLight (const PointLight& a)
    :    Light(a),
        ls(a.ls),
        color(a.color),
        location(a.location)
{}


// ---------------------------------------------------------------------- clone

Light*
PointLight::clone(void) const {
    return (new PointLight(*this));
}


// ---------------------------------------------------------------------- assignment operator

PointLight&
PointLight::operator= (const PointLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator= (rhs);

    ls        = rhs.ls;
    color     = rhs.color;
    location = rhs.location;

    return (*this);
}


// ---------------------------------------------------------------------- destructor

PointLight::~PointLight (void) {}


// ---------------------------------------------------------------------- get_direction
// ����һ��������ײ��ĵ�λ������
Vector3D
PointLight::get_direction(ShadeRec& sr) {
    // ����ʹ���˴洢��ShadeRec�����е���ײ�����ꡣ
    return ((location - sr.hit_point).hat());
}


// ---------------------------------------------------------------------- L
// ��Ambient��һ������������Ҳ�ǹ�Դ����ϵ�������Թ�Դ��ɫֵ���μ���ʽ14.1��
RGBColor
PointLight::L(ShadeRec& sr) {
    return (ls * color);
}


// ---------------------------------------------------------------------- in_shadow
// �ж��Լ������Ĺ��Ƿ��ײ�����塣ֻҪײ��һ�����壬�ͷ����档
// ��������£�������Գ����е�ȫ������ִ����Ӱ�����ཻ���ԣ�
// ��ĿǰΪֹ���ú�����Ȼ����ɫ��������Ϊ��ʱ��һ����
bool
PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float t;
    int num_objects = sr.w.objects.size();
    float d = location.distance(ray.o);

    // ���������е�ȫ�����󲢵��ø������shadow_hit() ������
    // �Լ����Ӱ�����Ƿ���λ����ɫ��ײ��͹�Դλ��֮��Ķ��������ײ�� 
    for(int j = 0; j < num_objects; j++)
    {
        if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
        {
            // ��������ײ����ú��������˳���
            return true;
        }
    }
    return false;
}


// ---------------------------------------------------------------------- G

float
PointLight::G(const ShadeRec& sr) const{
    return 5.5; //?
}


// ---------------------------------------------------------------------- pdf

float
PointLight::pdf(const ShadeRec& sr) const{

    return 5.5;//?
}
