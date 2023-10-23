#include "stdafx.h"
#include "Constants.h"
#include "Triangle.h"
#include "Maths.h"

// ----------------------------------------------------------------  default constructor

Triangle::Triangle (void)
    :     GeometricObject(),
        v0(0, 0, 0), 
        v1(0,0,1), 
        v2(1,0,0),
        normal(0, 1, 0)
{}

// ---------------------------------------------------------------- constructor

Triangle::Triangle (const Point3D& a, const Point3D& b, const Point3D& c)
    :     GeometricObject(),
        v0(a),
        v1(b),
        v2(c)
{
    compute_normal();    
}


// ---------------------------------------------------------------- clone

Triangle* 
Triangle::clone(void) const {
    return (new Triangle(*this));
}


// ---------------------------------------------------------------- copy constructor

Triangle::Triangle (const Triangle& triangle)
    :    GeometricObject(triangle),
        v0(triangle.v0),
        v1(triangle.v1),
        v2(triangle.v2),
        normal(triangle.normal)
{}


// ---------------------------------------------------------------- assignment operator

Triangle& 
Triangle::operator= (const Triangle& rhs) {
    if (this == &rhs)
        return (*this);

    GeometricObject::operator=(rhs);

    v0         = rhs.v0;
    v1         = rhs.v1;
    v2         = rhs.v2;
    normal     = rhs.normal;
        
    return (*this);
}


// ---------------------------------------------------------------- destructor

Triangle::~Triangle (void) {}



// ---------------------------------------------------------------- compute_normal

void 
Triangle::compute_normal(void) {
    normal = (v1 - v0) ^ (v2 - v0);  
    normal.normalize();
}

// 计算各三角形的包围盒
BBox
Triangle::get_bounding_box(void) {
    double delta = 0.000001; 
    
    return (BBox(min(min(v0.x, v1.x), v2.x) - delta, max(max(v0.x, v1.x), v2.x) + delta, 
                 min(min(v0.y, v1.y), v2.y) - delta, max(max(v0.y, v1.y), v2.y) + delta, 
                 min(min(v0.z, v1.z), v2.z) - delta, max(max(v0.z, v1.z), v2.z) + delta));
}


// ------------------------------------------------------------------------------ hit
// 对光线与三角形进行相交测试首先需要计算二者的碰撞点，
// 并进一步测试该碰撞点是否位于当前三角形内，
// 相关算法还将涉及到基于三角形顶点(a，b，c)的质心坐标(α，β，γ)。
bool 
Triangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
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
    sr.normal             = normal; 
    // 根据求出来的t计算碰撞点。
    sr.local_hit_point     = ray.o + t * ray.d;    
    
    return (true);    
}          


// ----------------------------------------------------------------- shadow_hit

bool
Triangle::shadow_hit(const Ray& ray, float& tmin) const {
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


// ------------------------------------------------------------------------------ shadow_hit

bool                                                                                          
Triangle::shadow_hit(const Ray& ray, double& tmin) const {
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
    if (gamma < 0.0)
         return (false);
    
    // 参见公式19.6。
    if (beta + gamma > 1.0)
        return (false);
            
    // 参见公式19.13中求t的公式。
    double e3 = a * p - b * r + d * s;
    double t = e3 * inv_denom;
    
    if (t < kEpsilon)
        return (false);
                    
    tmin = t;
    
    return(true);    
}  

