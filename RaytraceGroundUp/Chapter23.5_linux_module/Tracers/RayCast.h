#ifndef __RAY_CAST__
#define __RAY_CAST__

#include "Tracer.h"

class RayCast: public Tracer {

    public:

        RayCast(void);

        RayCast(World* _worldPtr);

        virtual
        ~RayCast(void);
        // 继承于Tracer基类的相关子类应重写trace_ray() 函数
        virtual RGBColor
        trace_ray(const Ray& ray) const;
        // 加入depth，用于模拟反射和透明度。
        virtual RGBColor
        trace_ray(const Ray ray, const int depth) const;
};

#endif
