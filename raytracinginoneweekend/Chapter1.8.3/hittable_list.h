#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

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

#endif
