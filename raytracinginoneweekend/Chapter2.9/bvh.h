#ifndef BVH_H
#define BVH_H

#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include <algorithm>


class bvh_node : public hittable {
    public:
        bvh_node();
		// 使用hittable_list初始化。
        bvh_node(const hittable_list& list, double time0, double time1)
            : bvh_node(list.objects, 0, list.objects.size(), time0, time1)
        {}
		// 使用hittable_list初始化。
        bvh_node(
            const std::vector<shared_ptr<hittable>>& src_objects,
            size_t start, size_t end, double time0, double time1);

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

		// 获得节点的包围盒。
        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
		// 左右节点
        shared_ptr<hittable> left;
        shared_ptr<hittable> right;
		// 自己的包围盒
        aabb box;
};

// 获得节点的包围盒。
bool bvh_node::bounding_box(double time0, double time1, aabb& output_box) const {
    output_box = box;
    return true;
}

// 判断射线是否在t区间
bool bvh_node::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
	// 首先判断光线是否击中自己，如果没有集击中，返回FALSE。
    if (!box.hit(r, t_min, t_max))
        return false;
	// 否则判断左子树。
    bool hit_left = left->hit(r, t_min, t_max, rec);
	// 之后判断右子树。
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

    return hit_left || hit_right;
}

// x轴、y轴和z轴上的qsort()比较函数
inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0,0, box_a) || !b->bounding_box(0,0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

// X轴上的qsort()比较函数
bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

// Y轴上的qsort()比较函数
bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

// Z轴上的qsort()比较函数
bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

// 对于所有的加速结构（包括BVH），最复杂的部分就是创建。
// 使用自顶向下创建BVH树的方式如下：
bvh_node::bvh_node(
    std::vector<shared_ptr<hittable>>& src_objects,
    size_t start, size_t end, double time0, double time1
) {
    auto objects = src_objects; // Create a modifiable array of the source scene objects

	// 1. 随机选择x,y,z轴中的一个
    int axis = random_int(0,2);
    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;
	// 3.2 如果等于1，则左右节点均指向该物体。最后将当前节点的aabb设置为包含左右子树的box。
    if (object_span == 1) {
        left = right = objects[start];
	// 3.1 如果左右子树的物体数量等于2，直接左右节点分别指向物体。
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start+1])) {
            left = objects[start];
            right = objects[start+1];
        } else {
            left = objects[start+1];
            right = objects[start];
        }
    } else {
		// 2. 对该轴正方向的顺序对物体排序，并分成前后两半；
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span/2;
		// 3. 前一半物体放左子树，后一半物体放右子树，继续对左右子树递归创建。
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;
	// 尝试获取左右子节点的BOX。
    if (  !left->bounding_box (time0, time1, box_left)
       || !right->bounding_box(time0, time1, box_right)
    )
	{
        std::cerr << "No bounding box in bvh_node constructor.\n";
	}
	// 计算2个子节点box的包围盒作为自己的包围盒
    box = surrounding_box(box_left, box_right);
}

#endif

