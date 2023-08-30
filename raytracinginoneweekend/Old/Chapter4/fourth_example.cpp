#include <iostream>
#include "Ray.h"

bool hit_sphere(const CVec3& center, double radius, const CRay& r) {
	CVec3 oc = r.origin() - center;
	auto a = dot(r.direction(), r.direction());
	auto b = 2.0 * dot(oc, r.direction());
	auto c = dot(oc, oc) - radius*radius;
	auto discriminant = b*b - 4 * a*c;
	return (discriminant > 0);
}

CVec3 ray_color_by_hit_sphere(const CRay& r) {
	if (hit_sphere(CVec3(0, 0, -1), 0.5, r))
		return CVec3(1, 0, 0);
	CVec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*CVec3(1.0, 1.0, 1.0) + t*CVec3(0.5, 0.7, 1.0);
}

void fourth_picture()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	CVec3 lower_left_corner(-2.0, -1.0, -1.0);
	CVec3 horizontal(4.0, 0.0, 0.0);
	CVec3 vertical(0.0, 2.0, 0.0);
	CVec3 origin(0.0, 0.0, 0.0);
	for (int j = ny - 1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			CRay r(origin, lower_left_corner + u*horizontal + v*vertical);
			CVec3 col = ray_color_by_hit_sphere(r);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}
int main(){
    fourth_picture();
}


