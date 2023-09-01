#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// 这一章引入球体。
// 首先球体的定义非常简单。
//     x * x + y * y + z * z = R * R
// 因此上，如果一个点(x, y, z)位于球面上，则：
//     x * x + y * y + z * z = R * R
// 如果一个点(x, y, z)位于球里面，则：
//     x * x + y * y + z * z < R * R
// 如果一个点(x, y, z)位于球外面，则：
//     x * x + y * y + z * z > R * R
// 如果球心位于(Cx, Cy, Cz)则：
//     (x - Cx) * (x - Cx) + (y - Cy) * (y - Cy) + (z - Cz) * (z - Cz) = R * R
// 下面写成向量格式。令C = (Cx, Cy, Cz)。空间中一个点P = (x, y, z)。
// 则可以写成：
//     (P - C) ● (P - C) = R * R
// 代入光线公式：
//      P(t)=A + tb
// 有：
//     (A + tb - C) ● (A + tb - C) = R * R
// 可以得到下面的一元二次方程。
// 		t * t * b * b + 2 * t * b ● (A - C) + (A - C) ● (A - C) - R * R = 0
// 这个一元二次方程是否有解。取决于判别式 b * b - 4 * a * c是否大于零。
// 如果判别式判别式大于零，就是有解。说明光线和这个球相交。
// 否则就是不相交。
// 这既是下面这个函数的逻辑。
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

// 如果命中球就返回红色
color ray_color(const ray& r) {
    if (hit_sphere(point3(0,0,-1), 0.5, r))
        return color(1, 0, 0);
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}


