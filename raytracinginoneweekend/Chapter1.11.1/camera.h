#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(
            double vfov, 		// 摄像机视场角。vertical field-of-view in degrees
            double aspect_ratio	// 摄像机长宽比
        ) {
			// 角度转弧度。
            auto theta = degrees_to_radians(vfov);
			// we made h a ratio to that distance.
            auto h = tan(theta/2);
            // 我们规定：
            //   1. 视口高度为2。（相机视口很小的）
			//      但是需要乘以h。
            auto viewport_height = 2.0 * h;
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
			// 光线起点：为原点(0,0)屏幕中心，也就是相机/人眼的位置（z = 0）；
			// 光线方向：像素点坐标 - 相机坐标
            return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};
#endif

