#ifndef __PLANE__
#define __PLANE__

#include "GeometricObject.h"

//-------------------------------------------------------------------- class Plane
// Plane类存储了顶点和法线数据，其中包含了两个构造函数。
// 每个类均应包含一个默认构造函数，并在必要时添加其他类型的构造函数。
class Plane: public GeometricObject {
	public:
		Plane(void);												// default constructor
		Plane(const Point3D& point, const Normal& normal);			// constructor
		Plane(const Plane& plane); 									// copy constructor
		virtual Plane* 												// virtual copy constructor
		clone(void) const;
		Plane& 														// assignment operator
		operator= (const Plane& rhs);	
		virtual														// destructor
		~Plane(void);
		virtual bool
		hit(const Ray& ray, double& tmin, ShadeRec& sr) const;		// 定义了光线-对象间的碰撞检测函数。
		virtual bool
		shadow_hit(const Ray& ray, float& tmin) const;

	public:

		Vector3D 	a;   				// point through which plane passes
		Vector3D 		n;					// normal to the plane
		static const double kEpsilon;   // for shadows and secondary rays
};

#endif
