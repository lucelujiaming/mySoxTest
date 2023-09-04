#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere() {}
        sphere(point3 cen, double r) : center(cen), radius(r) {};
		// 增加一个建构函数，初始化材质指针。
        sphere(point3 cen, double r, shared_ptr<material> m) : center(cen), radius(r), mat_ptr(m) {};
		// 关键字override表示该函数复写了一个纯虚函数
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;	// 材质指针。
};

// 表面是否与光线碰撞，且碰撞点范围合理。
// 有了前面的基础，这个函数就比较容易理解了。
// 首先用判别式判断光线和物体是否相交。
// 如果相交，填充hit_record结构体。
// t为方程的解。p为A + tb的结果。
// normal是p的法线方向。
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false;
    auto sqrtd = sqrt(discriminant);

	// 找到最近的交点，它应该在合理范围之内
    // Find the nearest root that lies in the acceptable range.
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    // rec.normal = (rec.p - center) / radius;
    vec3 outward_normal = (rec.p - center) / radius;
	// 为本次相交设置表面法向量
    rec.set_face_normal(r, outward_normal);
	// 记录材质指针
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
