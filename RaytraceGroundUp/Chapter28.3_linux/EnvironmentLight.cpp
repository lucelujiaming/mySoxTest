#include "stdafx.h"
#include "EnvironmentLight.h"

// ---------------------------------------------------------------- default constructor

EnvironmentLight::EnvironmentLight(void)
    :     Light(),
        sampler_ptr(NULL),
        material_ptr(NULL),
        u(), v(), w(),
        wi()
{}    


// ---------------------------------------------------------------- copy constructor

EnvironmentLight::EnvironmentLight(const EnvironmentLight& el)
    :     Light(el),
        u(el.u), v(el.v), w(el.w),
        wi(el.wi)
{
    if(el.sampler_ptr)
        sampler_ptr = el.sampler_ptr->clone(); 
    else  sampler_ptr = NULL;

    if(el.material_ptr)
        material_ptr = el.material_ptr->clone(); 
    else  material_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Light*
EnvironmentLight::clone(void) const {
    return (new EnvironmentLight(*this));
}


// ---------------------------------------------------------------- destructor

EnvironmentLight::~EnvironmentLight(void) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }
}


// --------------------------------------------------------------- assignment operator

EnvironmentLight&
EnvironmentLight::operator= (const EnvironmentLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator=(rhs);

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (rhs.sampler_ptr)
        sampler_ptr = rhs.sampler_ptr->clone();

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }

    if (rhs.material_ptr)
        material_ptr = rhs.material_ptr->clone();

    return (*this);
}


// --------------------------------------------------------------- get_direction

// ���ظ�����Ӱ���ߵķ���
Vector3D
EnvironmentLight::get_direction(ShadeRec& sr) {
    // ���ȹ���(u��v��w)��
    // 1. wֱ��ʹ�÷��߷���
    w = sr.normal;
    // 2. u��vλ�ڷ���ƽ�档
    //    u�������ϵķ����������Vector3D(0.0072, 1.0, 0.0034)����w�Ĳ�ˡ�
    //    �μ�Chapter17.1��β���еĹ�ʽ��
    //           v = w �� up / ||w �� up||
    //    2.1. ����w �� up
    v = Vector3D(0.0034, 1, 0.0071) ^ w;
    //    2.2. ����һ��������||w �� up||��
    v.normalize();
    // 3. �μ�Chapter17.1��β���еĹ�ʽ��
    //         u = v �� w
    u = v ^ w;
    // ���ش洢�ڲ����������е���һ�������㣬ӳ�䵽�����塣
    // ��Ϊ��ͨ������£�������Ҫ�ھֲ�����ϵ�м�����ߵķ���
    // ����������ù�������������ϵ�еķ���
    Point3D sp = sampler_ptr->sample_hemisphere();
    // �ú���ֻ�Ǽ򵥵ؽ��ֲ���������ط���ͶӰ��(u��v��w)�ϡ�
    // �μ���ʽ17.4
    wi = sp.x * u + sp.y * v + sp.z * w;

    return (wi);
}


// ---------------------------------------------------------------------- set_sampler
// Environment Light���Ambient Occluder�����ĳЩ��ͬ������
// ���ȣ����߾�����ͬ��set_sampler()���������������㰴�����ҷֲ��ķ�ʽӳ��������ģ���ϡ�
void
EnvironmentLight::set_sampler(Sampler* s_ptr) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // ���ò���������sp(��build() �����м��Թ���)��
    sampler_ptr = s_ptr;
    // ��������ҷֲ�(e=1)�ķ�ʽ������ӳ��������ģ��֮�ϡ�
    sampler_ptr->map_samples_to_hemisphere(1);
}


// --------------------------------------------------------------- L

RGBColor
EnvironmentLight::L(ShadeRec& sr) {
    return (material_ptr->get_Le(sr));
}


// ---------------------------------------------------------------------- in_shadow
// not sure if this is correct, copied it from poitn light...
bool
EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    return false; // can an environment light even be in shadow?
}


// ---------------------------------------------------------------- pdf
// The following function is not in the book.
// It uses Equation 18.6
float
EnvironmentLight::pdf(const ShadeRec& sr) const {

    return (sr.normal * wi * invPI);
}
