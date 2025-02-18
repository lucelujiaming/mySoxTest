#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <cstdlib>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants 常量

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions  常用函数
// 角度制转换为弧度制
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}
// 限制函数
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
// 随机数
inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}
// 随机数
inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max+1));
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif
