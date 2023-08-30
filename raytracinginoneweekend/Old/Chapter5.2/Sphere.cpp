#include "Sphere.h"


bool CSphere::hit(const CRay& r, double t_min, double t_max, hit_record& rec) const {
	CVec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto half_b = dot(oc, r.direction());
	auto c = dot(oc, oc) - radius * radius;

	auto discriminant = half_b * half_b - a * c;
	if (discriminant < 0) return false;
	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < t_min || t_max < root) {
		root = (-half_b + sqrtd) / a;
		if (root < t_min || t_max < root)
			return false;
	}

	rec.t = root;
	rec.p = r.point_at_parameter(rec.t);
	rec.normal = (rec.p - center) / radius;

	return true;
}
