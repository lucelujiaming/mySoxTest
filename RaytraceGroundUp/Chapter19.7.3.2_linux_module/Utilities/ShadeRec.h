#ifndef __SHADE_REC__
#define __SHADE_REC__

// this file contains the declaration of the class ShadeRec

#include <vector>

class Material;
class World;

// We need the following as #includes instead of forward class declarations,
// because we have the objects themselves, not pointers or references

#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"
#include "RGBColor.h"

// ShadeRec对象在光线跟踪器的着色过程中扮演着重要的角色。
class ShadeRec {

    public:

        bool                hit_an_object;      // Did the ray hit an object?
        Material*           material_ptr;       // Pointer to the nearest object's material
        // 在没有对光线击中的对象进行变换，也就是缩放旋转平移的情况下，hit_point和local_hit_point相等。
        // 但是如果发生了变换，这两个值就不一定相等了。这就是为什么分成两个值的原因。
        Point3D             hit_point;          // World coordinates of intersection
        Point3D             local_hit_point;    // World coordinates of hit point on generic object
                                                // (used for texture transformations)
        Normal              normal;             // Normal at hit point
        Ray                 ray;                // Required for specular highlights and area lights
        int                 depth;              // recursion depth
        Vector3D            dir;                // for area lights
        float               t;                  // ray parameter
        // Shade Rec对象存储了一个引用值以简化着色代码的语法。
        const World&        w;                  // World reference
        RGBColor            color;              // Only used in Chapter3
		double              u, v;               // texture parameters

        ShadeRec(const World& wr);              // constructor
        ShadeRec(const ShadeRec& sr);           // copy constructor
};

#endif
