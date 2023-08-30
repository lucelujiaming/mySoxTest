#ifndef CHITTABLELIST_H
#define CHITTABLELIST_H

#include "Hitable.h"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class CHittableList :
	public CHittable
{
public:
	CHittableList() {}
	CHittableList(CHittable **l, int n) {
		list = l;
		list_size = n;
	}

	virtual bool hit(
		const CRay& r, double t_min, double t_max, hit_record& rec) const ;

public:
	CHittable ** list;
	int list_size;
};


#endif
