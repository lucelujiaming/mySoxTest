#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class CCamera {
public:
	CCamera() {
		lower_left_corner = CVec3(-2.0, -1.0, -1.0);
		horizontal = CVec3(4.0, 0.0, 0.0);
		vertical = CVec3(0.0, 2.0, 0.0);
		origin = CVec3(0, 0, 0);
	}

	CRay get_ray(double u, double v) const {
		return CRay(origin, lower_left_corner + u*horizontal + v*vertical - origin);
	}

private:
	CVec3 origin;
	CVec3 lower_left_corner;
	CVec3 horizontal;
	CVec3 vertical;
};
#endif
