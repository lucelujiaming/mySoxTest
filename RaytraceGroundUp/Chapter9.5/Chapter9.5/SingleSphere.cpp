#include "stdafx.h"
#include "SingleSphere.h"
#include "World.h"
#include "ShadeRec.h"

// -------------------------------------------------------------------- default constructor

SingleSphere::SingleSphere(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

SingleSphere::SingleSphere(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

SingleSphere::~SingleSphere(void) {}


// -------------------------------------------------------------------- trace_ray
// 该函数将对一个红色球体实施光线跟踪计算，其中心位置和半径在程序清单3.11中所示的函数中加以定义。
RGBColor
SingleSphere::trace_ray(const Ray& ray) const {
	ShadeRec	sr(*world_ptr); 	// not used
	double    	t;  				// not used

	if (world_ptr->sphere.hit(ray, t, sr))
		return (red);
	else
		return (black);
}
