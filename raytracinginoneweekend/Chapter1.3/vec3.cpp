#include "vec3.h"

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

