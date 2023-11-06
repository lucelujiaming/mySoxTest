#include "stdafx.h"
#include "AmbientOccluder.h"
#include "ShadeRec.h"
#include "World.h"


// ---------------------------------------------------------------------- default constructor

AmbientOccluder::AmbientOccluder(void)
    :     Light(),
        ls(1.0),
        color(1.0),
        min_amount(0.25),
        u(0), v(0), w(0),
        sampler_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

AmbientOccluder::AmbientOccluder(const AmbientOccluder& a)
    :     Light(a),
        ls(a.ls),
        color(a.color),
        min_amount(a.min_amount),
        u(a.u), v(a.v), w(a.w)
{
    // need to do a deep copy of the sampler
    if(a.sampler_ptr != NULL) {
        sampler_ptr = a.sampler_ptr->clone();
    }
}


// ---------------------------------------------------------------------- clone

Light*
AmbientOccluder::clone(void) const {
    return (new AmbientOccluder(*this));
}


// ---------------------------------------------------------------------- assignment operator

AmbientOccluder&
AmbientOccluder::operator= (const AmbientOccluder& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator= (rhs);

    ls         = rhs.ls;
    color     = rhs.color;
    min_amount = rhs.min_amount;
    u = rhs.u; v = rhs.v; w = rhs.w;

    // need to do a deep copy of the sampler
    if(rhs.sampler_ptr != NULL) {
        sampler_ptr = rhs.sampler_ptr->clone();
    }

    return (*this);
}


// ---------------------------------------------------------------------- destructor

AmbientOccluder::~AmbientOccluder (void) {}


// ---------------------------------------------------------------------- get_direction
// ���ظ�����Ӱ���ߵķ���
Vector3D
AmbientOccluder::get_direction(ShadeRec& sr) {
    // ���ش洢�ڲ����������е���һ�������㣬ӳ�䵽�����塣
    // ��Ϊ��ͨ������£�������Ҫ�ھֲ�����ϵ�м�����ߵķ���
    // ����������ù�������������ϵ�еķ���
    Point3D sp = sampler_ptr->sample_hemisphere();
    // �ú���ֻ�Ǽ򵥵ؽ��ֲ���������ط���ͶӰ��(u��v��w)�ϡ�
    // �μ���ʽ17.4
    return (sp.x * u + sp.y * v + sp.z * w);
}    

// ---------------------------------------------------------------------- set_sampler
// ���ò���������sp
void
AmbientOccluder::set_sampler(Sampler* s_ptr) {
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // ���ò���������sp(��build() �����м��Թ���)��
    sampler_ptr = s_ptr;
    // ��������ҷֲ�(e=1)�ķ�ʽ������ӳ��������ģ��֮�ϡ�
    sampler_ptr->map_samples_to_hemisphere(1);
}


// ---------------------------------------------------------------------- L
// �����������ȡ�
// ��ʾ�������������Ĺ��������Լ�get_direction() ������in_shadow() �����ĵ��÷�ʽ��
RGBColor
AmbientOccluder::L(ShadeRec& sr) {
    // ���ȹ���(u��v��w)��
    // 1. wֱ��ʹ�÷��߷���
    w = sr.normal;
    // 2. u��vλ�ڷ���ƽ�档
    //    u�������ϵķ����������Vector3D(0.0072, 1.0, 0.0034)����w�Ĳ�ˡ�
    //    �μ�Chapter17.1��β���еĹ�ʽ��
    //           v = w �� up / ||w �� up||
    //    2.1. ����w �� up
    v = w ^ Vector3D(0.0072, 1.0, 0.0034); // jitter the up vector in case normal is vertical
    //    2.2. ����һ��������||w �� up||��
    v.normalize();
    // 3. �μ�Chapter17.1��β���еĹ�ʽ��
    //         u = v �� w
    u = v ^ w;
    // ����һ����ʱ���߱�����
    Ray shadow_ray;
    // ʹ����ײ����Ϊ��㡣
    shadow_ray.o = sr.hit_point;
    // �����Ӱ���ߵķ���
    shadow_ray.d = get_direction(sr);
    // ������Ӱ�Ƿ�ĳһ�������ڵ���
    if (in_shadow(shadow_ray, sr))
    {
        // ����ڵ����ͼ����������ȡ�
        return (min_amount * ls * color);
    }
    else
    {
        // �����������Ⱦ��ǹ�Դ����ϵ�������Թ�Դ��ɫֵ���μ���ʽ14.1��
        return (ls * color);
    }
}

// ---------------------------------------------------------------------- in_shadow
// Ϊ�˽�һ��������Ӱ�����Ƿ�ĳһ�������赲�� ��ʹ�ñ�׼��in_shadow() ������
// �����߼���ͬ�������Դ�Ķ�Ӧ������
bool
AmbientOccluder::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float     t;
    int     num_objects = sr.w.objects.size();
    
    // ���������е�ȫ�����󲢵��ø������shadow_hit() ������
    // �Լ����Ӱ�����Ƿ���λ����ɫ��ײ��͹�Դλ��֮��Ķ��������ײ�� 
    for (int j = 0; j < num_objects; j++)
    {
        if (sr.w.objects[j]->shadow_hit(ray, t))
        {
            // ��������ײ����ú��������˳���
            return (true);
        }
    }
    
    return (false);
}

// ---------------------------------------------------------------------- G

float
AmbientOccluder::G(const ShadeRec& sr) const{
    return 5.5; //?
}

// ---------------------------------------------------------------------- pdf

float
AmbientOccluder::pdf(const ShadeRec& sr) const{

    return 5.5;//?
}
