#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// 所有的光线追踪器都有一个光线类，和我们计算可以沿着这条光线看见什么颜色。
// 一根光线可以使用 P(t)=A+tb 表示。P表示起点，b表示方向，t为时间。
// 其中，t大于零。因此上这被称为half-line或者是ray。
class ray {
    public:
		// ***** 成员函数 ***** //
		// 默认构造函数
        ray() {}
		// 构造函数，指定起点和方向
		// 这里传入了一个时间参数，用于产生虚化效果。
		// 所谓的虚化其实就是摄像机在开关快门的这段时间里，物体有了移动。
		// 以此达到虚化的效果。
        ray(const point3& origin, const vec3& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time)
        {}
		// 获取起点
        point3 origin() const  { return orig; }
		// 获取方向
        vec3 direction() const { return dir; }
        double time() const    { return tm; }
		// 得到光线上的点
        point3 at(double t) const {
            return orig + t*dir;
        }

    public:
		// ***** 成员变量 ***** //
        point3 orig;		// 起点
        vec3 dir;		 	// 方向
        double tm;          // 虚化用时间参数
};

#endif


