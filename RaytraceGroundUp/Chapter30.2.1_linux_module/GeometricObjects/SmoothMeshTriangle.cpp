#include "stdafx.h"
#include "Constants.h"
#include "SmoothMeshTriangle.h"


// ----------------------------------------------------------------  default constructor

SmoothMeshTriangle::SmoothMeshTriangle(void)
    :     MeshTriangle()
{}


// ---------------------------------------------------------------- constructor

SmoothMeshTriangle::SmoothMeshTriangle (Mesh* _mesh_ptr, const int i0, const int i1, const int i2)
    :     MeshTriangle(_mesh_ptr, i0, i1, i2)
{}


// ---------------------------------------------------------------- clone

SmoothMeshTriangle*
SmoothMeshTriangle::clone (void) const {
    return (new SmoothMeshTriangle (*this));
}


// ---------------------------------------------------------------- copy constructor

SmoothMeshTriangle::SmoothMeshTriangle (const SmoothMeshTriangle& fmt)
    :    MeshTriangle(fmt)
{}


// ---------------------------------------------------------------- assignment operator

SmoothMeshTriangle&
SmoothMeshTriangle::operator= (const SmoothMeshTriangle& rhs) {
    if (this == &rhs)
        return (*this);

    MeshTriangle::operator= (rhs);

    return (*this);
}


// ---------------------------------------------------------------- destructor

SmoothMeshTriangle::~SmoothMeshTriangle(void) {}


// ---------------------------------------------------------------- interpolate_normal
// 法线差值计算采用双线性组合方案，同时，还将涉及到碰撞点处的质心坐标
Normal
SmoothMeshTriangle::interpolate_normal(const float beta, const float gamma) const {
    // 参见公式23.1。
    Normal normal((1 - beta - gamma) * mesh_ptr->normals[index0]
                        + beta * mesh_ptr->normals[index1]
                                + gamma * mesh_ptr->normals[index2]);
    normal.normalize();

    return(normal);
}

// ---------------------------------------------------------------- hit
// 对光线与三角形进行相交测试首先需要计算二者的碰撞点，
// 并进一步测试该碰撞点是否位于当前三角形内，
// 相关算法还将涉及到基于三角形顶点(a，b，c)的质心坐标(α，β，γ)。
bool
SmoothMeshTriangle::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    // 这3行代码用于从网格中提取顶点数据
    Point3D v0(mesh_ptr->vertices[index0]);
    Point3D v1(mesh_ptr->vertices[index1]);
    Point3D v2(mesh_ptr->vertices[index2]);
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
    // 使用双线性组合方案。参见公式23.1
    sr.normal             = interpolate_normal(beta, gamma);
    // 根据求出来的t计算碰撞点。
    sr.local_hit_point     = ray.o + t * ray.d;

    return (true);
}


// ---------------------------------------------------------------- shadow_hit

bool
SmoothMeshTriangle::shadow_hit(const Ray& ray, float& tmin) const {
    // 这3行代码用于从网格中提取顶点数据
    Point3D v0(mesh_ptr->vertices[index0]);
    Point3D v1(mesh_ptr->vertices[index1]);
    Point3D v2(mesh_ptr->vertices[index2]);
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

