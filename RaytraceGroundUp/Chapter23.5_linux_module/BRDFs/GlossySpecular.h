#ifndef __GLOSSYSPECULAR__
#define __GLOSSYSPECULAR__

#include "BRDF.h"
#include "Sampler.h"
// ���վ��淴��
class GlossySpecular: public BRDF {

    public:

        GlossySpecular(void);

        GlossySpecular(const GlossySpecular& gloss);

        virtual GlossySpecular*
        clone(void) const;

        ~GlossySpecular(void);

        GlossySpecular&
        operator= (const GlossySpecular& rhs);
        // ����ڷ������ģ����㣬���ؼ����������ɫֵ�� 
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // �ú�������ģ������䡣�μ�Chapter25.1��
        // ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
        // ʹ��Monte Calo����ʵ�֡�
        // ����wi�����ڷ������յķ����������Ϊ������
        // ����˼�壬��ط���ͨ����BRDF��������õ���
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
        // ����˫������ϵ���� 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;
        // ���þ��淴��ϵ��
        void
        set_ks(const float ks);
        // ����Phongָ��
        void
        set_exp(const float _e);
        // ���þ���߹���ɫֵ������ΪRGBColor���͡�
        void
        set_cs(const RGBColor& c);
        // ���þ���߹���ɫֵ������ΪRGB����ֵ��
        void
        set_cs(const float r, const float g, const float b);
        // ���þ���߹���ɫֵ������ΪRGB�Ҷ�ֵ��
        void
        set_cs(const float c);
        // ���ò�������Phongָ����
        void
        set_sampler(Sampler* sp, const float exp);                // any type of sampling
        // ����Phongָ�����������ò�����Ϊ���ض���������
        void
        set_samples(const int num_samples, const float exp);    // multi jittered sampling
        // ���÷��߷���δʵ�֡�
        void
        set_normal(const Normal& n);

    private:
        // ���淴��ϵ��
        float        ks;                // specular reflection coefficient
        // Phongָ��(P102)��
        // ��Ϊ�����������ı����ܶ�ֵd�뼫�Ǧ�֮��Ĺ�ϵΪ��
        //     d = cos(��)^exp
        // ���exp���ڿ��Ƽ��Ǧȱ��ʱ��dֵ���½��ٶȡ�
        // exp��ֵԽ���½����ٶ�ҲԽ�졣
        // ���������û�����ؿ��ƾ���߹�ĳߴ硣
        float        exp;               // specular exponent
        // ����߹���ɫֵ
        RGBColor     cs;                // specular color
        // �߹���ð�������� 
		// ��Ȼ�ڸ���BRDF���Ѿ������������Ա��������������������в����������Ա������
		// ���޷�����ͨ��������ʮ�ֲ��⡣
        Sampler*    sampler_ptr;    // for use in sample_f
};


// -------------------------------------------------------------- set_ks
// ���þ��淴��ϵ��
inline void
GlossySpecular::set_ks(const float k) {
    ks = k;
}

// -------------------------------------------------------------- set_exp
// ����Phongָ��
inline void
GlossySpecular::set_exp(const float e) {
    exp= e;
}


// -------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGBColor���͡�
inline void
GlossySpecular::set_cs(const RGBColor& c) {
    cs = c;
}


// ---------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGB����ֵ��
inline void
GlossySpecular::set_cs(const float r, const float g, const float b) {
    cs.r = r; cs.g = g; cs.b = b;
}


// ---------------------------------------------------------------- set_cs
// ���þ���߹���ɫֵ������ΪRGB�Ҷ�ֵ��
inline void
GlossySpecular::set_cs(const float c) {
    cs.r = c; cs.g = c; cs.b = c;
}

#endif
