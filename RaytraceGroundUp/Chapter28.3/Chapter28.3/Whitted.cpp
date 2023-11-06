#include "stdafx.h"
#include "Whitted.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

Whitted::Whitted(void)
    : Tracer()
{}


// -------------------------------------------------------------------- constructor
        
Whitted::Whitted(World* _worldPtr)
    : Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

Whitted::~Whitted(void) {}


// -------------------------------------------------------------------- trace_ray
// explained on page 499
// ʹ��depth�����洢ShadeRec�����и������ߵķ��������
RGBColor    
Whitted::trace_ray(const Ray ray, const int depth) const {
    // �����������������غ�ɫ��
    if (depth > world_ptr->vp.max_depth)
    {
        return(black);
    }
    else {
        // ʹ��һ����ʱ����������ײ�����
        ShadeRec sr(world_ptr->hit_objects(ray));    
        // ���������ײ��
        if (sr.hit_an_object) {
            // ������ײ��ȡ�
            sr.depth = depth;
            // ������ײ�Ĺ��ߡ�
            sr.ray = ray;
            // ���÷�����ʵ�Shade������
            // Ҳ����Reflective::shade������
            // ��������ڲ������sr.w.tracer_ptr->trace_ray��ɵݹ顣
            return (sr.material_ptr->shade(sr));   
        }
        else
            return (world_ptr->background_color);
    }
}

RGBColor    
Whitted::trace_ray(const Ray ray, float& tmin, const int depth) const {
    // �����������������غ�ɫ��
    if (depth > world_ptr->vp.max_depth)
    {
        return(black);
    }
    else {
        // ʹ��һ����ʱ����������ײ�����
        ShadeRec sr(world_ptr->hit_objects(ray));    
        // ���������ײ��
        if (sr.hit_an_object) {
            // ������ײ��ȡ�
            sr.depth = depth;
            // ������ײ�Ĺ��ߡ�
            sr.ray = ray;
            // �����������������ײʱ��sr.t��ֵ��tmin�У�
            tmin = sr.t;
            // ���÷�����ʵ�Shade������
            // Ҳ����Reflective::shade������
            // ��������ڲ������sr.w.tracer_ptr->trace_ray��ɵݹ顣
            return (sr.material_ptr->shade(sr));   
        }
        else
            // ����kHugeValue��ֵ��tmin�С�
            tmin = kHugeValue;
            return (world_ptr->background_color);
    }
}
