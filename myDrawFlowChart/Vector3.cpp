#include "stdafx.h"
#include "Vector3.h"
#include "math.h"

CVector3::CVector3(void)
{
	x = 0.0,y = 0.0, z = 1.0;//指向z轴正向
}


CVector3::~CVector3(void)
{
}

CVector3::CVector3(double x, double y, double z)//绝对矢量
{
	this->x = x;
	this->y = y;
	this->z = z;	
}

CVector3::CVector3(const CP3 &p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

CVector3::CVector3(const CP3 &p0, const CP3 &p1)//相对矢量
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}


CVector3::CVector3(const CColorP3 &p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

CVector3::CVector3(const CColorP3 &p0, const CColorP3 &p1)//相对矢量
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}

double CVector3::Magnitude(void)//矢量的模
{
	return sqrt(x * x + y * y + z * z);
}

CVector3 CVector3::Normalize(void)//归一化为单位矢量
{
	CVector3 vector;
	double magnitude = sqrt(x * x + y * y + z * z);
	if(fabs(magnitude) < 1e-4)
		magnitude  = 1.0;
	vector.x = x / magnitude;
	vector.y = y / magnitude;
	vector.z = z / magnitude;
	return vector;
}

CVector3 operator + (const CVector3 &v0, const CVector3 &v1)//矢量的和
{
	CVector3 vector;
	vector.x = v0.x + v1.x;
	vector.y = v0.y + v1.y;
	vector.z = v0.z + v1.z;
	return vector;
}

CVector3 operator - (const CVector3 &v0, const CVector3 &v1)//矢量的差
{
	CVector3 vector;
	vector.x = v0.x - v1.x;
	vector.y = v0.y - v1.y;
	vector.z = v0.z - v1.z;
	return vector;
}

CVector3 operator * (const CVector3 &v, double scalar)//矢量与常量的积
{
	CVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CVector3 operator * (double scalar, const CVector3 &v)//常量与矢量的积
{
	CVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CVector3 operator / (const CVector3 &v, double scalar)//矢量数除
{
	if(fabs(scalar) < 1e-4)
		scalar = 1.0;
	CVector3 vector;
	vector.x = v.x / scalar;
	vector.y = v.y / scalar;
	vector.z = v.z / scalar;
	return vector;
}

double DotProduct(const CVector3 &v0, const CVector3 &v1)//矢量的点积
{
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

/************************************************************************/
/* 下面复习一下：                                                       */
/* 首先叉乘是这么定义。                                                 */
/*     c = a × b = |a||b| * sin(cita)                                  */
/* 其中cita是a和b的夹角。c的模为以ab为两条边的平行四边形的面积。        */
/* 进一步有下面的几条性质：                                             */
/*   1. a × b = -b × a。                                              */
/*   2. a × a = 0。因为夹角cita为0，因此上sin(cita)也为零。            */
/*   3. a × (b + c) = a × b + a × c。                                */
/* 那么对于空间中的两个向量：                                           */
/*   a = ax * i + ay * j + az * k                                       */
/*   b = bx * i + by * j + bz * k                                       */
/* 则 a × b = (ax * i + ay * j + az * k)×(bx * i + by * j + bz * k)   */
/*           = ax * bx * i * i + ay * bx * j * i + az * bx * k * i +    */
/*             ax * by * i * j + ay * by * j * j + az * by * k * j +    */
/*             ax * bz * i * k + ay * bz * j * k + az * bz * k * k +    */
/* 根据公式2可知： i * i = j * j = k * k = 0。在结合公式1，则原式等于： */
/*             (ax * by - ay * bx) i * j +                              */
/*             (az * bx - ax * bz) k * i +                              */
/*             (ay * bz - az * by) j * k +                              */
/* 根据定义可知： i * j = k, k * i = j, k * i = j。则原式等于：         */
/*             (ay * bz - az * by) i +                                  */
/*             (az * bx - ax * bz) j +                                  */
/*             (ax * by - ay * bx) k                                    */
/* 这就是下面这个代码的计算公式。                                       */
/************************************************************************/
CVector3 CrossProduct(const CVector3 &v0, const CVector3 &v1) // 矢量的叉积
{
	CVector3 vector;
	vector.x = v0.y * v1.z - v0.z * v1.y;
	vector.y = v0.z * v1.x - v0.x * v1.z;
	vector.z = v0.x * v1.y - v0.y * v1.x;
	return vector;
}
