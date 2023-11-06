#ifndef __CUBIC_NOISE__
#define __CUBIC_NOISE__

//     Copyright (C) Kevin Suffern 2000-2008.
//    This C++ code is for non-commercial purposes only.
//    This C++ code is licensed under the GNU General Public License Version 2.
//    See the file COPYING.txt for the full license.


// This file contains the declaration of the class CubicNoise.
// CubicNoise defines the functions scalar_noise and vector_noise
// using tri-cubic interpolation of the lattice noise values.

#include "Vector3D.h"
#include "LatticeNoise.h"

//------------------------------------------------------------------------- class CubicNoise
// 栅格噪声函数的构造过程包含下列3个步骤：
//   (1)将噪声值置于栅格顶点处；
//   (2)使用哈希(散列)技术计算噪声值栅格单元；
//   (3)通过插值计算得到栅格单元内部的噪声值。
//  Cubic Noise类则实现了3次插值计算。
class CubicNoise: public LatticeNoise {
    public:

        CubicNoise(void);

        CubicNoise(int octaves);

        CubicNoise(int octaves, float lacunarity, float gain);

        CubicNoise(const CubicNoise& cns);

        CubicNoise&
        operator= (const CubicNoise& rhs);

        virtual CubicNoise*
        clone(void) const;

        virtual
        ~CubicNoise(void);
        // 执行插值计算的相关函数
        virtual float
        value_noise(const Point3D& p) const;

        virtual Vector3D
        vector_noise(const Point3D& p) const;
};


//------------------------------------------------------------------------- four_knot_spline

// Cubic interpolation using a Catmull-Rom four knot spline.
// This is templated so that we can use it to interpolate floats and Vector3Ds.
// This is not a class member function.
// 样条插值技术
template<class T> T
four_knot_spline(const float x, const T knots[]) {
    T c3 = -0.5 * knots[0] + 1.5 * knots[1] - 1.5 * knots[2] + 0.5 * knots[3];
      T c2 = knots[0] - 2.5 * knots[1] + 2.0 * knots[2] - 0.5 * knots[3];
      T c1 = 0.5 * (-knots[0] + knots [2]);
      T c0 = knots[1];

    return (T((c3*x + c2)*x + c1)*x + c0);
}

#endif
