#include "EnvironmentLight.h"

// ---------------------------------------------------------------- default constructor

EnvironmentLight::EnvironmentLight(void)
    :     Light(),
        sampler_ptr(NULL),
        material_ptr(NULL),
        u(), v(), w(),
        wi()
{}    


// ---------------------------------------------------------------- copy constructor

EnvironmentLight::EnvironmentLight(const EnvironmentLight& el)
    :     Light(el),
        u(el.u), v(el.v), w(el.w),
        wi(el.wi)
{
    if(el.sampler_ptr)
        sampler_ptr = el.sampler_ptr->clone(); 
    else  sampler_ptr = NULL;

    if(el.material_ptr)
        material_ptr = el.material_ptr->clone(); 
    else  material_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Light*
EnvironmentLight::clone(void) const {
    return (new EnvironmentLight(*this));
}


// ---------------------------------------------------------------- destructor

EnvironmentLight::~EnvironmentLight(void) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }
}


// --------------------------------------------------------------- assignment operator

EnvironmentLight&
EnvironmentLight::operator= (const EnvironmentLight& rhs) {

    if (this == &rhs)
        return (*this);

    Light::operator=(rhs);

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    if (rhs.sampler_ptr)
        sampler_ptr = rhs.sampler_ptr->clone();

    if (material_ptr) {
        delete material_ptr;
        material_ptr = NULL;
    }

    if (rhs.material_ptr)
        material_ptr = rhs.material_ptr->clone();

    return (*this);
}


// --------------------------------------------------------------- get_direction
// 返回各条阴影光线的方向。
Vector3D
EnvironmentLight::get_direction(ShadeRec& sr) {
    // 首先构造(u，v，w)。
    // 因为材质的area_light_shade函数首先调用EnvironmentLight::get_direction函数，
    // 之后调用nvironmentLight::L函数，因此上，需要在get_direction中构建(u，v，w)。
    // 1. w直接使用法线方向。
    w = sr.normal;
    // 2. u和v位于法线平面。
    //    u等于向上的方向，这里就是Vector3D(0.0072, 1.0, 0.0034)，和w的叉乘。
    //    参见Chapter17.1结尾处中的公式：
    //           v = w × up / ||w × up||
    //    2.1. 计算w × up
    v = Vector3D(0.0034, 1, 0.0071) ^ w;
    //    2.2. 做归一化，除以||w × up||。
    v.normalize();
    // 3. 参见Chapter17.1结尾处中的公式：
    //         u = v × w
    u = v ^ w;
    // 返回存储于采样器对象中的下一个采样点，映射到半球体。
    // 因为，通常情况下，我们需要在局部坐标系中计算光线的方向，
    // 并在随后计算该光线在世界坐标系中的方向。
    Point3D sp = sampler_ptr->sample_hemisphere();
    // 这个采样点是位于碰撞点基向量u, v, w组成的坐标系中的。
    // 我们把这个采样点换算到世界坐标系中。
    // 参见公式2.13。
    // 也就是局部变量的相关分量投影至(u，v，w)上。
    // 参见公式17.4。
    wi = sp.x * u + sp.y * v + sp.z * w;

    return (wi);
}


// ---------------------------------------------------------------------- set_sampler
// Environment Light类和Ambient Occluder类具有某些共同特征。
// 首先，二者具有相同的set_sampler()函数，并将采样点按照余弦分布的方式映射至半球模型上。
void
EnvironmentLight::set_sampler(Sampler* s_ptr) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // 设置采样器对象sp(在build() 函数中加以构造)，
    sampler_ptr = s_ptr;
    // 随后以余弦分布(e=1)的方式将采样映射至半球模型之上。
    sampler_ptr->map_samples_to_hemisphere(1);
}


// --------------------------------------------------------------- L

RGBColor
EnvironmentLight::L(ShadeRec& sr) {
    return (material_ptr->get_Le(sr));
}


// ---------------------------------------------------------------------- in_shadow
// not sure if this is correct, copied it from poitn light...
// 为了进一步测试阴影光线是否被某一对象所阻挡， 可使用标准的in_shadow() 函数，
// 函数逻辑等同于有向光源的对应函数。
bool
EnvironmentLight::in_shadow(const Ray& ray, const ShadeRec& sr) const {

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
    
    return false; // can an environment light even be in shadow?
}


// ---------------------------------------------------------------- pdf
// The following function is not in the book.
// It uses Equation 18.6
float
EnvironmentLight::pdf(const ShadeRec& sr) const {

    return (sr.normal * wi * invPI);
}
