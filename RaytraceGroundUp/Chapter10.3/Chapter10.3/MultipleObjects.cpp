﻿#include "stdafx.h"
#include "MultipleObjects.h"
#include "World.h"

// -------------------------------------------------------------------- default constructor

MultipleObjects::MultipleObjects(void)
	: Tracer()
{}


// -------------------------------------------------------------------- constructor

MultipleObjects::MultipleObjects(World* _worldPtr)
	: Tracer(_worldPtr)
{}


// -------------------------------------------------------------------- destructor

MultipleObjects::~MultipleObjects(void) {}


// -------------------------------------------------------------------- trace_ray
// 显示了在光线与对象发生碰撞时最近对象间颜色值的返回结果。
RGBColor
MultipleObjects::trace_ray(const Ray& ray) const {
	ShadeRec sr(world_ptr->hit_objects(ray)); // sr is copy constructed

	if (sr.hit_an_object)
		return (sr.color);
	else
		return (world_ptr->background_color);
}
