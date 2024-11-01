#ifndef __BBOX__
#define __BBOX__

#include "Ray.h"
#include "Point3D.h"

class BBox {
    public:
        // 盒体角顶点坐标
        double x0, x1, y0, y1, z0, z1;
        
        BBox(void);                                        
                        
        BBox(    const double x0, const double x1,            
                const double y0, const double y1, 
                const double z0, const double z1);

        BBox(const Point3D p0, const Point3D p1);                                            

        BBox(const BBox& bbox);                            

        BBox&                                                 
        operator= (const BBox& rhs);            

        ~BBox(void);                                        
        // 碰撞函数
        bool                                                             
        hit(const Ray& ray) const;
        // 判断点p是否位于盒子内部。
        bool
        inside(const Point3D& p) const;
};
    
#endif

