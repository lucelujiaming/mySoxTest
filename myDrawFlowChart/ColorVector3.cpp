#include "stdafx.h"
#include "ColorVector3.h"
#include "math.h"

CColorVector3::CColorVector3(void)
{
	x = 0.0,y = 0.0, z = 1.0;//指向z轴正向
}


CColorVector3::~CColorVector3(void)
{
}

// 使用(x, y, z)定义的绝对矢量进行初始化。
CColorVector3::CColorVector3(double x, double y, double z) 
{
	this->x = x;
	this->y = y;
	this->z = z;	
}

CColorVector3::CColorVector3(const CColorP3 &p)
{
	x = p.x;
	y = p.y;
	z = p.z;
}

// 使用p1 - p0定义的相对矢量进行初始化。
CColorVector3::CColorVector3(const CColorP3 &p0, const CColorP3 &p1) 
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}

// 矢量的模
double CColorVector3::Magnitude(void)
{
	return sqrt(x * x + y * y + z * z);
}

// 归一化为单位矢量
CColorVector3 CColorVector3::Normalize(void) 
{
	CColorVector3 vector;
	double magnitude = sqrt(x * x + y * y + z * z);
	if(fabs(magnitude) < 1e-4)
    {
		magnitude  = 1.0;
    }
	vector.x = x / magnitude;
	vector.y = y / magnitude;
	vector.z = z / magnitude;
	return vector;
}

// 矢量的和
CColorVector3 operator + (const CColorVector3 &v0, const CColorVector3 &v1)
{
	CColorVector3 vector;
	vector.x = v0.x + v1.x;
	vector.y = v0.y + v1.y;
	vector.z = v0.z + v1.z;
	return vector;
}

// 矢量的差
CColorVector3 operator - (const CColorVector3 &v0, const CColorVector3 &v1)
{
	CColorVector3 vector;
	vector.x = v0.x - v1.x;
	vector.y = v0.y - v1.y;
	vector.z = v0.z - v1.z;
	return vector;
}

// 矢量与常量的积
CColorVector3 operator * (const CColorVector3 &v, double scalar)
{
	CColorVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

// 常量与矢量的积
CColorVector3 operator * (double scalar, const CColorVector3 &v)
{
	CColorVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

// 矢量数除
CColorVector3 operator / (const CColorVector3 &v, double scalar)
{
	if(fabs(scalar) < 1e-4)
    {
		scalar = 1.0;
    }
	CColorVector3 vector;
	vector.x = v.x / scalar;
	vector.y = v.y / scalar;
	vector.z = v.z / scalar;
	return vector;
}

/************************************************************************/
/* 下面复习一下：                                                       */
/* 首先矢量的点积是这么定义。                                           */
/* 向量的点乘，也叫向量的内积、数量积，对两个向量执行点乘运算，         */
/* 就是对这两个向量对应位一一相乘之后求和的操作，点乘的结果是一个标量。 */
/* 点乘的几何意义是可以用来表征或计算两个向量之间的夹角，               */
/* 以及在b向量在a向量方向上的投影，有公式：                             */
/*     a和b的点积 = |a| * |b| * cos(cita)                               */
/* 推导过程如下，首先假设有两个向量a和b。定义：                         */
/*     c = a - b                                                        */
/* 根据三角形余弦定理有：                                               */
/*     c^2 = a^2 + b^2 - 2 * |a| * |b| * cos(cita)                      */
/* 根据关系c=a-b（a、b、c均为向量）有：                                 */
/*     (a - b)和(a - b)的点积 = a^2 + b^2 - 2 * (a和b的点积)            */
/*                            = a^2 + b^2 - 2 * |a| * |b| * cos(cita)   */
/* 根据这个公式就可以计算向量a和向量b之间的夹角。                       */
/* 从而就可以进一步判断这两个向量是否是同一方向，                       */
/* 是否正交(也就是垂直)等方向关系。即：                                 */
/*     a和b的点积大于零：方向基本相同，夹角在0°到90°之间。              */
/*     a和b的点积等于零：正交，相互垂直。                               */
/*     a和b的点积小于零：方向基本相反，夹角在90°到180°之间。            */
/************************************************************************/
double DotProduct(const CColorVector3 &v0, const CColorVector3 &v1) 
{
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

/************************************************************************/
/* 下面复习一下：                                                       */
/* 首先矢量的叉乘是这么定义。                                           */
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
/*          = ax * bx * i * i + ay * bx * j * i + az * bx * k * i +    */
/*            ax * by * i * j + ay * by * j * j + az * by * k * j +    */
/*            ax * bz * i * k + ay * bz * j * k + az * bz * k * k +    */
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
/* 证明两个矢量的叉乘垂直于这两个矢量也很简单：                         */
/*                   | ay * bz - az * by |                              */
/*    | ax ay az | * | az * bx - ax * bz | = | 0 |                      */
/*    | bx by bz |   | ax * by - ay * bx }   | 0 |                      */
/* 两个矢量的乘积为零，即为垂直。                                       */
/************************************************************************/
CColorVector3 CrossProduct(const CColorVector3 &v0, const CColorVector3 &v1)//矢量的叉积
{
	CColorVector3 vector;
	vector.x = v0.y * v1.z - v0.z * v1.y;
	vector.y = v0.z * v1.x - v0.x * v1.z;
	vector.z = v0.x * v1.y - v0.y * v1.x;
	return vector;
}
