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
    // 调用World::hit_objects初始化一个局部ShadeRec对象。
    ShadeRec sr(world_ptr->hit_objects(ray));
    // 如果击中物体，
    if (sr.hit_an_object) {
        // 返回材质上这根光线对应的颜色。
        sr.ray = ray;            // used for specular shading
        return (sr.material_ptr->shade(sr));
    }
    else
    {
        // 如果没有命中，返回背景颜色。
        RGBColor  backgroundColor = world_ptr->background_color;
        return backgroundColor;
    }
}


// -------------------------------------------------------------------- trace_ray
// this ignores the depth argument

RGBColor
RayCast::trace_ray(const Ray ray, const int depth) const {
    // 调用World::hit_objects初始化一个局部ShadeRec对象。
    ShadeRec sr(world_ptr->hit_objects(ray));
    // 如果击中物体，
    if (sr.hit_an_object) {
        // 返回材质上这根光线对应的颜色。
        sr.ray = ray;            // used for specular shading
        return (sr.material_ptr->shade(sr));
    }
    else
    {
        // 如果没有命中，返回背景颜色。
        return (world_ptr->background_color);
    }
}
