#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
    public:
        camera(
            point3 lookfrom,		// 摄像机起点
            point3 lookat,			// 摄像机方向
            vec3   vup,				// 摄像机视口向上的方向。
            double vfov, 		    // 摄像机视场角。vertical field-of-view in degrees
            double aspect_ratio	    // 摄像机长宽比
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
			// 首先需要注意的是：
			//     vup, v, w位于一个平面上。
			// 摄像机的方向向量。这里做了归一化。
            auto w = unit_vector(lookfrom - lookat);
			// 求视口向上的方向和摄像机的方向向量的法向量。
			// 得到屏幕横向方向。且做了归一化。
            auto u = unit_vector(cross(vup, w));
			// 得到摄像机的方向向量和屏幕横向方向的法向量。
			// 得到屏幕的纵向方向。因为w和u都做了归一化，这里无须进行归一化。
            auto v = cross(w, u);

            // 视口位于摄像机起点
            origin = lookfrom;

            // 两个沿着屏幕边的偏移量向量，用于移动光线的末端穿过屏幕。
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            // 指向屏幕左下角的坐标。
            lower_left_corner = origin - horizontal/2 - vertical/2 - w;
        }

        ray get_ray(double s, double t) const {
            return ray(origin, lower_left_corner + s*horizontal + t*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};
#endif

