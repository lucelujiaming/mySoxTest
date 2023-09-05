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

// 取一个位于(min, max)的随机值。
inline double vec3_random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*vec3_random_double();
}

using std::sqrt;

class vec3 {
    public:
		// ***** 函数 ***** //
		// 构造函数1
		// 构造函数初始值列表
        vec3() : e{0,0,0} {}   
		// 构造函数2
		// 带参数的构造函数
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}   
		// 获取分量x,y,z
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }
		// 取反
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		// 使用下标0,1,2获取分量
        double operator[](int i) const { return e[i]; }
		// 使用下标0,1,2获取分量的引用s
        double& operator[](int i) { return e[i]; }
		
		// 向量自增
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
			// 对 this 指针解引用，得到对象
            return *this;
        }
		// 向量自乘
        vec3& operator*=(const double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
		// 向量自除
        vec3& operator/=(const double t) {
			// 用前面定义的乘法来定义除法
            return *this *= 1/t;
        }
		// 获取向量长度
        double length() const {
            return sqrt(length_squared());
        }
		// 获取向量长度平方
        double length_squared() const {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline static vec3 random() {
            return vec3(vec3_random_double(), vec3_random_double(), vec3_random_double());
        }

        inline static vec3 random(double min, double max) {
            return vec3(vec3_random_double(min,max), vec3_random_double(min,max), vec3_random_double(min,max));
        }

        bool near_zero() const {
           // Return true if the vector is close to zero in all dimensions.
           const auto s = 1e-8;
           return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
       }

    public:
        double e[3];
};

// Type aliases for vec3  类型别名
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

// vec3 Utility Functions
// 打印向量
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
// 向量相加
inline vec3 operator+(const vec3 &u, const vec3 &v) {
	// 构造函数~
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
// 向量相减
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
// 向量乘法：分量分别相乘
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
// 向量数乘：数*向量
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}
// 向量数乘：向量*数
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}
// 向量除以数
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}
// 向量点乘【结果是一个数】
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}
// 向量叉乘【结果为向量，计算方法下面图片解释】
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
// 规范化的向量
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// 使用“拒绝法”找单位球以内的向量：
// 随机找一个点。让其长度小于1。找不到就继续找。找到就返回。
// 目的是为了了返回的向量加一个随机扰动。
vec3 random_in_unit_sphere() {
   while (true) {
	   // 生成一个 Vec3，每个分量为 -1~1 的随机值
       auto p = vec3::random(-1,1);
       if (p.length_squared() >= 1) continue;
       return p;
   }
}

vec3 random_unit_vector() {
    // random_in_unit_sphere在单位球中取的随机方向实际上是不均匀的，越靠近法线的概率越大
    //（直观的想象一下，取以交点（P）为原点的某条射线上的任意一点（S）最后返回的方向
    // 是相同的，而越靠近法线，这条射线在球内的长度就越长，点就越“多”，
    // 因此取到的概率也就越大）通过在球面上取点可以解决这种不均匀的问题
    return unit_vector(random_in_unit_sphere());
}

// 3. 在单位半球面内部随机取方向：（Shirley说的所谓 the initial hack）
// 对理想 Lambertian 漫反射的不正确近似。错误持续这么久的重要原因可能是：
//    1、很难从数学上证明这种概率分布不正确；
//    2、很难直观地解释为什么 cos(ϕ) 分布是可取的（以及它呈现的效果）
vec3 random_in_hemisphere(const vec3& normal) {
    // 还有另一种方法让出射方向均匀取值，
    // 那就是不让法向量参与计算，而是直接用随机的方向来作为出射方向。
	// 也就是对远离击中点的所有角度都有一个统一的散射方向，而不依赖于与法线的角度。
	// 许多最早的光线追踪论文都使用了这种漫反射方法
    vec3 in_unit_sphere = random_in_unit_sphere();
    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0) 
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

// 镜面反射向量
// 参数1.入射光单位向量（视线） 参数2.单位法向量
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

// 折射向量  【需要画图+推导】
// Snell's Law （仅用于各向同性介质构成的静止界面）
// 参数1.入射光  参数2.法向量  参数3.光在介质中的折射率n1/n2
vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    // 光从折射率较大的介质进入折射率较小的介质时可能发生全反射。判断条件就是看折射角有没有达到90度。
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// 使用“拒绝法”找单位球以内的向量：
// 随机找一个点。让其长度小于1。找不到就继续找。找到就返回。
// 目的是为了返回的向量加一个随机扰动。
vec3 random_in_unit_disk() {
    while (true) {
	   // 生成一个 Vec3，每个分量为 -1~1 的随机值
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}


#endif

