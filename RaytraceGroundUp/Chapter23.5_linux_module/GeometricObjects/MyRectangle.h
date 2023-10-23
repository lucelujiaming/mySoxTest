#ifndef __MYRectangle__
#define __MYRectangle__

#include "Sampler.h"
#include "GeometricObject.h"

// 可视为光源的几何对象应提供下列内容至Area Light类：
// 区表面上的采样点。
// 区各采样点处的pdf(需要相关对象的表面积)。
// 各采样点处的法线。
// 矩形光源几何对象。
class MyRectangle: public GeometricObject {

    public:
        
        MyRectangle(void);                                       
                
        MyRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
        
        MyRectangle(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
        
        virtual MyRectangle*                                         
        clone(void) const;
    
        MyRectangle(const MyRectangle& r);                         

        virtual                                                    
        ~MyRectangle(void);                                       

        MyRectangle&                                                 
        operator= (const MyRectangle& rhs);
        
        //BBox
        //get_bounding_box(void);                
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 并把计算出来碰撞点和碰撞点法线方向，存入ShadeRec中。
        virtual bool
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;        // 定义了光线-对象间的碰撞检测函数。
        // 判断光线ray和物体是否相交。光线的时间取值范围为[tmin, ∞)
        // 只判断是否相交。不计算碰撞点。
        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;
                
        
        // the following functions are used when the MyRectangle is a light source
        
        virtual void                                 
        set_sampler(Sampler* sampler); 
                
        virtual Point3D                                             
        sample(void);
        
        virtual Normal                                                                 
        get_normal(const Point3D& p);        
        
        virtual float                                                
			pdf(const ShadeRec& sr);
        
    private:
    
        Point3D           p0;               // 矩形的角顶点：corner vertex 
        Vector3D          a;                // 边向量：side
        Vector3D          b;                // 边向量：side
        double            a_len_squared;    // 边a长度的平方：square of the length of side a
        double            b_len_squared;    // 边b长度的平方：square of the length of side b
        Normal            normal;           // 法线方向：
        
        float             area;             // 矩形面积：for rectangular lights
        float             inv_area;         // 矩形面积的倒数：for rectangular lights
        Sampler*          sampler_ptr;      // 采样器指针：for rectangular lights     
        
        static const double kEpsilon;                                               
};

#endif
