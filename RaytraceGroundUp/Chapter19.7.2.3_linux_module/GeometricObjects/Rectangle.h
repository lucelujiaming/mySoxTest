#ifndef __RECTANGLE__
#define __RECTANGLE__



#include "Sampler.h"
#include "GeometricObject.h"
#include "BBox.h"

class Rectangle: public GeometricObject {	
	public:
		
		Rectangle(void);   									
				
		Rectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
		
		Rectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
		
		virtual Rectangle* 										
		clone(void) const;
	
		Rectangle(const Rectangle& r); 						

		virtual													
		~Rectangle(void);   									

		Rectangle& 												
		operator= (const Rectangle& rhs);
		
		BBox
		get_bounding_box(void);				
	
		virtual bool 												 
		hit(const Ray& ray, double& t, ShadeRec& s) const;	
				
        virtual bool
        shadow_hit(const Ray& ray, float& t) const;
		
		// the following functions are used when the rectangle is a light source
		
		virtual void 								
		set_sampler(Sampler* sampler); 
				
		virtual Point3D 											
		sample(void);
		
		virtual Normal 																
		get_normal(const Point3D& p);		
		
		virtual float												
		pdf(ShadeRec& sr);
		
	private:
        // 角顶点
		Point3D 		p0;   			// corner vertex 
        // 两条邻接边上的两个顶点
		Vector3D		a;				// side
		Vector3D		b;				// side
        // 邻接边长的平方
		double			a_len_squared;	// square of the length of side a
		double			b_len_squared;	// square of the length of side b
        // 法线方向
		Normal			normal;	
		// 面积
		float			area;			// for rectangular lights
        // 面积的倒数
		float			inv_area;		// for rectangular lights
        // 采样器指针
		Sampler*		sampler_ptr;	// for rectangular lights 	
		
		static const double kEpsilon;   											
};

#endif
