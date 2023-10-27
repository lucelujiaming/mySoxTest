#ifndef SV_LAMBERTIAN_H
#define SV_LAMBERTIAN_H

#include "BRDF.h"
#include "ConstantColor.h"
// 可将纹理整合至基于空间变化的BRDF的相关材质中，其属性与位置相关，
// 因而可通过存储纹理而非RGB颜色值加以实现。
// 这里，BRDF和材质的名称以字母SV_开始。
class SV_Lambertian: public BRDF {
	public:

		SV_Lambertian(void);

		SV_Lambertian(const SV_Lambertian& lamb);

		virtual SV_Lambertian*
		clone(void) const;

		~SV_Lambertian(void);

		SV_Lambertian&
		operator= (const SV_Lambertian& rhs);
        // 针对于反射材质以及漫反射-漫反射光线模拟计算，返回计算出来的颜色值。
        // 如果不包含delta()函数，将返回BRDF自身。
		virtual RGBColor
		f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

        // 返回双半球反射系数。 
		virtual RGBColor
		sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;

		virtual RGBColor
		rho(const ShadeRec& sr, const Vector3D& wo) const;
        // 设置环境光反射系数
		void
		set_ka(const float ka);
        // 设置漫反射系数
		void
		set_kd(const float kd);

		void
        // 设置漫反射颜色值。
        // 通过纹理指针存储了颜色值
		set_cd(Texture* t_ptr);

		void
		set_sampler(Sampler* sp);   			// any type of sampling

	private:

        float        kd;    // 漫反射系数：diffuse reflection coefficient
		Texture* 	cd;     // 漫反射颜色值变成纹理指针
		Sampler*	sampler_ptr;    // for use in sample_f
};




// -------------------------------------------------------------- set_ka

inline void
SV_Lambertian::set_ka(const float k) {
	kd = k;
}



// -------------------------------------------------------------- set_kd

inline void
SV_Lambertian::set_kd(const float k) {
	kd = k;
}


// -------------------------------------------------------------- set_cd

inline void
SV_Lambertian::set_cd(Texture* t_ptr) {
	cd = t_ptr;
}

#endif // SV_LAMBERTIAN_H
