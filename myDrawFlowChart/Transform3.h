// Transform3.h: interface for the Transform3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORM3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)
#define AFX_TRANSFORM3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P3.h"
#include"math.h"
#define PI 3.1415926

class CTransform3
{
public:
	CTransform3(void);
	~CTransform3(void);
	void SetMatrix(CP3*P,int ptNumber);
	void Identity();//初始变换单位矩阵
	void Translate(double tx,double ty,double tz);//平移变换矩阵
	void RotateX(double beta);//绕x轴旋转变换矩阵
	void RotateY(double beta);//绕x轴旋转变换矩阵
	void RotateZ(double beta);//绕x轴旋转变换矩阵
	void MultiplyMatrix();//变换矩阵左乘顶点列阵
public:
	double T[4][4];//三维变换矩阵
	CP3*P;//动态数组定义顶点列阵
	int ptNumber;//顶点个数
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)

