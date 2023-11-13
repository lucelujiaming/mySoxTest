#include "stdafx.h"
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
// 返回各条阴影光线的方向。
Vector3D
AmbientOccluder::get_direction(ShadeRec& sr) {
    Point3D sp = sampler_ptr->sample_hemisphere();
    // 该函数只是简单地将局部变量的相关分量投影至(u，v，w)上。
    // 参见公式17.4
    return (sp.x * u + sp.y * v + sp.z * w);
}    

// ---------------------------------------------------------------------- set_sampler
// 设置采样器对象sp
void
AmbientOccluder::set_sampler(Sampler* s_ptr) {
    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // 设置采样器对象sp(在build() 函数中加以构造)，
    sampler_ptr = s_ptr;
    // 随后以余弦分布(e=1)的方式将采样映射至半球模型之上。
    sampler_ptr->map_samples_to_hemisphere(1);
}


// ---------------------------------------------------------------------- L
// 计算入射辐射度。
// 显示了正交基向量的构建过程以及get_direction() 函数和in_shadow() 函数的调用方式。
RGBColor
AmbientOccluder::L(ShadeRec& sr) {
    // 首先构造(u，v，w)。
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
// 为了进一步测试阴影光线是否被某一对象所阻挡， 可使用标准的in_shadow() 函数，
// 函数逻辑等同于有向光源的对应函数。
bool
AmbientOccluder::in_shadow(const Ray& ray, const ShadeRec& sr) const {

    float     t;
    int     num_objects = sr.w.objects.size();
    
    // 遍历场景中的全部对象并调用各对象的shadow_hit() 函数，
    // 以检测阴影光线是否与位于着色碰撞点和光源位置之间的对象产生碰撞。 
    for (int j = 0; j < num_objects; j++)
    {
        if (sr.w.objects[j]->shadow_hit(ray, t))
        {
            // 若产生碰撞，则该函数即刻退出。
            return (true);
        }
    }
    
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
