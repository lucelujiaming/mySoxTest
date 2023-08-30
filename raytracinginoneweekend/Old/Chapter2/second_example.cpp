#include <iostream>
#include "Vec3.h"

void second_picture()
{
	int nx = 200;
	int ny = 100;
	std::cout << "P3\n" << nx << ' ' << ny << "\n255\n"; 
	for (int j = ny - 1; j >= 0; j--) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < nx; i++) {
			CVec3 col(float(i) / float(nx),
				float(j) / float(ny), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			std::cout << ir << " " << ig << " " << ib << "\n";
		}
	}
	std::cerr << "\nDone.\n";
}

int main(){
    second_picture();
}


