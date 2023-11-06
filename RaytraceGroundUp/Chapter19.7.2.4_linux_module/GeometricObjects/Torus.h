#ifndef __TORUS__
#define __TORUS__

#include "GeometricObject.h"
#include "BBox.h"
// 一个位于YZ平面上的圆绕着Y轴，在水平方向上绕一周。
// 可以得到关于XYZ的隐式方程。参见公式19.19。
// 代入光线方程可以得到关于t的一元四次方程。
// 显然光线和圆环可以有四个交点。
class Torus: public GeometricObject {
    public:
        
        Torus(void);                                       
        
        Torus(const double _a, const double _b);

        virtual Torus*                                     
        clone(void) const;
    
        Torus(const Torus& torus);
        
        virtual
        ~Torus(void);                                             
        
        Torus& 
        operator= (Torus& rhs);    
        
        Normal                     
        compute_normal(const Point3D& p) const;                                                  
        
        virtual bool                                                                  
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool                                                                  
        shadow_hit(const Ray& ray, float& tmin) const;
        
    private:
    
        double         a;         // swept radius 
        double        b;         // tube radius
        BBox        bbox;    // the bounding box
};

#endif
