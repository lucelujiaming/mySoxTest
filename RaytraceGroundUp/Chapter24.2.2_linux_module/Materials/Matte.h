#ifndef __MATTE__
#define __MATTE__

#include "Material.h"
#include "Lambertian.h"

//----------------------------------------------------------------------------- class Matte
// Matte���ʡ����ڻ����ⷴ��������䶼������ȫ������Lambertian����
class Matte: public Material {

    public:

        Matte(void);

        Matte(const Matte& m);

        virtual Material*
        clone(void) const;

        Matte& 
        operator= (const Matte& rhs);

        ~Matte(void);
        // ���û����ⷴ��ϵ��
        void
        set_ka(const float k);
        // ����������ϵ��
        void
        set_kd(const float k);
        // ���ò�����ɫֵ������ΪRGBColor���͡�
        void
        set_cd(const RGBColor c);
        // ���ò�����ɫֵ������ΪRGB����ֵ��
        void
        set_cd(const float r, const float g, const float b);
        // ���ò�����ɫֵ������ΪRGB�Ҷ�ֵ��
        void
        set_cd(const float c);
        // ���㻷�����գ�������ȫ����Դ�Լ���ֱ����������ա�
        // �Ӷ����ز��ʵ���ɫ��
        virtual RGBColor
        shade(ShadeRec& sr);
        // �����������
        virtual RGBColor
        area_light_shade(ShadeRec& sr);

        virtual RGBColor
        get_Le(ShadeRec& sr) const;

    private:
        Lambertian*        ambient_brdf;    // �����ⷴ��
        Lambertian*        diffuse_brdf;    // ������
};


// ---------------------------------------------------------------- set_ka
// this sets Lambertian::kd
// there is no Lambertian::ka data member because ambient reflection
// is diffuse reflection
// ���û����ⷴ��ϵ��
inline void
Matte::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Lambertian::kd, but for a different Lambertian object
// ����������ϵ��
inline void
Matte::set_kd (const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGBColor���͡�
inline void
Matte::set_cd(const RGBColor c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGB����ֵ��
inline void
Matte::set_cd(const float r, const float g, const float b) {
    ambient_brdf->set_cd(r, g, b);
    diffuse_brdf->set_cd(r, g, b);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGB�Ҷ�ֵ��
inline void
Matte::set_cd(const float c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
}

#endif
