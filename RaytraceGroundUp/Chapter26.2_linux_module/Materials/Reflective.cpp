#include "Reflective.h"

// ---------------------------------------------------------------- default constructor

Reflective::Reflective (void)
    :    Phong(),
        reflective_brdf(new PerfectSpecular)
{}


// ---------------------------------------------------------------- copy constructor

Reflective::Reflective(const Reflective& rm)
    :     Phong(rm) {
    
    if(rm.reflective_brdf)
        reflective_brdf = rm.reflective_brdf->clone(); 
    else  
        reflective_brdf = NULL;
}


// ---------------------------------------------------------------- assignment operator

Reflective& 
Reflective::operator= (const Reflective& rhs) {
    if (this == &rhs)
        return (*this);
        
    Phong::operator=(rhs);
    
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }

    if (rhs.reflective_brdf)
        reflective_brdf = rhs.reflective_brdf->clone();

    return (*this);
}

// ---------------------------------------------------------------- clone

Reflective*                                        
Reflective::clone(void) const {
    return (new Reflective(*this));
}    

// ---------------------------------------------------------------- destructor

Reflective::~Reflective(void) {
    if (reflective_brdf) {
        delete reflective_brdf;
        reflective_brdf = NULL;
    }
}

// ------------------------------------------------------------------------------------ shade 
// explained on page 501
RGBColor
Reflective::shade(ShadeRec& sr) {    
    // 直接光照。
    RGBColor L(Phong::shade(sr));  // direct illumination
    
    Vector3D wo = -sr.ray.d;
    Vector3D wi;    
    RGBColor fr = reflective_brdf->sample_f(sr, wo, wi); 
    // 反射光线的起点为碰撞点，方向为反射方向。
    Ray reflected_ray(sr.hit_point, wi); 
    // 累加反射次数。
    reflected_ray.depth = sr.depth + 1;
    
    // 调用sr.w.tracer_ptr->trace_ray完成递归。
    L += fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi);
                    
    return (L);
}


// ------------------------------------------------------------------------------------ path_shade
// 通过与Reflective::shade()函数进行比较可知，两个函数返回相同的反射辐射度表达式kr * cr * Li(p, ωi)。
// 且忽略了Reflective::shade中的直接光照项，但二者的实现方式稍有不同。
// 另外，两个函数均不包含cosθi项。
RGBColor
Reflective::path_shade(ShadeRec& sr) {
	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = reflective_brdf->sample_f(sr, wo, wi, pdf);
    // 反射光线的起点为碰撞点，方向为反射方向。
	Ray 		reflected_ray(sr.hit_point, wi);

    // 累加反射次数。
    // 调用sr.w.tracer_ptr->trace_ray完成递归。
	return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
}


// ------------------------------------------------------------------------------------ global_shade

RGBColor
Reflective::global_shade(ShadeRec& sr) {
	Vector3D 	wo = -sr.ray.d;
	Vector3D 	wi;
	float 		pdf;
	RGBColor 	fr = reflective_brdf->sample_f(sr, wo, wi, pdf);
	Ray 		reflected_ray(sr.hit_point, wi);
    // 各条光线均有depth>1。但该方案并未对(相机)直接可见对象上的光源反射进行渲染，
    // 其原因在于与光源碰撞的反射光线的depth值为1。
    // 该问题可通过进行简单地处理得以解决，例如下面的代码将光线的depth值赋值为2。
    // 同时， 这也反映出当前着色架构无法提供一种较为简单的方法以告知Emissive材质某些包含depth=1的光线将返回Le。
    // 然而，这并不是什么重要问题，读者不必纠结于此类问题。
	if (sr.depth == 0) {
        return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 2) * (sr.normal * wi) / pdf);
	}
	else {
        return (fr * sr.w.tracer_ptr->trace_ray(reflected_ray, sr.depth + 1) * (sr.normal * wi) / pdf);
	}
}

