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
// 使用depth参数存储ShadeRec对象中各条光线的反射次数。
RGBColor    
Whitted::trace_ray(const Ray ray, const int depth) const {
    // 大于最大反射次数，返回黑色。
    if (depth > world_ptr->vp.max_depth)
    {
        return(black);
    }
    else {
        // 使用一个临时变量保存碰撞结果。
        ShadeRec sr(world_ptr->hit_objects(ray));    
        // 如果发生碰撞，
        if (sr.hit_an_object) {
            // 保存碰撞深度。
            sr.depth = depth;
            // 保存碰撞的光线。
            sr.ray = ray;
            // 调用反射材质的Shade方法。
            // 也就是Reflective::shade函数。
            // 这个函数内部会调用sr.w.tracer_ptr->trace_ray完成递归。
            return (sr.material_ptr->shade(sr));   
        }
        else
            return (world_ptr->background_color);
    }
}

RGBColor    
Whitted::trace_ray(const Ray ray, float& tmin, const int depth) const {
    // 大于最大反射次数，返回黑色。
    if (depth > world_ptr->vp.max_depth)
    {
        return(black);
    }
    else {
        // 使用一个临时变量保存碰撞结果。
        ShadeRec sr(world_ptr->hit_objects(ray));    
        // 如果发生碰撞，
        if (sr.hit_an_object) {
            // 保存碰撞深度。
            sr.depth = depth;
            // 保存碰撞的光线。
            sr.ray = ray;
            // 当光线与对象间存在碰撞时，sr.t赋值于tmin中；
            tmin = sr.t;
            // 调用反射材质的Shade方法。
            // 也就是Reflective::shade函数。
            // 这个函数内部会调用sr.w.tracer_ptr->trace_ray完成递归。
            return (sr.material_ptr->shade(sr));   
        }
        else
            // 否则，kHugeValue赋值于tmin中。
            tmin = kHugeValue;
            return (world_ptr->background_color);
    }
}