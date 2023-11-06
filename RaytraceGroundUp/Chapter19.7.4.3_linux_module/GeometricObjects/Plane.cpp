#include "stdafx.h"
#include "Plane.h"

const double Plane::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

Plane::Plane(void)
    :     GeometricObject(),
        a(0.0),
        n(0, 1, 0)
{}


// ----------------------------------------------------------------------  constructor

Plane::Plane(const Point3D& point, const Normal& normal)
    :    GeometricObject(),
        a(point),
        n(normal)
{
        n.normalize();
}


// ---------------------------------------------------------------- copy constructor

Plane::Plane(const Plane& plane)
    :    GeometricObject(plane),
        a(plane.a),
        n(plane.n)
{}


// ---------------------------------------------------------------- clone

Plane*
Plane::clone(void) const {
    return (new Plane(*this));
}


// ---------------------------------------------------------------- assignment operator

Plane&
Plane::operator= (const Plane& rhs)    {

    if (this == &rhs)
        return (*this);

    GeometricObject::operator= (rhs);

    a = rhs.a;
    n = rhs.n;

    return (*this);
}


// ---------------------------------------------------------------- destructor

Plane::~Plane(void)
{}


// ----------------------------------------------------------------- hit
// explained on page 54
// 光线-对象间的碰撞检测函数。
// 对象的碰撞检测函数将以3种方式计算并返回相应的碰撞信息：
//   当仅返回布尔类型时，函数将判断光线是否与对象发生碰撞；
//   通过参数tmin返回最近碰撞点(若存在)的光线参数；
//   返回ShadeRec对象参数所需的相关信息。
// 第3章暂不需要法线数据，相关计算将在第14章讨论着色技术时加以介绍。
// 另外，在第29章分析纹理数据时，将会实现碰撞点数据local_hit_point。
// 此处，暂时添加这一类数据以保持碰撞测试函数的原貌。
// 
// 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
// 并把计算出来碰撞点和碰撞点法线方向，存入ShadeRec中。
bool
Plane::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    // 所谓的平面，定义方式为(a, n)。也就是该平面经过点a，且垂直于法线方向n。
    // 因此上，平面的任意一点p都满足：
    //     (p - a) * n = 0 
    // 即，从a到p的向量和法线方向n垂直。
    // 代入光线方程： p = o + td有：
    //      (o + td - a) * n = 0
    // 则：
    //       t = (a - o) * n / (d * n)
    // 即为下面的公式。
    // 也可以参见公式2.6和3.4
    float t = (a - ray.o) * n / (ray.d * n);

    if (t > kEpsilon) {
        tmin = t;
        sr.normal = n;
        sr.local_hit_point = ray.o + t * ray.d;

        return (true);
    }

    return(false);
}


// ----------------------------------------------------------------- hit
// explained on page 301
// 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
// 只判断是否相交。不计算碰撞点
bool
Plane::shadow_hit(const Ray& ray, float& tmin) const {

    float t = (a - ray.o) * n / (ray.d * n);

    if (t > kEpsilon) {
        tmin = t;
        return (true);
    }
    else
        return (false);
}

// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading

float
Plane::pdf(const ShadeRec& sr) {
    // 对于均匀光源来说，pdf为表面积的倒数。尤其是平面光源。
    // 我这里写成返回1.0。
    return (1.0);
}

