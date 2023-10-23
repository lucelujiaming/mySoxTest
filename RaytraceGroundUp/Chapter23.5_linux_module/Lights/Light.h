#ifndef __LIGHT__
#define __LIGHT__

#include "Vector3D.h"
#include "RGBColor.h"
#include "Ray.h"

class ShadeRec;

//-------------------------------------------------------------------- class Light

class Light {

    public:

        Light(void);

        Light(const Light& ls);

        Light&
        operator= (const Light& rhs);

        virtual Light*
        clone(void) const = 0;

        virtual
        ~Light(void);
        // ������ײ�㴦�Ĺ��߷���
        virtual Vector3D
        get_direction(ShadeRec& sr) = 0;
        // ������������
        virtual RGBColor
        L(ShadeRec& sr);

        virtual bool
        casts_shadows(void) const;

        virtual void
        set_shadows(bool val);

        virtual bool
        in_shadow(const Ray& ray, const ShadeRec& sr) const = 0;

        virtual float
        G(const ShadeRec& sr) const;

        virtual float
        pdf(const ShadeRec& sr) const;

    private:
        // ����Դ�ɿ���/�ر���Ӧ����ӰͶ�������
        bool    shadows;    // does the light cast shadows or not
        // �����в�ʹ����������Ա����������Ϊ����������պͻ������գ�
        // ��Դ����ϵ����Ҳ��������ֵlsλ�ڶ�Ӧ������ʹ�õķ�������У�
        // ��Դ��ɫֵcolorҲ��������
        // float        ls;
        // RGBColor    color;

};


//-------------------------------------------------------------------- casts_shadows

inline bool
Light::casts_shadows() const {
    return shadows;
}


//-------------------------------------------------------------------- set_shadows

inline void
Light::set_shadows(bool _s) {
    shadows = _s;
}


// ---------------------------------------------------------------------- G
// explained on page 333
// Chapter 13.10.2�����ģ���еļ��β����
inline float
Light::G(const ShadeRec& sr) const {
    return 1.0;
}


// ---------------------------------------------------------------------- pdf
// Chapter 13.11.3������Monte Carlo���ֵĸ����ܶȺ���
inline float
Light::pdf(const ShadeRec& sr) const {
    return 1.0;
}

#endif
