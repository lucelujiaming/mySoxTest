#ifndef __LAMBERTIAN__
#define __LAMBERTIAN__

#include "BRDF.h"
// ������ȫ������
class Lambertian: public BRDF {

    public:

        Lambertian(void);

        Lambertian(const Lambertian& lamb);

        virtual Lambertian*
        clone(void) const;

        ~Lambertian(void);

        Lambertian&
        operator= (const Lambertian& rhs);
        // ����ڷ�������Լ�������-���������ģ����㣬���ؼ����������ɫֵ��
        // ���������delta()������������BRDF����
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;

        // ����˫������ϵ���� 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
        // ���û����ⷴ��ϵ��
        void
        set_ka(const float ka);
        // ����������ϵ��
        void
        set_kd(const float kd);
        // ������������ɫֵ������ΪRGBColor���͡�
        void
        set_cd(const RGBColor& c);
        // ������������ɫֵ������ΪRGB����ֵ��
        void
        set_cd(const float r, const float g, const float b);
        // ������������ɫֵ������ΪRGB�Ҷ�ֵ��
        void
        set_cd(const float c);

    private:

        float        kd;    // ������ϵ����diffuse reflection coefficient
        RGBColor     cd;    // ��������ɫֵ��diffuse color
};


// -------------------------------------------------------------- set_ka
// ���û����ⷴ��ϵ��
inline void
Lambertian::set_ka(const float k) {
    kd = k;
}


// -------------------------------------------------------------- set_kd
// ����������ϵ��
inline void
Lambertian::set_kd(const float k) {
    kd = k;
}


// -------------------------------------------------------------- set_cd
// ������������ɫֵ������ΪRGBColor���͡�
inline void
Lambertian::set_cd(const RGBColor& c) {
    cd = c;
}


// ---------------------------------------------------------------- set_cd
// ������������ɫֵ������ΪRGB����ֵ��
inline void
Lambertian::set_cd(const float r, const float g, const float b) {
    cd.r = r; cd.g = g; cd.b = b;
}


// ---------------------------------------------------------------- set_cd
// ������������ɫֵ������ΪRGB�Ҷ�ֵ��
inline void
Lambertian::set_cd(const float c) {
    cd.r = c; cd.g = c; cd.b = c;
}

#endif
