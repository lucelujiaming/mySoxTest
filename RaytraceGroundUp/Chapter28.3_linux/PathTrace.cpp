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
// ���⣬���������۵�·�����ټ��������Lambertian�����Լ���ȫ������ʣ�
// ���ڹ�������ʽ���Ϊ��ϰ������ߡ�
RGBColor    
PathTrace::trace_ray(const Ray ray, const int depth) const {
    if (depth > world_ptr->vp.max_depth)
        return (black);
    else {
        ShadeRec sr(world_ptr->hit_objects(ray));  
                    
        if (sr.hit_an_object) {
            sr.depth = depth;
            sr.ray = ray;
            
            // �ú�����Whitted::trace_ray() ������Ψһ������ڣ�
            // �ú���������material_ptr->path_shade()��������material_ptr->shade()������
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
