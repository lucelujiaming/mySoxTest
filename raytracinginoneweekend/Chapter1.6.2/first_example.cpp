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
// 输入的参数：1.球心 2.半径 3.光线
double hit_sphere(const point3& center, double radius, const ray& r) {
	// 分子和分母都除以2得到的简化形式
    vec3 oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {   // 判别式
        return -1.0;          // 没有交点
    } else {
		// 有一个或两个交点，返回两个交点中离眼睛更近的点。
		// 这个逻辑其实是有问题的。但是因为现在场景中只有一个球。
		// 两个解取任意一个都是可以的。不影响显示。
		// 正确的逻辑是我们需要传入t的取值范围。根据t的取值范围决定判别式前面的符号。
		// 正确的逻辑参见CSphere::hit中的代码。
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    // 如果命中，
    if (t > 0.0) {
        // 球面像素颜色由法向量决定，来可视化法线方向。
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }
	// 背景色
    // 将光线向量归一化
    vec3 unit_direction = unit_vector(r.direction());
    // 使用标准图形化技巧让t位于0和1之间。
    t = 0.5*(unit_direction.y() + 1.0);
    // 线性混合白色和蓝色。也就是线性插值。
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

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
            auto u = double(i) / (image_width-1);   // u：成像上像素的 x 坐标
            auto v = double(j) / (image_height-1);  // v：成像上像素的 y 坐标
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


