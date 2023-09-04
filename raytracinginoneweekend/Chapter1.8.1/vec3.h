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


#endif

