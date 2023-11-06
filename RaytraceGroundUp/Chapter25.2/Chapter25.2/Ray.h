#ifndef __RAY__
#define __RAY__

#include "Point3D.h"
#include "Vector3D.h"

class Ray {

    public:

        Point3D            o;              // origin                        
        Vector3D        d;                 // direction                    
        int                depth;            // ray depth, for reflections    
        Ray(void);                        //default constructor            
        Ray(const Point3D& origin,        
            const Vector3D& dir);        //constructor
        Ray(const Ray& ray);            //copy constructor
        Ray&
        operator= (const Ray& rhs);        //assignment operator

        ~Ray(void);                        //destructor
};

#endif
