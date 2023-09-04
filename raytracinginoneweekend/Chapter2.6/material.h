#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "texture.h"

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
        // lambertian(const color& a) : albedo(a) {}
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

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
			// 传入虚化用时间参数。
            scattered = ray(rec.p, scatter_direction, r_in.time());
            // 衰减系数。传入纹理贴图坐标uv和命中终点坐标p
            attenuation = albedo->value(rec.u, rec.v, rec.p);
            return true;
        }

    public:
    	// 漫反射系数被改为纹理对象。
        // color albedo;
        shared_ptr<texture> albedo;
};

class metal : public material {
    public:
        // metal(const color& a) : albedo(a) {}
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // 计算反射光线  先单位化入射光线
            // 函数reflect: 镜面反射向量
            // 参数1.入射光单位向量（视线） 参数2.单位法向量
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
			// 使用模糊系数fuzz随机化反射方向。传入虚化用时间参数。
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere(), r_in.time()); 
            // 衰减系数
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        color albedo;
        // 模糊系数fuzz表示小球半径，为0~1，fuzz越大，越趋于漫反射；fuzz越小，越趋于镜面反射。
        double fuzz;
};

// 电介质材质
// 属性1.折射率
class dielectric : public material {
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        // 以下是电介质材质的“散射函数”，实际上就是计算了折射光向量【别忘了：光向量 = 起点 + 方向】
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            // *** 衰减系数 ***
            attenuation = color(1.0, 1.0, 1.0);
            // *** 计算折射率 ***
            // 光线击中外表面：1/ir    光线击中内表面：ir（假设外部是真空）
            // 首先需要根据光入射在内/外表面计算折射率
            double refraction_ratio = rec.front_face ? (1.0/ir) : ir;
			// 射入向量。
            vec3 unit_direction = unit_vector(r_in.direction());
			// 计算射入向量和物体表面法向量的点乘，得到两个矢量夹角的余弦值。
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
			// 进一步得到正弦值
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            
            // 判断此种材质的表面是否发生全反射。依据就是折射率乘以夹角正弦是否大于1。
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;   // 折射光方向，待计算

            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) 
            {
                // Must Reflect 全反射
                direction = reflect(unit_direction, rec.normal);
            }
            else{
                // Can Refract 发生折射
                direction = refract(unit_direction, rec.normal, refraction_ratio);
            }
            // 得到折射光或者是反射光的起点+方向
            scattered = ray(rec.p, direction, r_in.time());
            return true;
        }

    public:
        double ir; // Index of Refraction
     private:
	    // Schlick近似
		// 鉴于导体的菲涅耳方程较复杂，Schlick [2] 提供了一个近似的函数
		// 此函数只需要对材质提供光线垂直反射时的 值，
		// 就能近似地计算出不同入射角旳菲涅耳反射比。
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};


#endif
