#include "AmbientOccluder.h"
#include "ShadeRec.h"
#include "World.h"


// ---------------------------------------------------------------------- default constructor

AmbientOccluder::AmbientOccluder(void)
    :     Light(),
        ls(1.0),
        color(1.0),
        min_amount(0.25),
        u(0), v(0), w(0),
        sampler_ptr(NULL)
{}


// ---------------------------------------------------------------------- copy constructor

AmbientOccluder::AmbientOccluder(const AmbientOccluder& a)
    :     Light(a),
        ls(a.ls),
        color(a.color),
        min_amount(a.min_amount),
        u(a.u), v(a.v), w(a.w)
{
    // need to do a deep copy of the sampler
    if(a.sampler_ptr != NULL) {
        sampler_ptr = a.sampler_ptr->clone();
    }
}


// ---------------------------------------------------------------------- clone

Light*
AmbientOccluder::clone(void) const {
    return (new AmbientOccluder(*this));
}


// ---------------------------------------------------------------------- assignment operator

AmbientOccluder&
AmbientOccluder::operator= (const AmbientOccluder& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator= (rhs);

    ls         = rhs.ls;
    color     = rhs.color;
    min_amount = rhs.min_amount;
    u = rhs.u; v = rhs.v; w = rhs.w;

    // need to do a deep copy of the sampler
    if(rhs.sampler_ptr != NULL) {
        sampler_ptr = rhs.sampler_ptr->clone();
    }

    return (*this);
}


// ---------------------------------------------------------------------- destructor

AmbientOccluder::~AmbientOccluder (void) {}


// ---------------------------------------------------------------------- get_direction

Vector3D
AmbientOccluder::get_direction(ShadeRec& sr) {
    Point3D sp = sampler_ptr->sample_hemisphere();
    // 参见公式17.4
    return (sp.x * u + sp.y * v + sp.z * w);
}    



// ---------------------------------------------------------------------- set_sampler

void
AmbientOccluder::set_sampler(Sampler* s_ptr) {
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    
    sampler_ptr = s_ptr;
    sampler_ptr->map_samples_to_hemisphere(1);
}


// ---------------------------------------------------------------------- L
// 计算入射辐射度
RGBColor
AmbientOccluder::L(ShadeRec& sr) {

    // 1. w直接使用法线方向。
    w = sr.normal;
    // 2. u和v位于法线平面。
    //    u等于向上的方向，这里就是Vector3D(0.0072, 1.0, 0.0034)，和w的叉乘。
    //    参见Chapter17.1结尾处中的公式：
    //           v = w × up / ||w × up||
    //    2.1. 计算w × up
    v = w ^ Vector3D(0.0072, 1.0, 0.0034); // jitter the up vector in case normal is vertical
    //    2.2. 做归一化，除以||w × up||。
    v.normalize();
    // 3. 参见Chapter17.1结尾处中的公式：
    //         u = v × w
    u = v ^ w;
    // 创建一个临时光线变量。
    Ray shadow_ray;
    // 使用碰撞点作为起点。
    shadow_ray.o = sr.hit_point;
    // 获得阴影光线的方向
    shadow_ray.d = get_direction(sr);
    // 测试阴影是否被某一个对象遮挡。
    if (in_shadow(shadow_ray, sr))
    {
        // 如果遮挡，就减弱入射辐射度。
        return (min_amount * ls * color);
    }
    else
    {
        // 否则，入射辐射度就是光源缩放系数，乘以光源颜色值。参见公式14.1。
        return (ls * color);
    }
}



// ---------------------------------------------------------------------- in_shadow
// 测试阴影是否被某一个对象遮挡。
bool
AmbientOccluder::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float     t;
    int     num_objects = sr.w.objects.size();
    
    for (int j = 0; j < num_objects; j++)
        if (sr.w.objects[j]->shadow_hit(ray, t))
            return (true);
    
    return (false);
}

// ---------------------------------------------------------------------- G

float
AmbientOccluder::G(const ShadeRec& sr) const{
    return 5.5; //?
}

// ---------------------------------------------------------------------- pdf

float
AmbientOccluder::pdf(const ShadeRec& sr) const{

    return 5.5;//?
}
