//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "CylinderMap.h"

// ---------------------------------------------------------------- default constructor

CylinderMap::CylinderMap(void) {}


// ---------------------------------------------------------------- copy constructor

CylinderMap::CylinderMap(const CylinderMap& sm) {}


// ---------------------------------------------------------------- assignment operator

CylinderMap&
CylinderMap::operator= (const CylinderMap& rhs) {
    if (this == &rhs)
        return (*this);

    return (*this);
}


// ---------------------------------------------------------------- destructor

CylinderMap::~CylinderMap (void) {}


// ---------------------------------------------------------------- clone

CylinderMap*
CylinderMap::clone(void) const {
    return (new CylinderMap(*this));
}


// ---------------------------------------------------------------- get_texel_coordinates

// Given a hit point on a generic sphere, and the image resolution, this function
// returns the texel coordinates in the image

void
CylinderMap::get_texel_coordinates(    const     Point3D&     local_hit_point,
                                        const     int         xres,
                                        const     int         yres,
                                                int&         row,
                                                int&         column) const {

    // first, compute phi
    // 参见P525计算phi
    float phi   = atan2(local_hit_point.x, local_hit_point.z);
    if (phi < 0.0)
        phi += TWO_PI;

    // next, map theta and phi to (u, v) in [0, 1] X [0, 1]
    // 参见P525计算(u, v)
    float u = phi * invTWO_PI;
    float v = (local_hit_point.y + 1) / 2;

    // finally, map u and v to the texel coordinates

    column     = (int) ((xres - 1) * u);       // column is across
    row     = (int)  ((yres - 1) * v);        // row is up
}