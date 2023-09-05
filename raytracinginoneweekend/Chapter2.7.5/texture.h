#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"
#include "perlin.h"
#include "rtw_stb_image.h"

#include <iostream>

class texture {
    public:
        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
        solid_color() {}
        solid_color(color c) : color_value(c) {}

        solid_color(double red, double green, double blue)
          : solid_color(color(red,green,blue)) {}

        virtual color value(double u, double v, const vec3& p) const override {
            return color_value;
        }

    private:
        color color_value;
};

class checker_texture : public texture {
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
            : even(_even), odd(_odd) {}

        checker_texture(color c1, color c2)
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}

        virtual color value(double u, double v, const point3& p) const override {
			// 如果要实现棋盘纹理的话，就要让表面的反射率跟射线命中点的位置关联起来
			// sines为了区分球体纹理位置。
			// 利用正余弦函数的周期二值性来实现棋盘纹理。
            auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if (sines < 0)
                return odd->value(u, v, p);
            else
                return even->value(u, v, p);
        }

    public:
        shared_ptr<texture> odd;
        shared_ptr<texture> even;
};

// 柏林噪声纹理
class noise_texture : public texture {
    public:
        noise_texture() {}
		// 对参数施加一定的缩放比例，加速它的变化
        noise_texture(double sc) : scale(sc) {}

        virtual color value(double u, double v, const point3& p) const override {
			// 产生噪声值，并加入缩放比例。让噪声值位于-1~1。
			// 用turb函数来代替noise函数
            return color(1,1,1) * noise.turb(scale * p);
        }

    public:
        perlin noise;
        double scale;
};

// 图片纹理
class image_texture : public texture {
    public:
        const static int bytes_per_pixel = 3;

        image_texture()
          : data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

        image_texture(const char* filename) {
            auto components_per_pixel = bytes_per_pixel;
			// 采用stb图片读取处理库s
            data = stbi_load(
                filename, &width, &height, &components_per_pixel, components_per_pixel);

            if (!data) {
                std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
                width = height = 0;
            }
			// 得到每一行扫描线包含多少字节。
            bytes_per_scanline = bytes_per_pixel * width;
        }

        ~image_texture() {
            delete data;
        }
		// 根据uv坐标获得图片颜色值。
        virtual color value(double u, double v, const vec3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid.
            if (data == nullptr)
                return color(0,1,1);

            // Clamp input texture coordinates to [0,1] x [1,0]
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);  // Flip V to image coordinates
			// 使用uv坐标得到纹理贴图的坐标。
            auto i = static_cast<int>(u * width);
            auto j = static_cast<int>(v * height);

            // Clamp integer mapping, since actual coordinates should be less than 1.0
			// 大小限制一下。
            if (i >= width)  i = width-1;
            if (j >= height) j = height-1;

            const auto color_scale = 1.0 / 255.0;
			// 根据纹理贴图的坐标返回纹理贴图对应位置的颜色。
            auto pixel = data + j*bytes_per_scanline + i*bytes_per_pixel;
			// 每个元素的rgb是顺序存储的，所以每个像素点有三个值。
            return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
        }

    private:
		// 纹理图片数据指针。
		// 图片信息是有一维数组存储的，每个元素的rgb是顺序存储的，所以每个像素点有三个值。
        unsigned char *data;
		// 纹理图片尺寸
        int width, height;
		// 每一行扫描线包含多少字节。
        int bytes_per_scanline;
};

#endif
