#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>
// 对一个像素写入RGB
// 参数：采样数 samples_per_pixel；
void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.
	// RGB 折算比例，scale = 1 / 采样数；
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;
    // Write the translated [0,255] value of each color component.
	// 调整 RGB，限制 RGB 在 0~0.999
	// 最后记得乘以256，这样才能得到 0~255 的RGB值。
    out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
        << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

#endif

