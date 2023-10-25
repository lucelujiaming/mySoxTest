#ifndef __INSTANCE__
#define __INSTANCE__

// This includes a bounding box, instead of computing it. 
// This uses extra memory but the object to be transformed may be a grid or some other complex object
// whose bouding box is expensive to compute.

#include "GeometricObject.h"
#include "Matrix.h"
#include "BBox.h"

class Instance: public GeometricObject {    
    public:
        
        Instance(void);                                       
        
        Instance(GeometricObject* obj_ptr);                    
        
        Instance(const Instance& instance); 
        
        virtual Instance*                                     
        clone(void) const;                

        virtual                                                
        ~Instance(void); 
        
        Instance&                                             
        operator= (const Instance& rhs);
                
        void                                                 
        set_object(GeometricObject* obj_ptr);    
        
        void
        transform_texture(const bool transform);
            
        virtual void                                         
        compute_bounding_box(void);
        
        virtual BBox                                         
        get_bounding_box(void);                    
                                            
        virtual Material*                                     
        get_material(void) const;                                  
                
        virtual void                                  
        set_material(Material* materialPtr);
                
        virtual bool                                                  
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;
        
        
        // affine tranformation functions
        // 平移变换
        void                                                
        translate(const Vector3D& trans);
        // 平移变换
        void
        translate(const double dx, const double dy, const double dz);    
        // 比例变换
        void                                                
        scale(const Vector3D& s);
        // 比例变换
        void 
        scale(const double a, const double b, const double c);
        // 绕X轴旋转变换
        virtual void                                        
        rotate_x(const double r);
        // 绕Y轴旋转变换
        virtual void                                        
        rotate_y(const double r);
        // 绕Z轴旋转变换矩阵
        virtual void                                        
        rotate_z(const double r);
        // 错切变换
        void                                                
        shear(const Matrix& m);
        
    private:
        GeometricObject*    object_ptr;               // 对象列表：object to be transformed
        Matrix              inv_matrix;               // 逆变换矩阵：inverse transformation matrix
        static   Matrix     forward_matrix;           // 正变换矩阵：transformation matrix
        BBox                bbox;                     // 包围盒：transformed object's bounding box
        bool                transform_the_texture;    // 是否转换材质：do we transform the texture?
            

};


inline void
Instance::transform_texture(const bool transform) {
    transform_the_texture = transform;
}    

#endif
