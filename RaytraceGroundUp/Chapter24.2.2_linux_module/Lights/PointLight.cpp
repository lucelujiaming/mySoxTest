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
// 多数情况下，无需针对场景中的全部对象执行阴影光线相交测试，
// 但目前为止，该函数仍然是着色计算中最为耗时的一步。
bool
PointLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float t;
    int num_objects = sr.w.objects.size();
    float d = location.distance(ray.o);

    // 遍历场景中的全部对象并调用各对象的shadow_hit() 函数，
    // 以检测阴影光线是否与位于着色碰撞点和光源位置之间的对象产生碰撞。 
    for(int j = 0; j < num_objects; j++)
    {
        if (sr.w.objects[j]->shadow_hit(ray, t) && t < d)
        {
            // 若产生碰撞，则该函数即刻退出。
            return true;
        }
    }
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
