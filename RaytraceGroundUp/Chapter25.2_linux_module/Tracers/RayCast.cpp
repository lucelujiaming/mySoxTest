#include "stdafx.h"
#include "RayCast.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

RayCast::RayCast(void)
    : Tracer()
{}


// -------------------------------------------------------------------- constructor

RayCast::RayCast(World* _worldPtr)
    : Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

RayCast::~RayCast(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor
RayCast::trace_ray(const Ray& ray) const {
    // ����World::hit_objects��ʼ��һ���ֲ�ShadeRec����
    ShadeRec sr(world_ptr->hit_objects(ray));
    // ����������壬
    if (sr.hit_an_object) {
        // ���ز�����������߶�Ӧ����ɫ��
        sr.ray = ray;            // used for specular shading
        return (sr.material_ptr->shade(sr));
    }
    else
    {
        // ���û�����У����ر�����ɫ��
        return (world_ptr->background_color);
    }
}


// -------------------------------------------------------------------- trace_ray
// this ignores the depth argument

RGBColor
RayCast::trace_ray(const Ray ray, const int depth) const {
    // ����World::hit_objects��ʼ��һ���ֲ�ShadeRec����
    ShadeRec sr(world_ptr->hit_objects(ray));
    // ����������壬
    if (sr.hit_an_object) {
        // ���ز�����������߶�Ӧ����ɫ��
        sr.ray = ray;            // used for specular shading
        return (sr.material_ptr->shade(sr));
    }
    else
    {
        // ���û�����У����ر�����ɫ��
        return (world_ptr->background_color);
    }
}
