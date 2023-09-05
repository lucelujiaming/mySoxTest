#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"

#include "aarect.h"
#include "hittable_list.h"

class box : public hittable  {
    public:
        box() {}
        box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

        // 判断光线r在(t_min, t_max)中是否命中物体。如果命中，就返回真，并且结果放在rec中。
        // rec包括点，法线方向和t值。
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

		// 获得自己的包围盒。找到返回TRUE，找不到返回FALSE。
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
        }

    public:
        point3 box_min;			// 最小点坐标
        point3 box_max;			// 最大点坐标
        hittable_list sides;	// 六个面
};

// 使用空间中的两个点创建一个盒子。也就是根据最小点坐标和最大点坐标构建六个面
box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
}

bool box::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}

#endif
