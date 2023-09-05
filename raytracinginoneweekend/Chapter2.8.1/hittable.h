#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "aabb.h"

class material;
// 一个结构体：记录光线与表面的碰撞
struct hit_record {
    point3 p;		// 点
    vec3 normal;	// 表面法向量
    shared_ptr<material> mat_ptr;
    double t;		// 光线里面的参数
    double u;		// 纹理贴图坐标u
    double v;		// 纹理贴图坐标v
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
		// 纯虚函数：必须由派生类进行复写。
        // 判断光线r在(t_min, t_max)中是否命中物体。
        // 如果命中，就返回真，并且结果放在rec中。rec包括点，法线方向和t值。
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        // 纯虚函数：必须由派生类进行复写。
		// 获得自己的包围盒。找到返回TRUE，找不到返回FALSE。
		virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

// 平移
class translate : public hittable {
    public:
		// 建构函数，传入两个参数。一个是用于碰撞检测的对象。一个平移距离向量。
        translate(shared_ptr<hittable> p, const vec3& displacement)
            : ptr(p), offset(displacement) {}

        // 判断光线r在(t_min, t_max)中是否命中物体。
        // 如果命中，就返回真，并且结果放在rec中。rec包括点，法线方向和t值。
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

		// 获得自己的包围盒。找到返回TRUE，找不到返回FALSE。
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        shared_ptr<hittable> ptr;	// 用于碰撞检测的对象。
        vec3 offset;				// 平移距离向量。
};

bool translate::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// 对于平移，我们可以对每个碰撞点进行移动也可以在计算碰撞点
	// 的时候把eye往反方向移动，进而，求取碰撞点，来实现平移。
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;
	// 如果发生碰撞，碰撞点为平移offset后的结果。
    rec.p += offset;
	// 同步调整法线方向。
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

	// 包围盒平移offset
    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}

// Y方向旋转
class rotate_y : public hittable {
    public:
		// 建构函数，传入两个参数。一个是用于碰撞检测的对象。一个旋转角度。
        rotate_y(shared_ptr<hittable> p, double angle);

        // 判断光线r在(t_min, t_max)中是否命中物体。
        // 如果命中，就返回真，并且结果放在rec中。rec包括点，法线方向和t值。
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

		// 获得自己的包围盒。找到返回TRUE，找不到返回FALSE。
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        shared_ptr<hittable> ptr;	// 用于碰撞检测的对象。
        double sin_theta;			// 旋转角度的正弦值
        double cos_theta;			// 旋转角度的余弦值
        bool hasbox;				// 是否找到包围盒。
        aabb bbox;					// 包围盒。
};

// 建构函数，传入两个参数。一个是用于碰撞检测的对象。一个旋转角度。
rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) {
	// 角度转弧度
    auto radians = degrees_to_radians(angle);
	// 算出旋转角度的正弦值
    sin_theta = sin(radians);
	// 算出旋转角度的余弦值
    cos_theta = cos(radians);
	// 获得包围盒，返回是否找到包围盒。
    hasbox = ptr->bounding_box(0, 1, bbox);
	// 初始化最小点坐标和最大点坐标。
    point3 min( infinity,  infinity,  infinity);
    point3 max(-infinity, -infinity, -infinity);
	
	// 这三层循环的目的就是：
	// 1. 让最内层的xyz三个变量依次取bbox.max()和bbox.min()对应的值。
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();
				// 2. 把坐标沿着Y轴旋转
                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;
				// 得到新的坐标。
                vec3 tester(newx, y, newz);
				// 使用新的坐标生成新的包围盒。
                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();
	// 物体正转theta相当于光线反转theta。这里给出的是对于光线反转theta。
    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());
	// 使用反转theta的光线判断是否相交。
    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;
	// 
	// 如果发生碰撞，
    auto p = rec.p;
    auto normal = rec.normal;
	// 碰撞点为转theta后的结果。
    p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];
	// 同步调整法线方向。
    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];
	// 返回新的碰撞点
    rec.p = p;
	// 返回新的碰撞点的法线方向
    rec.set_face_normal(rotated_r, normal);

    return true;
}


#endif
