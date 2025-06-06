#include "AreaLight.h"

// ---------------------------------------------------------------- default constructor

AreaLight::AreaLight(void)
    :     Light(),
        object_ptr(NULL),
        material_ptr(NULL)
{}    


// ---------------------------------------------------------------- copy constructor

AreaLight::AreaLight(const AreaLight& al)
    :     Light(al) {

    if(al.object_ptr)
        object_ptr = al.object_ptr->clone(); 
    else  object_ptr = NULL;

    if(al.material_ptr)
        material_ptr = al.material_ptr->clone(); 
    else  material_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Light*
AreaLight::clone(void) const {
    return (new AreaLight(*this));
}


// ---------------------------------------------------------------- destructor

AreaLight::~AreaLight(void) {

    if (object_ptr) {
        delete object_ptr;
        object_ptr = NULL;
    }

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }
}


// --------------------------------------------------------------- assignment operator

AreaLight&
AreaLight::operator= (const AreaLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator=(rhs);

    if (object_ptr) {
        delete object_ptr;
        object_ptr = NULL;
    }

    if (rhs.object_ptr)
        object_ptr = rhs.object_ptr->clone();

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }

    if (rhs.material_ptr)
        material_ptr = rhs.material_ptr->clone();

    return (*this);
}


// --------------------------------------------------------------- get_direction

Vector3D
AreaLight::get_direction(ShadeRec& sr) {

    sample_point = object_ptr->sample();    // used in the G function
    light_normal = object_ptr->get_normal(sample_point); 
    wi = sample_point - sr.hit_point;        // used in the G function
    wi.normalize();

    return (wi);
}


// --------------------------------------------------------------- L

RGBColor
AreaLight::L(ShadeRec& sr) {

    float ndotd = -light_normal * wi;

    if (ndotd > 0.0)
        return (material_ptr->get_Le(sr));
    else
        return (black);
}


// ---------------------------------------------------------------- in_shadow    

bool
AreaLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float t;
    int num_objects = sr.w.objects.size();
    float ts = (sample_point - ray.o) * ray.d;

    for (int j = 0; j < num_objects; j++)
        if (sr.w.objects[j]->shadow_hit(ray, t) && t < ts)
            return (true); 

    return (false);
}


// ---------------------------------------------------------------- G
// G is part of the geometric factor
// explained on page 337
float
AreaLight::G(const ShadeRec& sr) const {

    float ndotd = -light_normal * wi;
    float d2     = sample_point.d_squared(sr.hit_point);

    return (ndotd / d2);
}


// ---------------------------------------------------------------- pdf

float
AreaLight::pdf(const ShadeRec& sr) const {

    return (object_ptr->pdf(sr));
}
