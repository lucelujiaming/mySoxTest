#ifndef __Box__
#define __Box__

#include "Sampler.h"
#include "GeometricObject.h"

// ����������������Χ�о�����ͬ����״���������ڼ��ζ��󷶳벢�����ڽ�ģͼԪ��
// ���⣬���������δ��ȡ��ʽ���̶��壬����������ǳ������ڹ��߸��ټ���Ľ�ѧ��ʾ��
// ͬʱ���������廹��������ʾ��28���е�ȫ�ڷ��䡣
class Box: public GeometricObject {

    public:
        
        Box(void);                                       
                
        Box(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b);
        
        Box(const Point3D& _p0, const Vector3D& _a, const Vector3D& _b, const Normal& n);
        
        virtual Box*                                         
        clone(void) const;
    
        Box(const Box& r);                         

        virtual                                                    
        ~Box(void);                                       

        Box&                                                 
        operator= (const Box& rhs);
        
        //Box
        //get_bounding_box(void);                
    
        virtual bool                                                  
        hit(const Ray& ray, double& t, ShadeRec& s) const;

        virtual bool
        shadow_hit(const Ray& ray, float& tmin) const;
        // �����Է��߼��Լ��㡣
        Normal
        get_normal(const int face_hit) const;
                
        
    private:
        // ���ԽǶ���洢��6��˽�����͵ĸ������С�
        float x0, y0, z0, x1, y1, z1;
        
        static const double kEpsilon;                                               
};

#endif
