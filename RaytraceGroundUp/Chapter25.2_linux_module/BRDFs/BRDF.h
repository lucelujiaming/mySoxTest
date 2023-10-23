#ifndef __BRDF__
#define __BRDF__

// This file contains the declaration of the base class BRDF

#include <math.h>

#include "RGBColor.h"
#include "Vector3D.h"
#include "ShadeRec.h"

#include "Sampler.h"

// ��BRDF����ʵ���˻����Ĳ��ʷ�����ƺ� �����ʶ������������һ��BRDF��
// ��������пռ䲻����������
// ����BRDF���������3����Ա������f()������sample_f()�����Լ�rho()������
// ÿ������������һ��RGB��ɫֵ��
class BRDF {

    public:

        BRDF(void);

        BRDF(const BRDF& object);

        virtual BRDF*
        clone(void) const = 0;

        ~BRDF(void);
        // ����ڷ�������Լ�������-���������ģ����㣬���ؼ����������ɫֵ��
        // ���������delta()������������BRDF����
        virtual RGBColor
        f(const ShadeRec& sr, const Vector3D& wo, const Vector3D& wi) const;
        // ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
        // ����wi�����ڷ������յķ����������Ϊ������
        // ����˼�壬��ط���ͨ����BRDF��������õ���
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi) const;
        // ���ڼ��㷴����ߵķ��򣬲��ҷ��ؼ����������ɫֵ��
        // ʹ��Monte Calo����ʵ�֡�
        virtual RGBColor
        sample_f(const ShadeRec& sr, const Vector3D& wo, Vector3D& wi, float& pdf) const;
        // ����˫������ϵ���� 
        virtual RGBColor
        rho(const ShadeRec& sr, const Vector3D& wo) const;

    protected:

        BRDF&
        operator= (const BRDF& rhs);
        // ��Ϊ������߷��򽫹���BRDF��������õ���
        // �����BRDF�������һ������������ָ���ԭ��
        // ����BRDF�Լ�û��ʹ�������Ա���������ǰ�����������ʹ�á�
        Sampler* sampler_ptr;    // for use in sample_f
};

#endif
