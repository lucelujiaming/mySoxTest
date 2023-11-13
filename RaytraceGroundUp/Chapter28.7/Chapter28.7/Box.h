#ifndef __Box__
#define __Box__

#include "Sampler.h"
#include "GeometricObject.h"
#include "BBox.h"

// 轴对齐盒体与轴对齐包围盒具有相同的形状，但隶属于几何对象范畴并适用于建模图元。
// 另外，若对象表面未采取隐式方程定义，轴对齐盒体则非常适用于光线跟踪计算的教学演示。
// 同时，轴对齐盒体还将用于演示第28章中的全内反射。
class Box: public GeometricObject {

public:
        Box(void);

        Box(const double x0, const double x1,
                 const double y0, const double y1,
                 const double z0, const double z1);

        Box(const Point3D p0, const Point3D p1);

        Box(const Box& bbox);

        Box&
        operator=(const Box& bbox);

        virtual Box*
        clone(void) const;
    
        virtual bool                                                  
        hit(const Ray& ray, double& t, ShadeRec& s) const;

        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;
        // 单独对法线加以计算。
        virtual Normal
        get_normal(const int face_hit) const;

        virtual BBox
        get_bounding_box(void) const;

private:
        // 将对角顶点存储于6个私有类型的浮点数中。
        float x0, y0, z0, x1, y1, z1;
        
        static const double kEpsilon;                                               
};

#endif
