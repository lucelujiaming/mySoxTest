#ifndef __RAY_CAST__
#define __RAY_CAST__

#include "Tracer.h"

class RayCast: public Tracer {

    public:

        RayCast(void);

        RayCast(World* _worldPtr);

        virtual
        ~RayCast(void);
        // �̳���Tracer������������Ӧ��дtrace_ray() ����
        virtual RGBColor
        trace_ray(const Ray& ray) const;
        // ����depth������ģ�ⷴ���͸���ȡ�
        virtual RGBColor
        trace_ray(const Ray ray, const int depth) const;
};

#endif
