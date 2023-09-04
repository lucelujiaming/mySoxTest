#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"
#include "perlin.h"

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
		// 组成棋盘纹理的两个纹理对象。
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

#endif
