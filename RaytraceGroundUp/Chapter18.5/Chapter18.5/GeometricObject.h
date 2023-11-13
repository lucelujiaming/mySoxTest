#ifndef __GEOMETRIC_OBJECT__
#define __GEOMETRIC_OBJECT__


#include <math.h>  // a lot of hit functions use use maths functions

#include "Constants.h"
#include "RGBColor.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Normal.h"
#include "Ray.h"
#include "ShadeRec.h"

//----------------------------------------------------------------------------------------------------- 
// class GeometricObject

// 全部几何对象均继承自基类Geometric Object。
// 这是光线跟踪器框架中最为庞大的继承层次结构，大约包含了40个对象。
// 在本章中，将使用某些简化的数据结构，包括Geometric Object 类、Plane类以及Sphere类。
class GeometricObject {    
    public:    

        GeometricObject(void);                                    
        
        GeometricObject(const GeometricObject& object);            
    
        virtual GeometricObject*                                
        clone(void) const = 0;

        virtual                                                 
        ~GeometricObject(void);    
        
        // hit() 函数参数列表中的Shade Rec对象充当一个工具类，
        // 用以存储光线跟踪器所需的全部信息，并对光线-对象间的碰撞点进行着色。
        // 简单地讲，着色将会计算反射光线的颜色值，这一过程将会在本书中多次出现。
        virtual bool                                                  
        hit(const Ray& ray, double& t, ShadeRec& s) const = 0;    

        virtual bool                                                  
        shadow_hit(const Ray& ray, float& t) const = 0;    

        virtual void                             // This needs to be virtual so that it can be overridden in Compound
            set_material(Material* mPtr);             // It therefore shouldn't be inlined

        Material*
            get_material(void) const;

        // The following three functions are only required for Chapter 3
        
        void
        set_color(const RGBColor& c);
                
        void
        set_color(const float r, const float g, const float b);
        
        RGBColor
        get_color(void);
        
        virtual void                                         // required for compound objects
        add_object(GeometricObject* object_ptr);
                
        
        // The following two functions are only required for objects that are light sources, eg disks, rectangles, and spheres
         
        virtual Point3D         
        sample(void);
        
        virtual float
        pdf(const ShadeRec& sr); 
                
        
        // The following two functions allow us to simplify the code for smooth shaded triangle meshes
        
        virtual Normal
        get_normal(void) const; 
        
        virtual Normal
        get_normal(const Point3D& p); 

    
    protected:

        mutable Material*   material_ptr;       // mutable allows the const functions Compound::hit, Instance::hit, and RegularGrid::hit to assign to material_ptr

        RGBColor               color;           // only used for Bare Bones ray tracing
    
        GeometricObject&                        
        operator= (const GeometricObject& rhs);
        // 对象选项将确定对象自身是否具备投射阴影的能力。这对于环境光照下的遮挡计算将是十分必要的。
        bool shadows;
};


// --------------------------------------------------------------------  set_color

inline void
GeometricObject::set_color(const RGBColor& c) {
    color = c;
}

// --------------------------------------------------------------------  set_color

inline void 
GeometricObject::set_color(const float r, const float g, const float b) {
    color.r = r;
    color.b = b;
    color.g = g;
}

// --------------------------------------------------------------------  get_color

inline RGBColor 
GeometricObject::get_color(void) {
    return (color);
}

#endif
