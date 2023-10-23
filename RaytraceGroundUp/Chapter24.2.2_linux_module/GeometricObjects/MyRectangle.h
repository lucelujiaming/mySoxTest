#ifndef __MYRectangle__
#define __MYRectangle__

#include "Sampler.h"
#include "GeometricObject.h"

// ����Ϊ��Դ�ļ��ζ���Ӧ�ṩ����������Area Light�ࣺ
// �������ϵĲ����㡣
// ���������㴦��pdf(��Ҫ��ض���ı����)��
// �������㴦�ķ��ߡ�
// ���ι�Դ���ζ���
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
        // �жϹ���ray�������Ƿ��ཻ�����ߵ�ʱ��ȡֵ��ΧΪ[tmin, ��)
        // ���Ѽ��������ײ�����ײ�㷨�߷��򣬴���ShadeRec�С�
        virtual bool
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;        // �����˹���-��������ײ��⺯����
        // �жϹ���ray�������Ƿ��ཻ�����ߵ�ʱ��ȡֵ��ΧΪ[tmin, ��)
        // ֻ�ж��Ƿ��ཻ����������ײ�㡣
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
    
        Point3D           p0;               // ���εĽǶ��㣺corner vertex 
        Vector3D          a;                // ��������side
        Vector3D          b;                // ��������side
        double            a_len_squared;    // ��a���ȵ�ƽ����square of the length of side a
        double            b_len_squared;    // ��b���ȵ�ƽ����square of the length of side b
        Normal            normal;           // ���߷���
        
        float             area;             // ���������for rectangular lights
        float             inv_area;         // ��������ĵ�����for rectangular lights
        Sampler*          sampler_ptr;      // ������ָ�룺for rectangular lights     
        
        static const double kEpsilon;                                               
};

#endif
