#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

//-------------------------------------------------------------------- class Plane
// Plane类存储了顶点和法线数据，其中包含了两个构造函数。
// 每个类均应包含一个默认构造函数，并在必要时添加其他类型的构造函数。
class Plane: public GeometricObject {
    public:
        Plane(void);                                                // default constructor
        Plane(const Point3D& point, const Normal& normal);            // constructor
        Plane(const Plane& plane);                                     // copy constructor
        virtual Plane*                                                 // virtual copy constructor
        clone(void) const;
        Plane&                                                         // assignment operator
        operator= (const Plane& rhs);    
        virtual                                                        // destructor
        ~Plane(void);
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 并把计算出来碰撞点和碰撞点法线方向，存入ShadeRec中。
        virtual bool
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;        // 定义了光线-对象间的碰撞检测函数。
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 只判断是否相交。不计算碰撞点。
        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;

        virtual float
            pdf(const ShadeRec& sr);
    public:

        Vector3D         a;                    // point through which plane passes
        Vector3D         n;                    // normal to the plane
        // 精度是阴影光线计算中的隐患之一。
        // 一种较好的方法是针对各几何对象图元，使用特定的静态类常量，这使得用户可对每一个对象定制相应的值。
        static const double kEpsilon;          // for shadows and secondary rays
};

#endif
