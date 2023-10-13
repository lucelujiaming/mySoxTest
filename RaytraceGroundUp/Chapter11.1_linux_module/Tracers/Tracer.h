// This is the declaration of the base class Tracer
// The tracer classes have no copy constructor, assignment operator. or clone function because 
// of the world pointer, which should not be assigned or copy constructed
// See comments in the World.h file.

#ifndef __TRACER__
#define __TRACER__

#include "Constants.h"
#include "Ray.h"
#include "RGBColor.h"

class World;

// 跟踪器允许用户利用多种版本的trace_ray()函数实现不同类型的光线跟踪计算，且无须替换当前代码。
// 因此，在计算光线源点、方向的render_scene()函数以及其他光线计算代码之间，
// 跟踪器包含了一套系统的抽象层次结构。
// 跟踪器按照一定的继承结构方式加以组织。
class Tracer {

    public:
        Tracer(void);
        Tracer(World* _world_ptr);
        virtual
        ~Tracer(void);
        // 继承于Tracer基类的相关子类应重写trace_ray() 函数
        virtual RGBColor
        trace_ray(const Ray& ray) const;
        virtual RGBColor
        trace_ray(const Ray ray, const int depth) const;
        virtual RGBColor
        trace_ray(const Ray ray, float& tmin, const int depth) const;

    protected:
        // 指向World类的指针，旨在访问场景中的几何对象以及背景颜色。
        World* world_ptr;
};

#endif
