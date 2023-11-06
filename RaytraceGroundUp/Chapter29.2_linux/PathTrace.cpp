//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "PathTrace.h"
#include "World.h"
#include "ShadeRec.h"
#include "Material.h"

// -------------------------------------------------------------------- default constructor

PathTrace::PathTrace(void)
    : Tracer()
{}


// -------------------------------------------------------------------- default constructor
        
PathTrace::PathTrace(World* _worldPtr)
    : Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- trace_ray
// 另外，这里所讨论的路径跟踪计算仅限于Lambertian材质以及完全镜面材质，
// 至于光泽反射材质将作为练习留予读者。
RGBColor    
PathTrace::trace_ray(const Ray ray, const int depth) const {
    if (depth > world_ptr->vp.max_depth)
        return (black);
    else {
        ShadeRec sr(world_ptr->hit_objects(ray));  
                    
        if (sr.hit_an_object) {
            sr.depth = depth;
            sr.ray = ray;
            
            // 该函数与Whitted::trace_ray() 函数的唯一差别在于：
            // 该函数调用了material_ptr->path_shade()函数而非material_ptr->shade()函数。
            return (sr.material_ptr->path_shade(sr));   
        }
        else
            return (world_ptr->background_color);
    }    
}


// -------------------------------------------------------------------- trace_ray
// this version has tmin as an argument, and is used only with the Dielectric material
// for color filtering

RGBColor    
PathTrace::trace_ray(const Ray ray, float& tmin, const int depth) const {
    if (depth > world_ptr->vp.max_depth)
        return (black);
    else {
        ShadeRec sr(world_ptr->hit_objects(ray));  
                    
        if (sr.hit_an_object) {
            sr.depth     = depth;
            sr.ray         = ray;
            tmin        = sr.t;     // required for colored transparency
            return (sr.material_ptr->path_shade(sr));   
        }
        else{
            tmin = kHugeValue;
            return (world_ptr->background_color);
        }
    }    
}
