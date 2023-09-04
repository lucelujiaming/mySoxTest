#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>
#include "aabb.h"

using std::shared_ptr;
using std::make_shared;

// 一个vector：记录场景中所有可以与光线碰撞的物体
class hittable_list : public hittable {
    public:
        // 构造函数
		hittable_list() {}
		// 使用一个物体进行构造
        hittable_list(shared_ptr<hittable> object) { 
			// 使用一个物体进行构造
			add(object); 
		}
		// 清除所有物体
        void clear() { objects.clear(); }
		// 加入物体
        void add(shared_ptr<hittable> object) { 
			objects.push_back(object); 
		}
		// 光线与物体碰撞，对父类纯虚函数的复写
        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;
		// 获得自己的包围盒，对父类纯虚函数的复写
        virtual bool bounding_box(
            double _time0, double _time1, aabb& output_box) const override;

    public:
        std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    // 记录相交信息
	hit_record temp_rec;
    // 记录光线是否碰撞到物体
	bool hit_anything = false;
    // 记录目前光线最早碰到场景中的物体的 位置（参数t）
	auto closest_so_far = t_max;
	// 遍历物体vector
    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

// 获得自己的包围盒。找到返回TRUE，找不到返回FALSE。
bool hittable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;
	// 遍历物体vector
    for (const auto& object : objects) {
		// 只要有一个物体没有包围盒，就返回FALSE。
        if (!object->bounding_box(time0, time1, temp_box)) 
			return false;
		// 否则计算这些包围盒的并集包围盒。
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}

#endif
