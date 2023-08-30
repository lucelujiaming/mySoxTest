#include "HittableList.h"


bool CHittableList::hit(const CRay& r, double t_min, double t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool hit_anything = false;
	auto closest_so_far = t_max;

	for (int i = 0; i < list_size; i++) {
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			std::cout << "get temp_rec in the CHittableList \n" ;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
