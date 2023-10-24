#ifndef __SHADE_REC__
#define __SHADE_REC__

// this file contains the declaration of the class ShadeRec

#include <vector>

class Material;
class World;

// We need the following as #includes instead of forward class declarations,
// because we have the objects themselves, not pointers or references

#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"
#include "RGBColor.h"

class ShadeRec {

	public:

		bool				hit_an_object;	
		Material* 			material_ptr;	
		Point3D 			hit_point;		
		Point3D				local_hit_point;
		Normal				normal;			
		Ray					ray;			
		int					depth;			
		Vector3D			dir;			
		float				t;				
		World&				w;				
		RGBColor			color;

		ShadeRec(World& wr);			

		ShadeRec(const ShadeRec& sr);	
};

#endif
