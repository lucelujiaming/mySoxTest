#ifndef __MATERIAL__
#define __MATERIAL__

#include "World.h"            // required for the shade function in all derived classes
#include "RGBColor.h"
#include "ShadeRec.h"

class Material {

    public:

        Material(void);

        Material(const Material& material);

        virtual Material*
        clone(void) const = 0;

        virtual
        ~Material(void);
        // 计算环境光照，并遍历全部光源以计算直接漫反射光照。
        // 从而返回材质的颜色
        virtual RGBColor
        shade(ShadeRec& sr);
        // 计算区域光照
        virtual RGBColor
        area_light_shade(ShadeRec& sr);

        virtual RGBColor
        path_shade(ShadeRec& sr);

        virtual RGBColor
        get_Le(ShadeRec& sr) const;

    protected:

        Material&
        operator= (const Material& rhs);
};

#endif
