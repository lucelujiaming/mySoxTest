#include <iostream>
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"


#define MAXFLOAT 2000
CVec3 color(const CRay&r, CHittable * world) {
	hit_record rec;
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
		return 0.5 * CVec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else{
		CVec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return(1.0 - t) *CVec3(1.0, 1.0, 1.0) + t*CVec3(0.5, 0.7, 1.0);
	}
}

void fifth_picture()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	CVec3 lower_left_corner(-2.0, -1.0, -1.0);
	CVec3 horizontal(4.0, 0.0, 0.0);
	CVec3 vertical(0.0, 2.0, 0.0);
	CVec3 origin(0.0, 0.0, 0.0);
	CHittable * hitable_list[2];
	hitable_list[0] = new CSphere(CVec3(0, 0, -1), 0.5);
	hitable_list[1] = new CSphere(CVec3(0, -100.5, -1), 100);

	CHittable *world = new CHittableList(hitable_list, 2);

	for (int j = ny - 1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			CRay r(origin, lower_left_corner + u*horizontal + v*vertical);
			CVec3   p = r.point_at_parameter(2.0);
			CVec3 col = color(r, world);
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}


int main(){
    fifth_picture();
}


