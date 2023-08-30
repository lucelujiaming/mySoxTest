#ifndef CHITTABLE_H
#define CHITTABLE_H

#include "Material.h"
#include "Ray.h"

struct hit_record {
    float t;
	CVec3 p;
	CVec3 normal;
	// double t;
    CMaterial * mat_ptr;
};

class CHittable {
public:
	//检测在t_min,t_max范围内光线是否与物体有交点
	virtual bool hit(const CRay& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
