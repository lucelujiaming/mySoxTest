#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    // 增加一个标志位，表示是否从外面命中了物体。
    bool front_face;
    // 使用点积操作，判断是否从外面命中了物体。
    // 并且根据这个标志位，调整法线方向。
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class hittable {
    public:
        // 判断光线r在(t_min, t_max)中是否命中物体。
        // 如果命中，就返回真，并且结果放在rec中。
        // rec包括点，法线方向和t值。
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
