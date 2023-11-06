#include "stdafx.h"
#include "Emissive.h"

// ---------------------------------------------------------------- default constructor

Emissive::Emissive(void)
    :    Material(),
        ls(40.0),
        ce(white)
{}


// ---------------------------------------------------------------- copy constructor

Emissive::Emissive(const Emissive& m)
    :     Material(m),
        ls(m.ls),
        ce(m.ce)
{}


// ---------------------------------------------------------------- clone

Material*
Emissive::clone(void) const {
    return (new Emissive(*this));
}


// ---------------------------------------------------------------- assignment operator

Emissive&
Emissive::operator= (const Emissive& rhs) {

    if (this == &rhs)
        return (*this);

    Material::operator=(rhs);

    return (*this);

}


// ---------------------------------------------------------------- destructor

Emissive::~Emissive(void) {

}


// ---------------------------------------------------------------- shade
// shade()函数使得自发光材质可与其他类型的光线跟踪器结合使用，
// 例如Ray Cast跟踪器和Whitted跟踪器。
RGBColor
Emissive::shade(ShadeRec& sr) {
    // 另外，仅当入射光线与对象表面法线位于同一侧时，函数均返回Le=ls * ce。
    if (-sr.normal * sr.ray.d > 0.0)
        return (ls * ce);
    // 否则， 函数将返回黑色值。
    else
        return black;
}

// ---------------------------------------------------------------- area_light_shade
// explained on page 332
// 这里需要强调的一点是，当主光线或次级光线的最近碰撞点位于区域光线表面上时，
// Area Lighting::trace_ray() 将调用该函数，存储于sr中的法线表示当前碰撞点上的法线。
// 因而，这属于区域光照的直接渲染范畴，而非其他表面上的直接光照范畴(由Area Light类处理)。
RGBColor
Emissive::area_light_shade(ShadeRec& sr) {
    // 另外，仅当入射光线与对象表面法线位于同一侧时，函数均返回Le=ls * ce。
    if (-sr.normal * sr.ray.d > 0.0)
    {
        return (ls * ce);
    }
    // 否则， 函数将返回黑色值。
    else
    {
        return black;
    }
}

// ---------------------------------------------------------------- path_shade

RGBColor
Emissive::path_shade(ShadeRec& sr) {
    // 另外，仅当入射光线与对象表面法线位于同一侧时，函数均返回Le=ls * ce。
    if (-sr.normal * sr.ray.d > 0.0)
    {
        return (ls * ce);
    }
    // 否则， 函数将返回黑色值。
    else
    {
        return (black);
    }
}


// ---------------------------------------------------------------- global_shade

RGBColor
Emissive::global_shade(ShadeRec& sr) {
    if (sr.depth == 1) {
        // 当光线的depth值为1时，将返回黑色值。
        return (black);
    }
    // 另外，仅当入射光线与对象表面法线位于同一侧时，函数均返回Le=ls * ce。
    if (-sr.normal * sr.ray.d > 0.0)
    {
        return (ls * ce);
    }
    // 否则， 函数将返回黑色值。
    else
    {
        return (black);
    }
}


// ---------------------------------------------------------------- get_Le
// 函数Emissive::get_Le()也将返回发射辐射度ls * ce，
// 并由区域光源中的L()函数(参见程序清单18.10)负责调用。
RGBColor
Emissive::get_Le(ShadeRec& sr) const {
    // 另外，仅当入射光线与对象表面法线位于同一侧时，函数均返回Le=ls * ce。
    if (-sr.normal * sr.ray.d > 0.0)
        return (ls * ce);
    // 否则， 函数将返回黑色值。
    else
        return black;
}

