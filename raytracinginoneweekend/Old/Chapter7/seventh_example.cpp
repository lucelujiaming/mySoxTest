#include <iostream>
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"

#define USE_PIMPLES_FIX

CVec3 random_in_unit_sphere(){
	CVec3 p;
	do{
		p = 2.0 * CVec3(drand48(), drand48(), drand48()) - CVec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}


CVec3 diffuse_color(const CRay&r, CHittable * world, int depth) {
	hit_record rec;
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
        	return CVec3(0,0,0);
#ifdef USE_PIMPLES_FIX
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
#else
	if (world->hit(r, 0.0, MAXFLOAT, rec)) {
#endif
		CVec3 target = rec.p + rec.normal + random_in_unit_sphere();
		return 0.5 * diffuse_color(CRay(rec.p, target - rec.p), world, depth - 1);
	}
	else{
		CVec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return(1.0 - t) *CVec3(1.0, 1.0, 1.0) + t*CVec3(0.5, 0.7, 1.0);
	}
}


void seventh_picture(bool isGray)
{
	int nx = 200;
	int ny = 100;
	int ns = 100;
	const int max_depth = 50;
	
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
				col += diffuse_color(r, world, max_depth);
			}
			col /= float(ns);
			if(isGray)
			{
				col = CVec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			}

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

int main(int argc, char ** argv){
    if(argc == 2)
    {
        seventh_picture(true);
    }
    else 
    {
        seventh_picture(false);
    }
}


