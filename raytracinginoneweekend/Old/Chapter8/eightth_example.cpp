#include <iostream>
#include "Ray.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Lambertian.h"

//  https://github.com/RayTracing/raytracing.github.io


CVec3 diffuse_color(const CRay &r, CHittable * world, int depth) {
	hit_record rec;
	rec.mat_ptr = NULL;
	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth > 50) {
        	return CVec3(0,0,0);
	}
	if (world->hit(r, 0.001, MAXFLOAT, rec)) {
		std::cout << "Hit  \n" ;
        	CRay scattered;
        	CVec3 attenuation;
        	if(rec.mat_ptr)
        	{
			std::cout << "mat_ptr \n" ;
			if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			{
				std::cout << "scatter \n" ;
		    		return attenuation * diffuse_color(scattered, world, depth+1);
		    	}
		    	else 
			{
				std::cout << "Not scatter \n" ;
				return CVec3(0,0,0);
		    	}
            	}
            	else {
			std::cout << "No mat_ptr \n" ;
        		return CVec3(0,0,0);
		}
	}
	else{
		CVec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return(1.0 - t) *CVec3(1.0, 1.0, 1.0) + t*CVec3(0.5, 0.7, 1.0);
	}
}


void eightth_picture()
{
	int nx = 200;
	int ny = 100;
	int ns = 100;
	
	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n";
	CHittable * hitable_list[4];
	hitable_list[0] = new CSphere(CVec3(0, 0, -1), 0.5, new CLambertian(CVec3(0.8, 0.3, 0.3)));
	hitable_list[1] = new CSphere(CVec3(0, -100.5, -1), 100, new CLambertian(CVec3(0.8, 0.8, 0.0)));
	hitable_list[2] = new CSphere(CVec3(1, 0, -1), 0.5, new CLambertian(CVec3(0.8, 0.6, 0.2)));
	hitable_list[3] = new CSphere(CVec3(-1, 0, -1), 0.5, new CLambertian(CVec3(0.8, 0.8, 0.8)));

	CHittable *world = new CHittableList(hitable_list, 4);
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
				col += diffuse_color(r, world, 0);
			}
			col /= float(ns);
			col = CVec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
}

int main(int argc, char ** argv){
   eightth_picture();
}


