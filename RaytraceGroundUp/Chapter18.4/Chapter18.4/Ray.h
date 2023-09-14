#ifndef __RAY__
#define __RAY__

#include "Point3D.h"
#include "Vector3D.h"

// 定义一个Ray类以存储光源点及其方向向量。
class Ray {

	public:

		Point3D			o;  			// origin						光源点
		Vector3D		d; 				// direction					方向向量
		int				depth;			// ray depth, for reflections	光线深度用于反射。
		Ray(void);						//default constructor			默认建构函数。
		Ray(const Point3D& origin,		
			const Vector3D& dir);		//constructor
		Ray(const Ray& ray);			//copy constructor
		Ray&
		operator= (const Ray& rhs);		//assignment operator

		~Ray(void);						//destructor
};

#endif
