#include "AreaLighting.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

AreaLighting::AreaLighting(void)
    : Tracer()
{}


// -------------------------------------------------------------------- constructor

AreaLighting::AreaLighting(World* _worldPtr)
    : Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

AreaLighting::~AreaLighting(void) {}


// -------------------------------------------------------------------- trace_ray

RGBColor
AreaLighting::trace_ray(const Ray& ray) const {

    ShadeRec sr(world_ptr->hit_objects(ray));

    if (sr.hit_an_object) {
        sr.ray = ray;            // used for specular shading
        // 与Ray Cast::trace_ray() 函数的唯一区别在于：
        //   该函数使用了material_ptr->area_light_shade()而非material_ptr->shade()。
        return (sr.material_ptr->area_light_shade(sr));
    }
    else
    {
        return (world_ptr->background_color);
    }
}


// -------------------------------------------------------------------- trace_ray
// this ignores the depth argument

RGBColor
AreaLighting::trace_ray(const Ray ray, const int depth) const {

    ShadeRec sr(world_ptr->hit_objects(ray));

    if (sr.hit_an_object) {
        sr.ray = ray;            // used for specular shading
        // 与Ray Cast::trace_ray() 函数的唯一区别在于：
        //   该函数使用了material_ptr->area_light_shade()而非material_ptr->shade()。
        return (sr.material_ptr->area_light_shade(sr));
    }
    else
    {
        return (world_ptr->background_color);
    }
}
