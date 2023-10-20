#ifndef __SPHERE__
#define __SPHERE__

// This file contains the declaration of the class Sphere

#include "GeometricObject.h"

//-------------------------------------------------------------------------------- class Sphere

class Sphere: public GeometricObject {

    public:

        Sphere(void);                                       // Default constructor

        Sphere(Point3D center, double r);                    // Constructor

        Sphere(const Sphere& sphere);                         // Copy constructor

        virtual Sphere*                                     // Virtual copy constructor
        clone(void) const;

        virtual                                                // Destructor
        ~Sphere(void);                                       

        Sphere&                                             // assignment operator
        operator= (const Sphere& sphere);

        void
        set_center(const Point3D& c);

        void
        set_center(const double x, const double y, const double z);

        void
        set_radius(const double r);
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 并把计算出来碰撞点和碰撞点法线方向，存入ShadeRec中。
        virtual bool
        hit(const Ray& ray, double& tmin, ShadeRec& s) const;
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 只判断是否相交。不计算碰撞点。
        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;

		virtual float
			pdf(const ShadeRec& sr);
    private:

        Point3D     center;               // center coordinates as a point
        double         radius;                // the radius 

        static const double kEpsilon;   // for shadows and secondary rays
};


inline void
Sphere::set_center(const Point3D& c) {
    center = c;
}

inline void
Sphere::set_center(const double x, const double y, const double z) {
    center.x = x;
    center.y = y;
    center.z = z;
}

inline void
Sphere::set_radius(const double r) {
    radius = r;
}

#endif
