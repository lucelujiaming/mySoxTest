#include <iostream>

int main() {

    // Image

    const int image_width = 256;
    const int image_height = 256;

    // Render
	// 这是第一个函数。目的为了介绍PPM文件格式。
	// 文件格式比较简单。就是第一行写P3，意思是颜色是ASCII码格式的。
	// 之后是分辨率，即行列宽，也就是多少行，和一行几个像素。
	// 之后就是一行一行的RGB像素值。从左到右，从上到下。
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) { // j 是从大到小的
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) { // i 是从小到大的
            auto r = double(i) / (image_width-1);     // 每一行 red 0~1
            auto g = double(j) / (image_height-1);    // 每一列 green 1~0
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::cerr << "\nDone.\n";
}

