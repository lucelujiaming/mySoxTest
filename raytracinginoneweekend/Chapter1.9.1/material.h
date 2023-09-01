#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

struct hit_record;

class material {
    public:
    	// // 纯虚函数  是否散射    参数：1、入射光 2、碰撞记录 3、散射光 4、散射衰减
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(a) {}
	// 该材质是否散射  并计算：1、碰撞记录 2、散射光线 3、衰减系数
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // 设置散射方向
            // 注意这里Hit_record为前向声明，应#include它的定义所在的头文件，否则无法访问rec.normal
            auto scatter_direction = rec.normal + random_unit_vector();

            // Catch degenerate scatter direction
            // 如果散射方向接近于(0,0,0)，则设置为normal 
            // 【rec.normal + random_unit_vector()可能取0，后面可能出问题】
            if (scatter_direction.near_zero())
                scatter_direction = rec.normal;
            // *** 计算散射光 ***
            scattered = ray(rec.p, scatter_direction);
            // 衰减系数
            attenuation = albedo;
            return true;
        }

    public:
    	// 漫反射系数
        color albedo;
};

class metal : public material {
    public:
        metal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // 计算反射光线  先单位化入射光线
            // 函数reflect: 镜面反射向量
            // 参数1.入射光单位向量（视线） 参数2.单位法向量
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            // *** 计算散射光 ***
            scattered = ray(rec.p, reflected);
            // 衰减系数
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
};

#endif
