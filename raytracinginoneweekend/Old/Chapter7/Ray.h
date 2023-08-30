#ifndef RAY_H
#define RAY_H

#include "Vec3.h"
class CRay
{
public:
	CRay(){}
	CRay(const CVec3& a, const CVec3 & b){
		A = a; 
		B = b; 
	}
	
	CVec3 origin() const { return A; }
	CVec3 direction() const { return B; }
	CVec3 point_at_parameter(float t) const{
		return A + t * B; 
	}
	CVec3 A;
	CVec3 B;
};

#endif

