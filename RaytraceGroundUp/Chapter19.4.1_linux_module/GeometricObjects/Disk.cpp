#include "Disk.h"


const double Disk::kEpsilon = 0.001;

// ----------------------------------------------------------------------  default constructor

Disk::Disk(void)
	: 	GeometricObject(),
		center(0.0),
		radius(1.0),
		normal(0, 1, 0),
		area(PI),
		inv_area(1.0 / area),
		sampler_ptr(NULL)
{}


// ----------------------------------------------------------------------  constructor

Disk::Disk(const Point3D& c, const double& r, const Normal& n)
	:	GeometricObject(),
		center(c),
		radius(r),
		normal(n),
		area(r*r*PI),
		inv_area(1.0 / area),
		sampler_ptr(NULL)
{
		normal.normalize();
}


// ---------------------------------------------------------------- clone

Disk*
Disk::clone(void) const {
	return (new Disk(*this));
}


// ---------------------------------------------------------------- copy constructor

Disk::Disk(const Disk& disk)
	:	GeometricObject(disk),
		center(disk.center),
		radius(disk.radius),
		normal(disk.normal),
		area(disk.area),
		inv_area(disk.inv_area)
{
	if(disk.sampler_ptr)
		sampler_ptr	= disk.sampler_ptr->clone();
	else  sampler_ptr = NULL;
}


// ---------------------------------------------------------------- assignment operator

Disk&
Disk::operator= (const Disk& rhs)	{

	if (this == &rhs)
		return (*this);

	GeometricObject::operator= (rhs);

	center      = rhs.center;
	radius      = rhs.radius;
	normal      = rhs.normal;
	area        = rhs.area;
	inv_area    = rhs.inv_area;

	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}

	if (rhs.sampler_ptr)
		sampler_ptr= rhs.sampler_ptr->clone();

	return (*this);
}


// ---------------------------------------------------------------- destructor

Disk::~Disk(void)
{
	if (sampler_ptr) {
		delete sampler_ptr;
		sampler_ptr = NULL;
	}
}


// ---------------------------------------------------------------- get_bounding_box

BBox
Disk::get_bounding_box(void) {
	double delta = 0.000001;
	double cos_x = normal * Vector3D(1, 0, 0);
	double cos_y = normal * Vector3D(0, 1, 0);
	double cos_z = normal * Vector3D(0, 0, 1);

	return (BBox(center.x - radius*fabs(cos_x) - delta, center.x + radius*fabs(cos_x) + delta,
				 center.y - radius*fabs(cos_y) - delta, center.y + radius*fabs(cos_y) + delta,
				 center.z - radius*fabs(cos_z) - delta, center.z + radius*fabs(cos_z) + delta));
}


// ----------------------------------------------------------------- hit

bool
Disk::hit(const Ray& ray, double& tmin, ShadeRec& sr) const {
    // 计算光线和圆形对象所在平面的碰撞点。推导过程如下：
    //     所谓的平面，定义方式为(a, n)。也就是该平面经过点a，且垂直于法线方向n。
    //     因此上，平面的任意一点p都满足：
    //         (p - a) * n = 0 
    //     即，从a到p的向量和法线方向n垂直。
    //     代入光线方程： p = o + td有：
    //          (o + td - a) * n = 0
    //     则：
    //           t = (a - o) * n / (d * n)
    //     即为下面的公式。
    // 也可以参见公式2.6和3.4
	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);
    // 根据上面计算出来的t算出来碰撞点。
	Point3D p = ray.o + t * ray.d;
    // 如果碰撞点和圆心的距离小于半径。
	if (center.d_squared(p) < (radius*radius)) {
        // 返回碰撞点，碰撞点法线方向，对应的t值。
		tmin 				= t;
		sr.normal 			= normal;
		sr.local_hit_point	= p;
		return (true);
	}
	else
		return (false);
}


// ----------------------------------------------------------------- shadow_hit

bool
Disk::shadow_hit(const Ray& ray, float& tmin) const {
    // 计算光线和圆形对象所在平面的碰撞点。推导过程如下：
    //     所谓的平面，定义方式为(a, n)。也就是该平面经过点a，且垂直于法线方向n。
    //     因此上，平面的任意一点p都满足：
    //         (p - a) * n = 0 
    //     即，从a到p的向量和法线方向n垂直。
    //     代入光线方程： p = o + td有：
    //          (o + td - a) * n = 0
    //     则：
    //           t = (a - o) * n / (d * n)
    //     即为下面的公式。
    // 也可以参见公式2.6和3.4
	double t = (center - ray.o) * normal / (ray.d * normal);

	if (t <= kEpsilon)
		return (false);

    // 根据上面计算出来的t算出来碰撞点。
	Point3D p = ray.o + t * ray.d;

    // 如果碰撞点和圆心的距离小于半径。
	if (center.d_squared(p) < (radius*radius)) {
		tmin 				= t;
		return (true);
	}
	else
		return (false);
}


// ---------------------------------------------------------------- setSampler

// 函数将使用一个采样器对象指针作为参数，该函数对于采样器的类型并没有任何要求。
void
Disk::set_sampler(Sampler* sampler) {
	sampler_ptr = sampler;
    // 同心映射(Shirley): 将位于正方形内的采样点映射至对应的同心圆上，
    // 并且可将映射偏差控制在较低的范围内。
	sampler_ptr->map_samples_to_unit_disk();
}


// ---------------------------------------------------------------- sample
// returns a sample point on the disk
// just for disks with normal(0, 1, 0) or normal(0, -1, 0)

Point3D
Disk::sample(void) {
    // 返回存储于采样器对象中的下一个采样点，映射到单位圆。
    // 因为圆心对象是一个平面对象，所以sp.z不使用。
	Point2D sample_point = sampler_ptr->sample_unit_disk();
    // 这个采样点是位于圆形对象坐标系中的。
    // 我们把这个采样点换算到世界坐标系中。
	return (center + Vector3D(sample_point.x * radius, 0, sample_point.y *radius));
}


//------------------------------------------------------------------ get_normal

Normal
Disk::get_normal(const Point3D& p) {
	return (normal);
}


// ---------------------------------------------------------------- pdf
float
Disk::pdf(ShadeRec& sr) {
    // 对于均匀光源来说，pdf为表面积的倒数。尤其是平面光源。
	return (inv_area);
}

