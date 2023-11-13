#ifndef __WHITTED__
#define __WHITTED__

#include "Tracer.h"

class Whitted: public Tracer {
    public:
        
        Whitted(void);
        
        Whitted(World* _worldPtr);
        
        virtual                                            
        ~Whitted(void);
        // 使用depth参数存储ShadeRec对象中各条光线的反射次数。
        virtual RGBColor    
        trace_ray(const Ray ray, const int depth) const;
        
        virtual RGBColor
        trace_ray(const Ray ray, float& tmin, const int depth) const;
};

#endif
