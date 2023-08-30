#ifndef CSPHERE_H
#define CSPHERE_H

#include "Hitable.h"
#include "Vec3.h"
#include "Material.h"

class CSphere :
	public CHittable
{
public:
	CSphere() {}
        CSphere(CVec3 cen, float r, CMaterial *m) 
        		: center(cen), radius(r), mat_ptr(m)  {};
	CSphere(CVec3 cen, double r) 
			: center(cen), radius(r) {};

	virtual bool hit(
		const CRay& r, double t_min, 
		double t_max, hit_record& rec) const override;

public:
	CVec3 center;
	double radius;
        CMaterial *mat_ptr;
};


#endif

