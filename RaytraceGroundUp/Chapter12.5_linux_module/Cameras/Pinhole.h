#ifndef __PINHOLE__
#define __PINHOLE__

// This file contains the declaration of the class Pinhole

#include "Point2D.h"
#include "World.h"        // we can #include "World.h" here

//--------------------------------------------------------------------- class Pinhole

class Pinhole: public Camera {

    public:

        Pinhole();

        Pinhole(const Pinhole& ph);

        virtual Camera*
        clone(void) const;

        Pinhole&
        operator= (const Pinhole& rhs);

        virtual
        ~Pinhole();

        void
        set_view_distance(const float vpd);

        void
        set_zoom(const float zoom_factor);

        Vector3D
        get_direction(const Point2D& p) const;

        void
        render_stereo(const World& w, float x, int pixel_offset);

        virtual void
        render_scene(const World& w);

    private:

        float    d;        // 视平面距离：view plane distance
        float    zoom;     // 缩放系数：zoom factor

};


//-------------------------------------------------------------------------- set_vpd

inline void
Pinhole::set_view_distance(float _d) {
    d = _d;
}


//-------------------------------------------------------------------------- set_zoom

inline void
Pinhole::set_zoom(float zoom_factor) {
    zoom = zoom_factor;
}

#endif
