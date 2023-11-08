// This file contains the definition of the Camera class

#include "stdafx.h"
#include "Camera.h"


// ----------------------------------------------------------------- default constructor

Camera::Camera(void)
    :    eye(0, 0, 500),
        lookat(0),
        ra(0),
        up(0, 1, 0),
        u(1, 0, 0),
        v(0, 1, 0),
        w(0, 0, 1),
        exposure_time(1.0)
{}


// ----------------------------------------------------------------- copy constructor

Camera::Camera(const Camera& c)
    :     eye(c.eye),
        lookat(c.lookat),
        ra(c.ra),
        up(c.up),
        u(c.u),
        v(c.v),
        w(c.w),
        exposure_time(c.exposure_time)
{}


// ----------------------------------------------------------------- assignment operator

Camera&
Camera::operator= (const Camera& rhs) {
    if (this == &rhs)
        return (*this);

    eye                = rhs.eye;
    lookat            = rhs.lookat;
    ra                = rhs.ra;
    up                = rhs.up;
    u                = rhs.u;
    v                = rhs.v;
    w                = rhs.w;
    exposure_time     = rhs.exposure_time;

    return (*this);
}


// ----------------------------------------------------------------- destructor

Camera::~Camera(void) {}


//-------------------------------------------------------------- compute_uvw

// This computes an orthornormal basis given the view point, lookat point, and up vector
// 因为，我们使用正交基向量(u，y，w)定义视见坐标系统，因此上公式9.1计算出来的w就是z轴。
// 之后我们使用9.2和9.3计算另外两个轴。这就是这个函数的意思。
void
Camera::compute_uvw(void) {
    w = eye - lookat;   // 公式9.1
    w.normalize();
    u = up ^ w;         // 公式9.2
    u.normalize();
    v = w ^ u;          // 公式9.3

    // 当相机垂直向上或向下观察场景时，将会出现异常情况。
    // take care of the singularity by hardwiring in specific camera orientations
    // 当相机垂直向下时，则按照下列方式定义向量u、v和w
    if (eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) { // camera looking vertically down
        u = Vector3D(0, 0, 1);
        v = Vector3D(1, 0, 0);
        w = Vector3D(0, 1, 0);
    }
    // 当相机垂直向上时，则按照下列方式定义向量u、v和w
    if (eye.x == lookat.x && eye.z == lookat.z && eye.y < lookat.y) { // camera looking vertically up
        u = Vector3D(1, 0, 0);
        v = Vector3D(0, 0, 1);
        w = Vector3D(0, -1, 0);
    }
}
