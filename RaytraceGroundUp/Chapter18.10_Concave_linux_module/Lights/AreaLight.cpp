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
// 函数不仅返回着色点与采样点之间的单位方向向量ωi，
// 还将存储采样点、采样点处的法线以及wi至成员变量中，
// 因为函数n_shadow()、L()和G()同样需要这些数据。
Vector3D
AreaLight::get_direction(ShadeRec& sr) {
    // 1. 存储采样点。
    sample_point = object_ptr->sample();    // used in the G function
    // 2. 存储采样点处的法线。
    //    几何对象的get_normal()函数将被调用。
    light_normal = object_ptr->get_normal(sample_point); 
    // 计算着色点与采样点之间的方向向量。
    wi = sample_point - sr.hit_point;        // used in the G function
    // 单位化方向向量。
    wi.normalize();
    // 3. 返回着色点与采样点之间的单位方向向量ωi。
    return (wi);
}


// --------------------------------------------------------------- L
// 在返回材质的发射辐射度之前将检测光线是否与位于法线同一侧的表面产生碰撞。
RGBColor
AreaLight::L(ShadeRec& sr) {
    // 法线方向
    float ndotd = -light_normal * wi;
    // 同侧
    if (ndotd > 0.0)
    {
        // 返回材质的发射辐射度
        return (material_ptr->get_Le(sr));
    }
    // 不同侧
    else
    {
        return (black);
    }
}


// ---------------------------------------------------------------- in_shadow    
// 检测阴影光线是否与碰撞点和采样点之间的对象产生碰撞。
bool
AreaLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float t;
    int num_objects = sr.w.objects.size();
    // 光线上碰撞点对应时间点。
    float ts = (sample_point - ray.o) * ray.d;

    for (int j = 0; j < num_objects; j++)
    {
        // 如果有一个对象和光线产生碰撞，且碰撞时间点在sample_point对应的时间点之前，
        // 说明这个对象位于碰撞点和采样点之间。
        // 也就是说，光线与碰撞点和采样点之间的对象产生碰撞。
        if (sr.w.objects[j]->shadow_hit(ray, t) && t < ts)
        {
            return (true); 
        }
    }

    return (false);
}


// ---------------------------------------------------------------- G
// G is part of the geometric factor
// explained on page 337
// 计算余弦项cosθ并除以d^2。
float
AreaLight::G(const ShadeRec& sr) const {

    float ndotd = -light_normal * wi;
    float d2     = sample_point.d_squared(sr.hit_point);

    return (ndotd / d2);
}


// ---------------------------------------------------------------- pdf
// 简单地调用当前对象的pdf()函数。
// 对于均匀光源来说，pdf为表面积的倒数。尤其是平面光源。
float
AreaLight::pdf(const ShadeRec& sr) const {
    return (object_ptr->pdf(sr));
}
