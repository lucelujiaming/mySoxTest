#include "BBox.h"
#include "Constants.h"


// --------------------------------------------------------------------- default constructor

BBox::BBox (void)
    : x0(-1), x1(1), y0(-1), y1(1), z0(-1), z1(1)
{}    


// --------------------------------------------------------------------- constructor

BBox::BBox (    const double _x0, const double _x1,            
                const double _y0, const double _y1, 
                const double _z0, const double _z1)
    : x0(_x0), x1(_x1), y0(_y0), y1(_y1), z0(_z0), z1(_z1)
{}


// --------------------------------------------------------------------- constructor

BBox::BBox (const Point3D p0, const Point3D p1)
    : x0(p0.x), x1(p1.x), y0(p0.y), y1(p1.y), z0(p0.z), z1(p1.z)
{}
                                        


// --------------------------------------------------------------------- copy constructor

BBox::BBox (const BBox& bbox)
    : x0(bbox.x0), x1(bbox.x1), y0(bbox.y0), y1(bbox.y1), z0(bbox.z0), z1(bbox.z1)
{}                        
                

// --------------------------------------------------------------------- assignment operator

BBox&
BBox::operator= (const BBox& rhs) {
    if (this == &rhs)
        return (*this);

    x0    = rhs.x0;
    x1    = rhs.x1;
    y0    = rhs.y0;
    y1    = rhs.y1;
    z0    = rhs.z0;
    z1    = rhs.z1;    
    
    return (*this);
}            


// --------------------------------------------------------------------- destructor

BBox::~BBox (void) {}    


// --------------------------------------------------------------------- hit
// 碰撞函数
bool                                     
BBox::hit(const Ray& ray) const {
    // ox、oy、oz表示光线源点坐标。
    double ox = ray.o.x; double oy = ray.o.y; double oz = ray.o.z;
    // (dx, dy, dz)为光线方向。
    double dx = ray.d.x; double dy = ray.d.y; double dz = ray.d.z;
    
    // 光线与包围盒x0，y0，z0，x1，y1，z1碰撞时的t值的最大值和最小值。
    double tx_min, ty_min, tz_min;
    double tx_max, ty_max, tz_max; 
    
    // the following code includes modifications from Shirley and Morley (2003)
    // 因为光线公式d * t + o = p，则：t = (p - o) / d。
    // 计算光线与包围盒碰撞时的tx值的最大值和最小值。
    double a = 1.0 / dx;
    if (a >= 0) {
        tx_min = (x0 - ox) * a;
        tx_max = (x1 - ox) * a;
    }
    else {
        tx_min = (x1 - ox) * a;
        tx_max = (x0 - ox) * a;
    }
    // 计算光线与包围盒碰撞时的ty值的最大值和最小值。
    double b = 1.0 / dy;
    if (b >= 0) {
        ty_min = (y0 - oy) * b;
        ty_max = (y1 - oy) * b;
    }
    else {
        ty_min = (y1 - oy) * b;
        ty_max = (y0 - oy) * b;
    }
    // 计算光线与包围盒碰撞时的tz值的最大值和最小值。
    double c = 1.0 / dz;
    if (c >= 0) {
        tz_min = (z0 - oz) * c;
        tz_max = (z1 - oz) * c;
    }
    else {
        tz_min = (z1 - oz) * c;
        tz_max = (z0 - oz) * c;
    }
    
    double t0, t1;
    
    // find largest entering t value
    // t0为光线与包围盒碰撞时的t值。
    // tx_min, ty_min, tz_min表明在和包围盒xyz方向的三个平面上碰撞时的t值。
    // 因为和包围盒平面碰撞不等于和包围盒碰撞。
    // 因此上，我们需要取这三个值中的最大值作为实际碰撞值。
    if (tx_min > ty_min)
        t0 = tx_min;
    else
        t0 = ty_min;
    if (tz_min > t0)
        t0 = tz_min;
        
    // find smallest exiting t value
    // t0为光线离开包围盒时的t值。
    // tx_max, ty_max, tz_max表明在离开包围盒xyz方向的三个平面时的t值。
    // 因为在包围盒平面的时候，我们可能早就离开包围盒了。
    // 因此上，我们需要取这三个值中的最小值作为实际离开值。
    if (tx_max < ty_max)
        t1 = tx_max;
    else
        t1 = ty_max;
        
    if (tz_max < t1)
        t1 = tz_max;
    // 如果先离开后碰撞，说明光线未与栅格对象的包围盒碰撞。直接返回false。
    // 否则返回真。说明发生碰撞。
    return (t0 < t1 && t1 > kEpsilon);
}


// --------------------------------------------------------------------- inside
// used to test if a ray starts inside a grid
// 判断点p是否位于盒子内部。也就是(x, y, z)坐标位于盒体角顶点坐标范围内。
bool
BBox::inside(const Point3D& p) const {
    return ((p.x > x0 && p.x < x1) 
         && (p.y > y0 && p.y < y1) 
         && (p.z > z0 && p.z < z1));
};




