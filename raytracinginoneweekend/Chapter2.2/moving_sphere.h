#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"


class moving_sphere : public hittable {
    public:
        moving_sphere() {}
        moving_sphere(
            point3 cen0, point3 cen1, double _time0, double _time1, double r, shared_ptr<material> m)
            : center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r), mat_ptr(m)
        {};

		// 关键字override表示该函数复写了一个纯虚函数
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 center(double time) const;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;	// 材质指针。
};

// 计算在time时刻球体的球心位置。
point3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

// 如果命中了，命中记录保存到rec
bool moving_sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
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
    auto outward_normal = (rec.p - center(r.time())) / radius;
	// 为本次相交设置表面法向量
    rec.set_face_normal(r, outward_normal);
	// 记录材质指针
    rec.mat_ptr = mat_ptr;

    return true;
}

#endif
