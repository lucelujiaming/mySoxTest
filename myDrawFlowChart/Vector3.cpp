#include "stdafx.h"
#include "Vector3.h"
#include "math.h"

CVector3::CVector3(void)
{
	x = 0.0,y = 0.0, z = 1.0;//ָ��z������
}


CVector3::~CVector3(void)
{
}

// ʹ��(x, y, z)����ľ���ʸ�����г�ʼ����
CVector3::CVector3(double x, double y, double z)
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

CVector3::CVector3(const CP3 &p0, const CP3 &p1)//���ʸ��
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

// ʹ��p1 - p0��������ʸ�����г�ʼ����
CVector3::CVector3(const CColorP3 &p0, const CColorP3 &p1)
{
	x = p1.x - p0.x;
	y = p1.y - p0.y;
	z = p1.z - p0.z;
}

double CVector3::Magnitude(void)//ʸ����ģ
{
	return sqrt(x * x + y * y + z * z);
}

// ��һ��Ϊ��λʸ��
CVector3 CVector3::Normalize(void)
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

CVector3 operator + (const CVector3 &v0, const CVector3 &v1)//ʸ���ĺ�
{
	CVector3 vector;
	vector.x = v0.x + v1.x;
	vector.y = v0.y + v1.y;
	vector.z = v0.z + v1.z;
	return vector;
}

CVector3 operator - (const CVector3 &v0, const CVector3 &v1)//ʸ���Ĳ�
{
	CVector3 vector;
	vector.x = v0.x - v1.x;
	vector.y = v0.y - v1.y;
	vector.z = v0.z - v1.z;
	return vector;
}

CVector3 operator * (const CVector3 &v, double scalar)//ʸ���볣���Ļ�
{
	CVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CVector3 operator * (double scalar, const CVector3 &v)//������ʸ���Ļ�
{
	CVector3 vector;
	vector.x = v.x * scalar;
	vector.y = v.y * scalar;
	vector.z = v.z * scalar;
	return vector;
}

CVector3 operator / (const CVector3 &v, double scalar)//ʸ������
{
	if(fabs(scalar) < 1e-4)
		scalar = 1.0;
	CVector3 vector;
	vector.x = v.x / scalar;
	vector.y = v.y / scalar;
	vector.z = v.z / scalar;
	return vector;
}

/************************************************************************/
/* ���渴ϰһ�£�                                                       */
/* ����ʸ���ĵ������ô���塣                                           */
/* �����ĵ�ˣ�Ҳ���������ڻ���������������������ִ�е�����㣬         */
/* ���Ƕ�������������Ӧλһһ���֮����͵Ĳ�������˵Ľ����һ�������� */
/* ��˵ļ��������ǿ������������������������֮��ļнǣ�               */
/* �Լ���b������a���������ϵ�ͶӰ���й�ʽ��                             */
/*     a��b�ĵ�� = |a| * |b| * cos(cita)                               */
/* �Ƶ��������£����ȼ�������������a��b�����壺                         */
/*     c = a - b                                                        */
/* �������������Ҷ����У�                                               */
/*     c^2 = a^2 + b^2 - 2 * |a| * |b| * cos(cita)                      */
/* ���ݹ�ϵc=a-b��a��b��c��Ϊ�������У�                                 */
/*     (a - b)��(a - b)�ĵ�� = a^2 + b^2 - 2 * (a��b�ĵ��)            */
/*                            = a^2 + b^2 - 2 * |a| * |b| * cos(cita)   */
/* ���������ʽ�Ϳ��Լ�������a������b֮��ļнǡ�                       */
/* �Ӷ��Ϳ��Խ�һ���ж������������Ƿ���ͬһ����                       */
/* �Ƿ�����(Ҳ���Ǵ�ֱ)�ȷ����ϵ������                                 */
/*     a��b�ĵ�������㣺���������ͬ���н���0�㵽90��֮�䡣              */
/*     a��b�ĵ�������㣺�������໥��ֱ��                               */
/*     a��b�ĵ��С���㣺��������෴���н���90�㵽180��֮�䡣            */
/************************************************************************/
double DotProduct(const CVector3 &v0, const CVector3 &v1)
{
	return(v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
}

/************************************************************************/
/* ���渴ϰһ�£�                                                       */
/* ����ʸ���Ĳ������ô���塣                                           */
/*     c = a �� b = |a||b| * sin(cita)                                  */
/* ����cita��a��b�ļнǡ�c��ģΪ��abΪ�����ߵ�ƽ���ı��ε������        */
/* ��һ��������ļ������ʣ�                                             */
/*   1. a �� b = -b �� a��                                              */
/*   2. a �� a = 0����Ϊ�н�citaΪ0�������sin(cita)ҲΪ�㡣            */
/*   3. a �� (b + c) = a �� b + a �� c��                                */
/* ��ô���ڿռ��е�����������                                           */
/*   a = ax * i + ay * j + az * k                                       */
/*   b = bx * i + by * j + bz * k                                       */
/* �� a �� b = (ax * i + ay * j + az * k)��(bx * i + by * j + bz * k)   */
/*           = ax * bx * i * i + ay * bx * j * i + az * bx * k * i +    */
/*             ax * by * i * j + ay * by * j * j + az * by * k * j +    */
/*             ax * bz * i * k + ay * bz * j * k + az * bz * k * k +    */
/* ���ݹ�ʽ2��֪�� i * i = j * j = k * k = 0���ڽ�Ϲ�ʽ1����ԭʽ���ڣ� */
/*             (ax * by - ay * bx) i * j +                              */
/*             (az * bx - ax * bz) k * i +                              */
/*             (ay * bz - az * by) j * k +                              */
/* ���ݶ����֪�� i * j = k, k * i = j, k * i = j����ԭʽ���ڣ�         */
/*             (ay * bz - az * by) i +                                  */
/*             (az * bx - ax * bz) j +                                  */
/*             (ax * by - ay * bx) k                                    */
/* ����������������ļ��㹫ʽ��                                       */
/************************************************************************/
CVector3 CrossProduct(const CVector3 &v0, const CVector3 &v1) // ʸ���Ĳ��
{
	CVector3 vector;
	vector.x = v0.y * v1.z - v0.z * v1.y;
	vector.y = v0.z * v1.x - v0.x * v1.z;
	vector.z = v0.x * v1.y - v0.y * v1.x;
	return vector;
}
