﻿#include "stdafx.h"
// This file contains the definition of the class sphere

#include "Sphere.h"
#include "math.h"

const double Sphere::kEpsilon = 0.001;

// ---------------------------------------------------------------- default constructor

Sphere::Sphere(void)
    :     GeometricObject(),
        center(0.0),
        radius(1.0)
{}


// ---------------------------------------------------------------- constructor

Sphere::Sphere(Point3D c, double r)
    :     GeometricObject(),
        center(c),
        radius(r)
{}


// ---------------------------------------------------------------- clone

Sphere*
Sphere::clone(void) const {
    return (new Sphere(*this));
}


// ---------------------------------------------------------------- copy constructor

Sphere::Sphere (const Sphere& sphere)
    :     GeometricObject(sphere),
        center(sphere.center),
        radius(sphere.radius)
{}



// ---------------------------------------------------------------- assignment operator

Sphere&
Sphere::operator= (const Sphere& rhs)
{
    if (this == &rhs)
        return (*this);

    GeometricObject::operator= (rhs);

    center     = rhs.center;
    radius    = rhs.radius;

    return (*this);
}


// ---------------------------------------------------------------- destructor

Sphere::~Sphere(void) {}


//---------------------------------------------------------------- hit
// explained on page 57
// 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
// 并把计算出来碰撞点和碰撞点法线方向，存入ShadeRec中。
bool
Sphere::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {

    double         t;
    Vector3D    temp     = ray.o - center;
    double         a         = ray.d * ray.d;
    double         b         = 2.0 * temp * ray.d;
    double         c         = temp * temp - radius * radius;
    double         disc    = b * b - 4.0 * a * c;

    if (disc < 0.0)
        return(false);
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom;    // smaller root

        if (t > kEpsilon) {
            tmin = t;
            sr.normal     = (temp + t * ray.d) / radius;
            sr.local_hit_point = ray.o + t * ray.d;

            return (true);
        }

        t = (-b + e) / denom;    // larger root

        if (t > kEpsilon) {
            tmin = t;
            sr.normal   = (temp + t * ray.d) / radius;
            sr.local_hit_point = ray.o + t * ray.d;
            return (true);
        }
    }

    return (false);
}


//---------------------------------------------------------------- shadow_hit
// 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
// 只判断是否相交。不计算碰撞点
bool
Sphere::shadow_hit(const Ray& ray, float& tmin) const {

    double         t;
    Vector3D    temp     = ray.o - center;
    double         a         = ray.d * ray.d;
    double         b         = 2.0 * temp * ray.d;
    double         c         = temp * temp - radius * radius;
    double         disc    = b * b - 4.0 * a * c;

    if (disc < 0.0)
        return(false);
    else {
        double e = sqrt(disc);
        double denom = 2.0 * a;

        t = (-b - e) / denom;    // smaller root

        if (t > kEpsilon)
            return (true);

        t = (-b + e) / denom;    // larger root

        if (t > kEpsilon)
            return (true);
    }

    return (false);
}
