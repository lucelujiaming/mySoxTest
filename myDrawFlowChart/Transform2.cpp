#include "stdafx.h"
#include "Transform2.h"
#include "math.h"

#define PI 3.1415926//圆周率

CTransform2::CTransform2(void)
{

}

CTransform2::~CTransform2(void)
{

}

void CTransform2::SetMatrix(CP2* P,int ptNumber)
{
	this->P = P;
	this->ptNumber = ptNumber;
}

void CTransform2::Identity(void)//单位矩阵
{
	M[0][0] = 1.0, M[0][1] = 0.0, M[0][2] = 0.0;
	M[1][0] = 0.0, M[1][1] = 1.0, M[1][2] = 0.0;
	M[2][0] = 0.0, M[2][1] = 0.0, M[2][2] = 1.0;
}

void CTransform2::Translate(double tx, double ty)//平移变换矩阵
{
	Identity();
	M[0][2] = tx; 
	M[1][2] = ty; 
	MultiplyMatrix();	
}

void CTransform2::Scale(double sx, double sy)//比例变换矩阵
{
	Identity();
	M[0][0] = sx; 
	M[1][1] = sy; 
	MultiplyMatrix();	
}

void CTransform2::Scale(double sx,double sy, CP2 p)//相对于任意点的比例变换矩阵
{
	Translate(-p.x, -p.y);
	Scale(sx, sy);
	Translate(p.x, p.y);	
}

void CTransform2::Rotate(double beta)//旋转变换矩阵
{
	Identity();
	M[0][0] = cos(beta * PI/180), M[0][1] =-sin(beta * PI/180);
	M[1][0] = sin(beta * PI/180), M[1][1] = cos(beta * PI/180);
	MultiplyMatrix();	
}

void CTransform2::Rotate(double beta, CP2 p)//相对于任意点的旋转变换矩阵
{
	Translate(-p.x, -p.y);
	Rotate(beta);
	Translate(p.x, p.y);	
}

void CTransform2::ReflectOrg(void)//原点反射变换矩阵
{
	Identity();
	M[0][0] = -1;
	M[1][1] = -1;
	MultiplyMatrix();
}

void CTransform2::ReflectX(void)//X轴反射变换矩阵
{
	Identity();
	M[0][0] =  1;
	M[1][1] = -1;
	MultiplyMatrix();	
}

void CTransform2::ReflectY(void)//Y轴反射变换矩阵
{
	Identity();
	M[0][0]=-1;
	M[1][1]=1;
	MultiplyMatrix();	
}

void CTransform2::Shear(double b,double c)//错切变换矩阵
{
	Identity();
	M[0][1]=b;
	M[1][0]=c;
	MultiplyMatrix();	
}

void CTransform2::MultiplyMatrix(void)//矩阵相乘
{
	CP2* PTemp = new CP2[ptNumber];
	for(int i = 0;i < ptNumber;i++)
		PTemp[i] = P[i];
	for( i = 0;i < ptNumber;i++)
	{
		P[i].x = M[0][0] * PTemp[i].x + M[0][1] * PTemp[i].y + M[0][2] * PTemp[i].w;
		P[i].y = M[1][0] * PTemp[i].x + M[1][1] * PTemp[i].y + M[1][2] * PTemp[i].w;
		P[i].w = M[2][0] * PTemp[i].x + M[2][1] * PTemp[i].y + M[2][2] * PTemp[i].w;
	}
	delete []PTemp;
}