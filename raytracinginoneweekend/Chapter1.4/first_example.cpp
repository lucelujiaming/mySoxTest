#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// 设置光的颜色
// 在把方向归一化以后，根据y的坐标高度线性混合白色和蓝色。
color ray_color(const ray& r) {
    // 将光线向量归一化
    vec3 unit_direction = unit_vector(r.direction());
    // 使用标准图形化技巧让t位于0和1之间。
    auto t = 0.5*(unit_direction.y() + 1.0);
    // 线性混合白色和蓝色。也就是线性插值。
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

// 整个main函数的注释来自于<4.2 Sending Rays Into the Scene>。
// 这个函数给出了向场景发射光线的过程。并使用了光路可逆原理。
// 光线追踪器通过像素发射光线，并计算沿着这些光线的方向上看到的颜色。
// 包括：
//   1. 计算出一条从眼睛到像素的光线。
//   2. 计算那些物体和这条光线相交。
//   2. 计算相交点的颜色。
int main() {

    // Image
	// 最终成像（图像）的宽高比
    const auto aspect_ratio = 16.0 / 9.0;
	// 图像宽度（像素个数）
    const int image_width = 400;
	// 图像高度（像素个数）
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    // 为此，我们需要设定一个视口。我们规定：
    //   1. 视口高度为2。
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    //   2. 投影点到投影平面的距离为1。称作focal length。
    auto focal_length = 1.0;

    // 进一步规定：
    //   1. 视点也就是眼睛位于原点。Y指向上，X指向右，则Z轴指向屏幕里面。
    //      也就是右手坐标系。
    auto origin = point3(0, 0, 0);
    // 两个沿着屏幕边的偏移量向量，用于移动光线的末端穿过屏幕。
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    // 屏幕的左下角坐标。
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";
    // 从左上角遍历整个屏幕
	// j：竖向移动的横向扫描线，j的值从大到小。（输出像素是从上到下，与j的值相反）
    for (int j = image_height-1; j >= 0; --j) {
		// 剩余扫描线数
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		// i：横向移动的竖向扫描线，i的值从小到大。（输出像素是从左到右，与i的值一致）
        for (int i = 0; i < image_width; ++i) {
            // 计算纹理坐标
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            // 根据纹理坐标计算出来光线方向，并根据该方向创建一根光线。
			// 光线起点：为原点(0,0)屏幕中心，也就是相机/人眼的位置（z=0）；
            // 光线方向：终点 - 起点，光线的终点为成像上的点（z=-focalLength）
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            // 对成像上的每一个像素发出光线，并使用光线向量的 y 分量设置像素颜色值。
			// 来计算这根光线的颜色。
            color pixel_color = ray_color(r);
            // 输出光线颜色。
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}


