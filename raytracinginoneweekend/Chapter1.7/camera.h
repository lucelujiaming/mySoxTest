#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera() {
            auto aspect_ratio = 16.0 / 9.0;
            // 我们规定：
            //   1. 视口高度为2。
            auto viewport_height = 2.0;
            auto viewport_width = aspect_ratio * viewport_height;
            //   2. 投影点到投影平面的距离为1。称作focal length。
            auto focal_length = 1.0;

            // 视口位于原点
            origin = point3(0, 0, 0);
            // 两个沿着屏幕边的偏移量向量，用于移动光线的末端穿过屏幕。
            horizontal = vec3(viewport_width, 0.0, 0.0);
            vertical = vec3(0.0, viewport_height, 0.0);
            // 指向屏幕左下角的坐标。
            lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
        }

        ray get_ray(double u, double v) const {
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};
#endif

