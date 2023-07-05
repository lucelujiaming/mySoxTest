#if !defined(AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Vector3.h"

class CNormalRotation//法矢量旋转类
{
public:
	CNormalRotation(void);
	virtual ~CNormalRotation(void);
	void Identity(void);//单位矩阵
	void SetMatrix(CVector3* N, int nNumber);//法矢量数组初始化
	void RotateX(double beta);//绕X轴旋转变换
	void RotateY(double beta);//绕Y轴旋转变换
	void RotateZ(double beta);//绕Z轴旋转变换
	void InverseMatrix(double M[3][3]);//逆矩阵
	void TransposeMatrix(double M[3][3]);//转置矩阵
	void MultiplyMatrix(void);//矩阵相乘
public:
	double M[3][3];//法矢量变换矩阵
	CVector3* N;//法矢量数组名
	int	nNormalNumber;//法矢量个数
};

#endif // !defined(AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
