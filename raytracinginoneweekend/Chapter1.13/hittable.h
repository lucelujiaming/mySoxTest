#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"

class material;
// 一个结构体：记录光线与表面的碰撞
struct hit_record {
    point3 p;		// 点
    vec3 normal;	// 表面法向量
    shared_ptr<material> mat_ptr;
    double t;		// 光线里面的参数
    // 增加一个标志位，表示是否从外面命中了物体。
    bool front_face;
	// 设置表面法向量：判断光线与之碰撞的表面是内表面还是外表面，然后得到表面法向量。
    // 使用点积操作，判断是否从外面命中了物体。
    // 并且根据这个标志位，调整法线方向。
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
		// 内积 < 0：光线打在外表面，表面法向量 = 向外的法向量
		// 内积 > 0：光线打在内表面，表面法向量 = -向外的法向量
        normal = front_face ? outward_normal :-outward_normal;
    }
};

// 表示“可与光线碰撞的东西”，是一个虚基类，没有实际意义。
// 后面有Sphere类继承了它，以及Hittable_list继承了它。
class hittable {
    public:
		// 纯虚函数：必须由派生类进行复写
        // 判断光线r在(t_min, t_max)中是否命中物体。
        // 如果命中，就返回真，并且结果放在rec中。
        // rec包括点，法线方向和t值。
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
