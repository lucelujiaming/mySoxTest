#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include <limits>
#include <memory>
#include <random>
#include <cstdlib>


inline double vec3_random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double vec3_random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*vec3_random_double();
}



using std::sqrt;

class vec3 {
    public:
        vec3() : e{0,0,0} {}
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }

        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/=(const double t) {
            return *this *= 1/t;
        }

        double length() const {
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random() {
            return vec3(vec3_random_double(), vec3_random_double(), vec3_random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(vec3_random_double(min,max), vec3_random_double(min,max), vec3_random_double(min,max));
        }

    public:
        double e[3];
};

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// 1. 在单位球面内随机取方向（“如何在球面内取随机点”的方法是通用的）
vec3 random_in_unit_sphere() {
   while (true) {
       auto p = vec3::random(-1,1);
       if (p.length_squared() >= 1) continue;
       return p;
   }
}

// 2. 在单位球面上随机取方向（经典的Lambertian反射模型）
//    在球内随机取点 & 规范化
vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// 3. 在单位半球面内部随机取方向：（Shirley说的所谓 the initial hack）
// 对理想 Lambertian 漫反射的不正确近似。错误持续这么久的重要原因可能是：
//    1、很难从数学上证明这种概率分布不正确；
//    2、很难直观地解释为什么 cos(ϕ) 分布是可取的（以及它呈现的效果）
vec3 random_in_hemisphere(const vec3& normal) {
    // 还有另一种方法让出射方向均匀取值，
    // 那就是不让法向量参与计算，而是直接用随机的方向来作为出射方向
    vec3 in_unit_sphere = random_in_unit_sphere();
    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0) 
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

#endif

