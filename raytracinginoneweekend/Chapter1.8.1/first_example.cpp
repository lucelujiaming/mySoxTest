#include "rtweekend.h"


#include "camera.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    // 1. 超过一定递归次数，则返回黑色
    if (depth <= 0)
        return color(0,0,0);
    if (world.hit(r, 0, infinity, rec)) {
        // 2. 如果命中，给出反射方向。
        //    计算方法是：以交点法向量所指的坐标为原点，以单位长度为半径的球中随机取一个点S。
        //       S − P 就是我们找到的随机的反射方向
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        //    使用这个反射方向进行迭代运算i。
        //    0.5是漫反射系数，代表了材质的吸收量。
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth-1);
    }
    // 3. 若未击中物体，则返回环境光RGB。
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

// 整个主函数包含四个部分。逻辑如下：
//   1. Image：这个部分定义了一些基本参数。包括生成图片的大小。
//             每一个像素采样了几根光线。还有就是光线反射次数。
//   2. World：这个部分是一个对象列表。也就是一个std::vector<hittable>。
// 			   每一个对象是一个hittable对象。包含一个hit函数用于判断是否和光线相交。
//   3. Camera：顾名思义，这个部分定义了视口。
//   4. Render：有了前面的设定，我们就可以进行绘制了。
//              步骤就是
//              1. 计算每一个像素。
//              2. 通过像素内多次采样的的方式抗锯齿。
//              2. 对于每一次采样，需要根据光线反射次数，把每一个像素的光照采样累加起来。
//                 计算函数就是ray_color函数。
//              4. 之后输出第二步的计算结果。因为前面的计算结果是采样累加值。
//                 因此上，结果需要除以光线反射次数。
//   经过上面的分析可以知道，这个的计算量是非常大的。
//   首先就是一帧图像包含很多的像素点。而每一个像素点都需要做多次采样。
//   而每一次采样都需要使用当前采样光线判断是否和物体相交。
//   并且每一次相交计算都需要模拟计算迭代光线跳跃过程。
int main() {

    // Image

    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    // Camera
    camera cam;

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
   // 从左上角遍历整个屏幕
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            // 通过像素内多次采样的的方式抗锯齿，采样点坐标用前面的随机数获得。
            // 在main函数中只是简单的将采样结果相加。
            for (int s = 0; s < samples_per_pixel; ++s) {
                // 计算纹理坐标
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                // 根据纹理坐标计算出来光线方向，并根据该方向创建一根光线。
                ray r = cam.get_ray(u, v);
                // 累加这根光线多次采样得到的颜色。
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
       }
    }

    std::cerr << "\nDone.\n";
}
