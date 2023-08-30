#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "Material.h"
#include "Ray.h"

CVec3 reflect(const CVec3& v, const CVec3& n) {
    return v - 2*dot(v,n)*n;
}

class CMetal : public CMaterial {
    public:
        CMetal(const color& a) : albedo(a) {}

        virtual bool scatter(
            const CRay& r_in, const hit_record& rec, CVec3& attenuation, CRay& scattered
        ) const override {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = CRay(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
        CVec3 albedo;
};


#endif

