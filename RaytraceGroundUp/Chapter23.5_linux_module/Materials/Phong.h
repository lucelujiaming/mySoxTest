#ifndef __PHONG__
#define __PHONG__

#include "Material.h"
#include "Lambertian.h"
#include "GlossySpecular.h"
#include "Phong.h"

//----------------------------------------------------------------------------- class Phong
// Phong������
class Phong: public Material {

    public:

        Phong(void);

        Phong(const Phong& m);

        virtual Material*
        clone(void) const;

        Phong& 
        operator= (const Phong& rhs);

        ~Phong(void);
        // ���û����ⷴ��ϵ��
        void
        set_ka(const float k);
        // ����������ϵ��
        void
        set_kd(const float k);
        // ���þ��淴��ϵ��
        void
        set_ks(const float ks);
        // ���ò�����ɫֵ������ΪRGBColor���͡�
        void
        set_cd(const RGBColor c);
        // ���ò�����ɫֵ������ΪRGB����ֵ��
        void
        set_cd(const float r, const float g, const float b);
        // ���ò�����ɫֵ������ΪRGB�Ҷ�ֵ��
        void
        set_cd(const float c);
        // ���þ���߹���ɫֵ������ΪRGBColor���͡�
        void
        set_cs(const RGBColor c);
        // ���þ���߹���ɫֵ������ΪRGB����ֵ��
        void
        set_cs(const float r, const float g, const float b);
        // ���þ���߹���ɫֵ������ΪRGB�Ҷ�ֵ��
        void
        set_cs(const float c);
        // ����Phongָ��
        void
        set_exp(const float _e);
        // ���㻷�����գ�������ȫ����Դ�Լ���ֱ����������պ;���߹ⷴ����ա�
        // �Ӷ����ز��ʵ���ɫ��
        virtual RGBColor
        shade(ShadeRec& sr);

        virtual RGBColor
        get_Le(ShadeRec& sr) const;

    private:
        Lambertian*        ambient_brdf;    // �����ⷴ��
        Lambertian*        diffuse_brdf;    // ������
        GlossySpecular*    specular_brdf;   // ����߹ⷴ��
};


// ---------------------------------------------------------------- set_ka
// this sets Phong::kd
// there is no Phong::ka data member because ambient reflection
// is diffuse reflection
// ���û����ⷴ��ϵ��
inline void
Phong::set_ka(const float ka) {
    ambient_brdf->set_kd(ka);
}


// ---------------------------------------------------------------- set_kd
// this also sets Phong::kd, but for a different Phong object
// ����������ϵ��
inline void
Phong::set_kd(const float kd) {
    diffuse_brdf->set_kd(kd);
}


// ---------------------------------------------------------------- set_ks
// ���þ��淴��ϵ��
inline void
Phong::set_ks(const float ks) {
    specular_brdf->set_ks(ks);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGBColor���͡�
inline void
Phong::set_cd(const RGBColor c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGB����ֵ��
inline void
Phong::set_cd(const float r, const float g, const float b) {
    ambient_brdf->set_cd(r, g, b);
    diffuse_brdf->set_cd(r, g, b);
    specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cd
// ���ò�����ɫֵ������ΪRGB�Ҷ�ֵ��
inline void
Phong::set_cd(const float c) {
    ambient_brdf->set_cd(c);
    diffuse_brdf->set_cd(c);
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGBColor���͡�
inline void
Phong::set_cs(const RGBColor c) {
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGB����ֵ��
inline void
Phong::set_cs(const float r, const float g, const float b) {
    specular_brdf->set_cs(r, g, b);
}


// ---------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGB�Ҷ�ֵ��
inline void
Phong::set_cs(const float c) {
    specular_brdf->set_cs(c);
}


// ---------------------------------------------------------------- set_exp
// ����Phongָ��
inline void
Phong::set_exp(const float _e) {
    specular_brdf->set_exp(_e);
}

#endif
