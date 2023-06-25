#include "StdAfx.h"
#include "Transform3.h"

CTransform3::CTransform3(void)
{

}

CTransform3::~CTransform3(void)
{

}

void CTransform3::SetMatrix(CP3* P, int ptNumber)
{
	this->P = P;
	this->ptNumber = ptNumber;
}

void CTransform3::Identity(void)//单位矩阵
{
	M[0][0] = 1.0, M[0][1] = 0.0, M[0][2] = 0.0, M[0][3] = 0.0;
	M[1][0] = 0.0, M[1][1] = 1.0, M[1][2] = 0.0, M[1][3] = 0.0;
	M[2][0] = 0.0, M[2][1] = 0.0, M[2][2] = 1.0, M[2][3] = 0.0;
	M[3][0] = 0.0, M[3][1] = 0.0, M[3][2] = 0.0, M[3][3] = 1.0;
}

void CTransform3::Translate(double tx, double ty, double tz)//平移变换
{
	Identity();
	M[0][3] = tx;
	M[1][3] = ty;
	M[2][3] = tz;
	MultiplyMatrix();
}

void CTransform3::Scale(double sx, double sy, double sz)//比例变换
{
	Identity();
	M[0][0] = sx;
	M[1][1] = sy;
	M[2][2] = sz;
	MultiplyMatrix();
}

void CTransform3::Scale(double sx, double sy, double sz, CP3 p)//相对于任意点的比例变换
{
	Translate(-p.x, -p.y, -p.z);
	Scale(sx, sy, sz);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateX(double beta)//绕X轴旋转变换
{
	Identity();
	double rad = beta * PI / 180;
	M[1][1] = cos(rad), M[1][2] =-sin(rad);
	M[2][1] = sin(rad), M[2][2] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateX(double beta, CP3 p)//相对于任意点的绕X轴旋转变换
{
	Translate(-p.x, -p.y, -p.z);
	RotateX(beta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateY(double beta)//绕Y轴旋转变换
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad), M[0][2] = sin(rad);
	M[2][0] =-sin(rad), M[2][2] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateY(double beta, CP3 p)//相对于任意点的绕Y轴旋转变换
{
	Translate(-p.x, -p.y, -p.z);
	RotateY(beta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::RotateZ(double beta)//绕Z轴旋转变换
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad), M[0][1] =-sin(rad);
	M[1][0] = sin(rad), M[1][1] = cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateZ(double beta, CP3 p)//相对于任意点的绕Z轴旋转变换
{
	Translate(-p.x, -p.y, -p.z);
	RotateZ(beta);
	Translate(p.x, p.y, p.z);
}

void CTransform3::ReflectX(void)//X轴反射变换
{
	Identity();
	M[1][1] = -1, M[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectY(void)//Y轴反射变换
{
	Identity();
	M[0][0] = -1, M[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectZ(void)//Z轴反射变换
{
	Identity();
	M[0][0] = -1, M[1][1] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectXOY(void)//XOY面反射变换
{
	Identity();
	M[2][2] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectYOZ(void)//YOZ面反射变换
{
	Identity();
	M[0][0] = -1;
	MultiplyMatrix();
}

void CTransform3::ReflectZOX(void)//ZOX面反射变换
{
	Identity();
	M[1][1] = -1;
	MultiplyMatrix();
}

void CTransform3::ShearX(double b, double c)//X方向错切变换
{
	Identity();
	M[0][1] = b, M[0][2] = c;
	MultiplyMatrix();
}

void CTransform3::ShearY(double d, double f)//Y方向错切变换
{
	Identity();
	M[1][0] = d, M[1][2] = f;
	MultiplyMatrix();
}

void CTransform3::ShearZ(double g, double h)//Z方向错切变换
{
	Identity();
	M[2][0] = g, M[2][1] = h;
	MultiplyMatrix();
}

void CTransform3::MultiplyMatrix(void)//矩阵相乘
{
	CP3* PTemp = new CP3[ptNumber];
	for (int i = 0; i < ptNumber; i++)
		PTemp[i] = P[i];
	for (i = 0; i < ptNumber; i++)
	{
		P[i].x = M[0][0] * PTemp[i].x + M[0][1] * PTemp[i].y + M[0][2] * PTemp[i].z + M[0][3] * PTemp[i].w;
		P[i].y = M[1][0] * PTemp[i].x + M[1][1] * PTemp[i].y + M[1][2] * PTemp[i].z + M[1][3] * PTemp[i].w;
		P[i].z = M[2][0] * PTemp[i].x + M[2][1] * PTemp[i].y + M[2][2] * PTemp[i].z + M[2][3] * PTemp[i].w;
		P[i].w = M[3][0] * PTemp[i].x + M[3][1] * PTemp[i].y + M[3][2] * PTemp[i].z + M[3][3] * PTemp[i].w;
	}
	delete[]PTemp;
}

void CTransform3::LeftMultiplyMatrix(double M1[4][4], double M2[4][4])//左乘矩阵M2 = M1 * M2
{
	double MTemp[4][4];//临时矩阵
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			MTemp[i][j] = M1[i][0] * M2[0][j] + M1[i][1] * M2[1][j] + M1[i][2] * M2[2][j] + M1[i][3] * M2[3][j];
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M2[i][j] = MTemp[i][j];
}

void CTransform3::ArbitraryDirection(double beta, double nx, double ny, double nz)//绕任意方向的旋转
{//beta为旋转角，n={nx，ny，nz}为方向矢量，4个参数构成4元数
	double Beta = double(beta * PI / 180);
	double M1[4][4], M2[4][4], M3[4][4], M4[4][4], M5[4][4]; 
	double sinBetaX = ny;
	double cosBetaX = sqrt(nx * nx + nz * nz);
	double sinBetaY = nx / cosBetaX;
	double cosBetaY = nz / cosBetaX;

	M5[0][0] = cosBetaY,  M5[0][1] = 0,  M5[0][2] = sinBetaY,  M5[0][3] = 0;
	M5[1][0] = 0,         M5[1][1] = 1,  M5[1][2] = 0,         M5[1][3] = 0;
	M5[2][0] = -sinBetaY, M5[2][1] = 0,  M5[2][2] = cosBetaY,  M5[2][3] = 0;
	M5[3][0] = 0,         M5[3][1] = 0,  M5[3][2] = 0,         M5[3][3] = 1;
	
	M4[0][0] = 1, M4[0][1] = 0,         M4[0][2] = 0,          M4[0][3] = 0;
	M4[1][0] = 0, M4[1][1] = cosBetaX,  M4[1][2] = sinBetaX,   M4[1][3] = 0;
	M4[2][0] = 0, M4[2][1] = -sinBetaX, M4[2][2] = cosBetaX,   M4[2][3] = 0;
	M4[3][0] = 0, M4[3][1] = 0,         M4[3][2] = 0,          M4[3][3] = 1;
	LeftMultiplyMatrix(M5, M4);

	M3[0][0] = cos(Beta),  M3[0][1] = -sin(Beta), M3[0][2] = 0, M3[0][3] = 0;
	M3[1][0] = sin(Beta),  M3[1][1] = cos(Beta),  M3[1][2] = 0, M3[1][3] = 0;
	M3[2][0] = 0,          M3[2][1] = 0,          M3[2][2] = 1, M3[2][3] = 0;
	M3[3][0] = 0,          M3[3][1] = 0,          M3[3][2] = 0, M3[3][3] = 1;
	LeftMultiplyMatrix(M4, M3);

	M2[0][0] = 1, M2[0][1] = 0,        M2[0][2] = 0,         M2[0][3] = 0;
	M2[1][0] = 0, M2[1][1] = cosBetaX, M2[1][2] = -sinBetaX, M2[1][3] = 0;
	M2[2][0] = 0, M2[2][1] = sinBetaX, M2[2][2] = cosBetaX,  M2[2][3] = 0;
	M2[3][0] = 0, M2[3][1] = 0,        M2[3][2] = 0,         M2[3][3] = 1;
	LeftMultiplyMatrix(M3, M2);

	M1[0][0] = cosBetaY, M1[0][1] = 0, M1[0][2] = -sinBetaY, M1[0][3] = 0;
	M1[1][0] = 0,        M1[1][1] = 1, M1[1][2] = 0,         M1[1][3] = 0;
	M1[2][0] = sinBetaY, M1[2][1] = 0, M1[2][2] = cosBetaY,  M1[2][3] = 0;
	M1[3][0] = 0,        M1[3][1] = 0, M1[3][2] = 0,         M1[3][3] = 1;
	LeftMultiplyMatrix(M2, M1);
	for (int i = 0; i < 4; i++)//将矩阵脸乘结果转储到变换矩阵M中
		for (int j = 0; j < 4; j++)
			M[i][j] = M1[i][j];	
	MultiplyMatrix();
}