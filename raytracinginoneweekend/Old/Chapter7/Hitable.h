#ifndef CHITTABLE_H
#define CHITTABLE_H

#include "Ray.h"

struct hit_record {
	CVec3 p;
	CVec3 normal;
	double t;
};

class CHittable {
public:
	//�����t_min,t_max��Χ�ڹ����Ƿ��������н���
	virtual bool hit(const CRay& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif
