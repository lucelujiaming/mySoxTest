#include "stdafx.h"
#include "Light.h"
#include "Constants.h"

// ---------------------------------------------------------------------- default constructor

Light::Light(void)
        // 初始状态下，Light默认将shadows设置为true。
        : shadows(true)
{}


// ---------------------------------------------------------------------- copy constructor

Light::Light(const Light& ls)
    :    shadows(ls.shadows)
{}


// ---------------------------------------------------------------------- assignment operator

Light&
Light::operator= (const Light& rhs) {

    if (this == &rhs)
        return (*this);

    shadows = rhs.shadows;

    return (*this);
}


// ---------------------------------------------------------------------- destructor

Light::~Light(void) {}


// ---------------------------------------------------------------------- L
// returns the radiance

RGBColor
Light::L(ShadeRec& s) {
    return (black);
}

