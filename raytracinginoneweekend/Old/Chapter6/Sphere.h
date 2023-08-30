#ifndef CSPHERE_H
#define CSPHERE_H

#include "Hitable.h"
#include "Vec3.h"

class CSphere :
	public CHittable
{
public:
	CSphere() {}
	CSphere(CVec3 cen, double r) : center(cen), radius(r) {};

	virtual bool hit(
		const CRay& r, double t_min, double t_max, hit_record& rec) const override;

public:
	CVec3 center;
	double radius;
};


#endif

