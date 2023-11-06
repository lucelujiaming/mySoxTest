#include "stdafx.h"
#include "Ambient.h"

// ---------------------------------------------------------------------- default constructor

Ambient::Ambient (void)
    :     Light(),
        ls(1.0),
        color(1.0)
{}


// ---------------------------------------------------------------------- copy constructor

Ambient::Ambient (const Ambient& a)
    :     Light(a),
        ls(a.ls),
        color(a.color)
{}


// ---------------------------------------------------------------------- clone

Light*
Ambient::clone(void) const {
    return (new Ambient(*this));
}


// ---------------------------------------------------------------------- assignment operator

Ambient&
Ambient::operator= (const Ambient& rhs) {
    if (this == &rhs)
        return (*this);

    Light::operator= (rhs);

    ls         = rhs.ls;
    color     = rhs.color;

    return (*this);
}


// ---------------------------------------------------------------------- destructor

Ambient::~Ambient (void) {}


// ---------------------------------------------------------------------- get_direction
// Ambient类不涉及方向问题。直接返回0向量。
Vector3D
Ambient::get_direction(ShadeRec& sr) {
    return (Vector3D(0.0));
}


// ---------------------------------------------------------------------- L
// 入射辐射度就是光源缩放系数，乘以光源颜色值。参见公式14.1。
RGBColor
Ambient::L(ShadeRec& sr) {
    return (ls * color);
}


// ---------------------------------------------------------------------- in_shadow
// 环境光会撞到所有物体。因此上永远返回真。
bool
Ambient::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    return true; // not implented yet

}

// ---------------------------------------------------------------------- G

float
Ambient::G(const ShadeRec& sr) const{
    return 5.5; //?
}


// ---------------------------------------------------------------------- pdf

float
Ambient::pdf(const ShadeRec& sr) const{

    return 5.5;//?
}
