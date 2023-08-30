#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"

struct hit_record;

class CMaterial {
    public:
        virtual bool scatter(
            const CRay& r_in, const hit_record& rec, CVec3& attenuation, CRay& scattered
        ) const = 0;
};


#endif

