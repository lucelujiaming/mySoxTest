#include "stdafx.h"

// this file contains the definition of the class GeometricObject 

#include "Constants.h"
// #include "Material.h"
#include "GeometricObject.h"

// ---------------------------------------------------------------------- default constructor

GeometricObject::GeometricObject(void)
    :     color(black),
//        material_ptr(NULL),
        shadows(true)
{}


// ---------------------------------------------------------------------- copy constructor

GeometricObject::GeometricObject (const GeometricObject& object)
    :     color(object.color),
        shadows(object.shadows) {
//    if(object.material_ptr)
//        material_ptr = object.material_ptr->clone(); 
//    else  
//        material_ptr = NULL;
}    


// ---------------------------------------------------------------------- assignment operator

GeometricObject&                                                        
GeometricObject::operator= (const GeometricObject& rhs) {
    if (this == &rhs)
        return (*this);
        
    color = rhs.color;
    
//    if (material_ptr) {
//        delete material_ptr;
//        material_ptr = NULL;
//    }

//    if (rhs.material_ptr)
//        material_ptr = rhs.material_ptr->clone();

    shadows = rhs.shadows;

    return (*this);
}


// ---------------------------------------------------------------------- destructor

GeometricObject::~GeometricObject (void) {    
//    if (material_ptr) {
//        delete material_ptr;
//        material_ptr = NULL;
//    }
}


// ---------------------------------------------------------------------- add_object
// required for Compound objects 

void                                                 
GeometricObject::add_object(GeometricObject* object_ptr) {}


// ----------------------------------------------------------------------- get_normal

Normal
GeometricObject::get_normal(void) const{
    return (Normal());
} 

// ----------------------------------------------------------------------- compute_normal

Normal
GeometricObject::get_normal(const Point3D& p) {
    return (Normal());
}  

// ----------------------------------------------------------------------- sample
// returns a sample point on the object for area light shading

Point3D 
GeometricObject::sample(void) {
    return (Point3D(0.0));
}


// ----------------------------------------------------------------------- pdf
// returns the probability density function for area light shading
        
float
GeometricObject::pdf(const ShadeRec& sr) {
    return (0.0);
}      

                                    

