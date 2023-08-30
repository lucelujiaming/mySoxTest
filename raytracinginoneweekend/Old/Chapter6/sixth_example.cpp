#include <iostream>
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"

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

void sixth_picture()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;
	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	CHittable * hitable_list[2];
	hitable_list[0] = new CSphere(CVec3(0, 0, -1), 0.5);
	hitable_list[1] = new CSphere(CVec3(0, -100.5, -1), 100);

	CHittable *world = new CHittableList(hitable_list, 2);
	CCamera cam;
	for (int j = ny - 1; j >= 0; j--){
		for (int i = 0; i < nx; i++){
			CVec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);

				CRay r = cam.get_ray(u, v);
				CVec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

int main(){
    sixth_picture();
}


