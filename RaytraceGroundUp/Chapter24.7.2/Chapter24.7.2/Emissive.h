
#ifndef __EMISSIVE__
#define __EMISSIVE__

#include "Material.h"

//----------------------------------------------------------------------------- class Emissive
// 自发光材质。 仅讨论空间不变、各向同性的自发光材质。
// 同时，本章将假设自发光材质不会反射任何光线，
// 因而，当主光线或次级光线与自发光对象发生碰撞时，将不会产生其他光线。
class Emissive: public Material {

    public:
        Emissive(void);
        Emissive(const Emissive& m);
        virtual Material*      clone(void) const;
        Emissive&       operator= (const Emissive& rhs);
        ~Emissive(void);

        void       scale_radiance(const float _ls);

        void       set_ce(const float r, const float g, const float b);

        virtual RGBColor       get_Le(ShadeRec& sr) const;

        virtual RGBColor       shade(ShadeRec& sr);

        virtual RGBColor       area_light_shade(ShadeRec& sr);

    private:
        // 缩放系数ls
        float ls;        // radiance scaling facor
        // 以及自发光颜色值ce
        RGBColor ce;    // color
};


// ---------------------------------------------------------------- scale_radiance

inline void
Emissive::scale_radiance(const float _ls){
    ls = _ls;
}


// ---------------------------------------------------------------- set_ce

inline void
Emissive::set_ce(const float r, const float g, const float b){
    ce.r = r; ce.g = g; ce.b = b;
}

#endif
