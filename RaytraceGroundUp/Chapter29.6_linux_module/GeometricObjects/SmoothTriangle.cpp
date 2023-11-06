#include "stdafx.h"
#include "Constants.h"
#include "Maths.h"
#include "SmoothTriangle.h"

// ----------------------------------------------------------------  default constructor

SmoothTriangle::SmoothTriangle(void)
    :    GeometricObject(),
        v0(0.0),     
        v1(0,0,1), 
        v2(1,0,0),
        n0(0, 1, 0),       
        n1(0, 1, 0),
        n2(0, 1, 0)
{}


// ---------------------------------------------------------------- constructor

SmoothTriangle::SmoothTriangle(const Point3D& a, const Point3D& b, const Point3D& c)
    :    GeometricObject(),    
        v0(a),
        v1(b),
        v2(c),
        n0(0, 1, 0),  
        n1(0, 1, 0),
        n2(0, 1, 0)
{}


// ---------------------------------------------------------------- clone

SmoothTriangle* 
SmoothTriangle::clone(void) const {
    return (new SmoothTriangle (*this));
}


// ---------------------------------------------------------------- copy constructor

SmoothTriangle::SmoothTriangle (const SmoothTriangle& st)
    :    GeometricObject(st),
        v0(st.v1),
        v1(st.v1),
        v2(st.v2),
        n0(st.n0),
        n1(st.n1),
        n2(st.n2)
{}


// ---------------------------------------------------------------- assignment operator

SmoothTriangle& 
SmoothTriangle::operator= (const SmoothTriangle& rhs) {
    if (this == &rhs)
        return (*this);

    v0 = rhs.v0;
    v1 = rhs.v1;
    v2 = rhs.v2;
    n0 = rhs.n0;
    n1 = rhs.n1;
    n2 = rhs.n2;
    
    return (*this);
}


// ---------------------------------------------------------------- destructor

SmoothTriangle::~SmoothTriangle(void) {}


// ---------------------------------------------------------------- computeNormal
// 法线差值计算采用双线性组合方案，同时，还将涉及到碰撞点处的质心坐标
Normal 
SmoothTriangle::interpolate_normal(const float beta, const float gamma) const {    
    // 参见公式23.1
    Normal normal((1 - beta - gamma) * n0 + beta * n1 + gamma * n2);
    normal.normalize();

    return(normal);
}


//---------------------------------------------------------------- get_bounding_box

BBox
SmoothTriangle::get_bounding_box(void) {
    double delta = 0.0001; 
    
    return(BBox(min(min(v0.x, v1.x), v2.x) - delta, max(max(v0.x, v1.x), v2.x) + delta, 
                min(min(v0.y, v1.y), v2.y) - delta, max(max(v0.y, v1.y), v2.y) + delta, 
                min(min(v0.z, v1.z), v2.z) - delta, max(max(v0.z, v1.z), v2.z) + delta));
}



// ------------------------------------------------------------------------------ hit
// 对光线与三角形进行相交测试首先需要计算二者的碰撞点，
// 并进一步测试该碰撞点是否位于当前三角形内，
// 相关算法还将涉及到基于三角形顶点(a，b，c)的质心坐标(α，β，γ)。
bool 
SmoothTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {    
    // 参见公式19.10
    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;

    // 参见公式19.13
    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, s = e * j - f * i;

    double inv_denom  = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    // 参见公式19.13中求β的公式。
    double beta = e1 * inv_denom;

    if (beta < 0.0)
         return (false);

    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    // 参见公式19.13中求γ的公式。
    double gamma = e2 * inv_denom;

    // 参见公式19.6。
    if (gamma < 0.0 )
         return (false);

    // 参见公式19.6。
    if (beta + gamma > 1.0)
        return (false);

    // 参见公式19.13中求t的公式。
    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < kEpsilon)
        return (false);

    tmin                 = t;
    // 使用采用双线性组合方案。参见公式23.1
    sr.normal             = interpolate_normal(beta, gamma);
    // 根据求出来的t计算碰撞点。
    sr.local_hit_point     = ray.o + t * ray.d;

    return (true);
}

    

// ------------------------------------------------------------------------------ shadow_hit
// Hit function for shadow rays

bool                                                                                          
SmoothTriangle::shadow_hit(const Ray& ray, float& tmin) const {    
    // 参见公式19.10
    double a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x; 
    double e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
    double i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
        
    // 参见公式19.13
    double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    double q = g * i - e * k, s = e * j - f * i;

    double inv_denom  = 1.0 / (a * m + b * q + c * s);

    double e1 = d * m - b * n - c * p;
    // 参见公式19.13中求β的公式。
    double beta = e1 * inv_denom;

    if (beta < 0.0)
         return (false);

    double r = e * l - h * i;
    double e2 = a * n + d * q + c * r;
    // 参见公式19.13中求γ的公式。
    double gamma = e2 * inv_denom;

    // 参见公式19.6。
    if (gamma < 0.0 )
         return (false);
    
    // 参见公式19.6。
    if (beta + gamma > 1.0)
        return (false);
            
    // 参见公式19.13中求t的公式。
    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;

    if (t < kEpsilon)
        return (false);

    tmin                 = t;

    return (true);
}

