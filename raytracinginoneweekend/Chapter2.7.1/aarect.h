#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"

#include "hittable.h"

// 一个位于 z = k 平面的平板物体。
class xy_rect : public hittable {
    public:
		// 默认建构函数。
        xy_rect() {}
		// X轴，Y轴的值和材质。
        xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, 
            shared_ptr<material> mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
			// 理论上面无厚薄，线无粗细，但是实际中面有厚薄，我们可以将厚度设置为0.0001，以此模拟理论厚度。
            output_box = aabb(point3(x0,y0, k-0.0001), point3(x1, y1, k+0.0001));
            return true;
        }

    public:
        shared_ptr<material> mp;
        double x0, x1, y0, y1, k;
};

// 假设长方形位于 z = k 平面，x和y边界如上，交点为P(x,y,k)，我们如何确定光线参数t？
// 已知光线方程为：
//     p(t) = eye + t * direction
// 则，z方向的方程为：
//     z(t) = eye.z + t * direction.z
// 那么，若满足z = k，则：
//     t = (k - eye.z) / direction.z ----- (1)
bool xy_rect::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// 使用公式(1)：t = (k - eye.z) / direction.z 计算光线参数t
    auto t = (k-r.origin().z()) / r.direction().z();
	// 检查计算出来的t是否在区间内。
    if (t < t_min || t > t_max)
        return false;
	// 使用计算出来的t球对应的x和y。
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
	// 检查计算出来的x和y是否在长方形区间内。
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
	// 反之，我们求取该点的纹理坐标，并记录其他碰撞点信息。
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mp;
    rec.p = r.at(t);
    return true;
}


#endif

