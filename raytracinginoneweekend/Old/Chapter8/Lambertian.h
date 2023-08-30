#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"
#include "Ray.h"

CVec3 random_in_unit_sphere(){
	CVec3 p;
	do{
		p = 2.0 * CVec3(drand48(), drand48(), drand48()) - CVec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}


class CLambertian : public CMaterial {
public:
    CLambertian(const CVec3& a) : albedo(a) {}

    virtual bool scatter(
        const CRay& r_in, const hit_record& rec, CVec3& attenuation, CRay& scattered
    ) const override {
		std::cout << "CLambertian scatter \n" ;
        auto scatter_direction = rec.normal + random_in_unit_sphere();

        // Catch degenerate scatter direction
        // if (scatter_direction.near_zero())
        //      scatter_direction = rec.normal;

        scattered = CRay(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    CVec3 albedo;
};

#endif

