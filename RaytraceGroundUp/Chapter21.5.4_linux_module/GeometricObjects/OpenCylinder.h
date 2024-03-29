#ifndef __OPEN_CYLINDER__
#define __OPEN_CYLINDER__

//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


// An open cylinder is defined by its extent in the y direction and its radius.
// It is centered on the y axis.
// The classes ConcaveOpenCylinder and ConvexOpenCylinder can inherit from OpenCylinder.
// The classes ConcavePartCylinder and ConvexPartCylinder can in turn inherit from these.

#include "GeometricObject.h"
// OpenCylinder是一个绕Y轴的圆柱体。
class OpenCylinder: public GeometricObject {
    
    public:
        
        OpenCylinder(void);                                       

        OpenCylinder(const double bottom, const double top, const double radius);

        OpenCylinder(const OpenCylinder& c);                     
        
        virtual OpenCylinder*                                     
        clone (void) const;

        OpenCylinder&                                             
        operator= (const OpenCylinder& rhs);        
        
        virtual                                                 
        ~OpenCylinder(void);
                        
        virtual bool
        hit(const Ray& ray, double& t, ShadeRec& sr) const;
        
        virtual bool
        shadow_hit(const Ray& ray, float& t) const;
        
    protected:
        double        y0;                // 底面的Y值：bottom y value
        double        y1;                // 顶面的Y值：top y value
        double        radius;            // 半径：radius
        double        inv_radius;      // 半径的倒数：one over the radius    
};

#endif
