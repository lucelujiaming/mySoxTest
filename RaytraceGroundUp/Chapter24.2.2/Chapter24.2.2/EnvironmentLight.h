#ifndef __ENVIRONMENT_LIGHT__
#define __ENVIRONMENT_LIGHT__

#include "Light.h"
#include "Sampler.h"
#include "Material.h"
// 环境光照可视为一个无限大的球体包围当前场景并在各个方向上提供光照，常用于模拟室外场景。
// 同时，球体包含某一固定颜色值或随空间位置变化的自发光材质，
// 对于后者，颜色值基于物理天空盒模型或相应的天空图像。最终结果将呈现为包含软阴影的精美图像。
// Environment Light类和Ambient Occluder类具有某些共同特征。
class EnvironmentLight: public Light {

    public:

        EnvironmentLight(void);

        EnvironmentLight(const EnvironmentLight& al);

        virtual Light*
        clone(void) const;

        virtual
        ~EnvironmentLight(void);

        EnvironmentLight&
        operator= (const EnvironmentLight& rhs);

        void
        set_material(Material* material);

        // 返回各条阴影光线的方向。
        virtual Vector3D
        get_direction(ShadeRec& s);

        // 设置采样器对象sp
        void
        set_sampler(Sampler* sampler);

        // 返回入射辐射度
        virtual RGBColor
        L(ShadeRec& sr);
        // 测试阴影是否被某一个对象遮挡。
        virtual bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const;

        virtual float
        pdf(const ShadeRec& sr) const;

        virtual bool
        casts_shadows(void) const;
        // 开启或关闭阴影。
        virtual void
        set_shadows(bool _s);

    private:

        Sampler* sampler_ptr;
        Material* material_ptr;
        Vector3D u, v, w;
        Vector3D            wi;                // unit direction from hit point being shaded to sample point on light surface
};


// ---------------------------------------------------------------------- casts_shadows

inline bool
EnvironmentLight::casts_shadows() const {

    return Light::casts_shadows();
}


// ---------------------------------------------------------------------- set_shadows

inline void
EnvironmentLight::set_shadows(bool _s) {

    return Light::set_shadows(_s);
}


// --------------------------------------------------------------- set_material

inline void
EnvironmentLight::set_material(Material* mat_ptr) {
    material_ptr = mat_ptr;
}

#endif
