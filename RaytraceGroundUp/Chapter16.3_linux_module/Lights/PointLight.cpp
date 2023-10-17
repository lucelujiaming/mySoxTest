#include "stdafx.h"
#include "PointLight.h"
#include "World.h"

// ---------------------------------------------------------------------- default constructor

PointLight::PointLight (void)
    :    Light(),
        ls(1.0),
        color(1.0),
        location()
{}


// ---------------------------------------------------------------------- copy constructor

PointLight::PointLight (const PointLight& a)
    :    Light(a),
        ls(a.ls),
        color(a.color),
        location(a.location)
{}


// ---------------------------------------------------------------------- clone

Light*
PointLight::clone(void) const {
    return (new PointLight(*this));
}


// ---------------------------------------------------------------------- assignment operator

PointLight&
PointLight::operator= (const PointLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator= (rhs);

    ls        = rhs.ls;
    color     = rhs.color;
    location = rhs.location;

    return (*this);
}


// ---------------------------------------------------------------------- destructor

PointLight::~PointLight (void) {}


// ---------------------------------------------------------------------- get_direction
// 返回一个背向碰撞点的单位向量。
Vector3D
PointLight::get_direction(ShadeRec& sr) {
    // 该类使用了存储于ShadeRec对象中的碰撞点坐标。
    return ((location - sr.hit_point).hat());
}


// ---------------------------------------------------------------------- L
// 和Ambient类一样，入射辐射度也是光源缩放系数，乘以光源颜色值。参见公式14.1。
RGBColor
PointLight::L(ShadeRec& sr) {
    return (ls * color);
}


// ---------------------------------------------------------------------- in_shadow
// 判断自己发出的光是否会撞到物体。只要撞到一个物体，就返回真。
bool
PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float t;
    int num_objects = sr.w.objects.size();
    float d = location.distance(ray.o);

    for(int j = 0; j < num_objects; j++)
        if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
            return true;

    return false;
}


// ---------------------------------------------------------------------- G

float
PointLight::G(const ShadeRec& sr) const{
    return 5.5; //?
}


// ---------------------------------------------------------------------- pdf

float
PointLight::pdf(const ShadeRec& sr) const{

    return 5.5;//?
}
