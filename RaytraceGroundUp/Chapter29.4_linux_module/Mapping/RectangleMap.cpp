//     Copyright (C) Kevin Suffern 2000-2007.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


#include "RectangleMap.h"

// ---------------------------------------------------------------- default constructor

RectangleMap::RectangleMap(void) {}


// ---------------------------------------------------------------- copy constructor

RectangleMap::RectangleMap(const RectangleMap& sm) {}


// ---------------------------------------------------------------- assignment operator

RectangleMap&
RectangleMap::operator= (const RectangleMap& rhs) {
    if (this == &rhs)
        return (*this);

    return (*this);
}


// ---------------------------------------------------------------- destructor

RectangleMap::~RectangleMap (void) {}


// ---------------------------------------------------------------- clone

RectangleMap*
RectangleMap::clone(void) const {
    return (new RectangleMap(*this));
}


// ---------------------------------------------------------------- get_texel_coordinates

// Given a hit point on a generic sphere, and the image resolution, this function
// returns the texel coordinates in the image

void
RectangleMap::get_texel_coordinates(    const     Point3D&     local_hit_point,
                                        const     int         xres,
                                        const     int         yres,
                                                int&         row,
                                                int&         column) const {

    // first, compute (u, v) in [0, 1] X [0, 1]
    // 参见P525计算(u, v)
    float u = (local_hit_point.z + 1) / 2;
    float v = (local_hit_point.x + 1) / 2;

    // finally, map u and v to the texel coordinates

    column     = (int) ((xres - 1) * u);       // column is across
    row     = (int)  ((yres - 1) * v);        // row is up
}
