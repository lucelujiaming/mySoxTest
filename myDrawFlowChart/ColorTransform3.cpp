#include "StdAfx.h"
#include "ColorTransform3.h"

CColorTransform3::CColorTransform3(void)
{

}

CColorTransform3::~CColorTransform3(void)
{

}

void CColorTransform3::SetMatrix(CColorP3* P, int ptNumber)
{
	this->P = P;
	this->ptNumber = ptNumber;
}

void CColorTransform3::Identity(void)//��λ����
{
	M[0][0] = 1.0, M[0][1] = 0.0, M[0][2] = 0.0, M[0][3] = 0.0;
	M[1][0] = 0.0, M[1][1] = 1.0, M[1][2] = 0.0, M[1][3] = 0.0;
	M[2][0] = 0.0, M[2][1] = 0.0, M[2][2] = 1.0, M[2][3] = 0.0;
	M[3][0] = 0.0, M[3][1] = 0.0, M[3][2] = 0.0, M[3][3] = 1.0;
}

void CColorTransform3::Translate(double tx, double ty, double tz)//ƽ�Ʊ任
{
	Identity();
	M[0][3] = tx;
	M[1][3] = ty;
	M[2][3] = tz;
	MultiplyMatrix();
}

void CColorTransform3::Scale(double sx, double sy, double sz)//�����任
{
	Identity();
	M[0][0] = sx;
	M[1][1] = sy;
	M[2][2] = sz;
	MultiplyMatrix();
}

void CColorTransform3::Scale(double sx, double sy, double sz, CColorP3 p)//����������ı����任
{
	Translate(-p.x, -p.y, -p.z);
	Scale(sx, sy, sz);
	Translate(p.x, p.y, p.z);
}

void CColorTransform3::RotateX(double beta)//��X����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[1][1] = cos(rad), M[1][2] =-sin(rad);
	M[2][1] = sin(rad), M[2][2] = cos(rad);
	MultiplyMatrix();
}

void CColorTransform3::RotateX(double beta, CColorP3 p)//�������������X����ת�任
{
	Translate(-p.x, -p.y, -p.z);
	RotateX(beta);
	Translate(p.x, p.y, p.z);
}

void CColorTransform3::RotateY(double beta)//��Y����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad), M[0][2] = sin(rad);
	M[2][0] =-sin(rad), M[2][2] = cos(rad);
	MultiplyMatrix();
}

void CColorTransform3::RotateY(double beta, CColorP3 p)//�������������Y����ת�任
{
	Translate(-p.x, -p.y, -p.z);
	RotateY(beta);
	Translate(p.x, p.y, p.z);
}

void CColorTransform3::RotateZ(double beta)//��Z����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad), M[0][1] =-sin(rad);
	M[1][0] = sin(rad), M[1][1] = cos(rad);
	MultiplyMatrix();
}

void CColorTransform3::RotateZ(double beta, CColorP3 p)//�������������Z����ת�任
{
	Translate(-p.x, -p.y, -p.z);
	RotateZ(beta);
	Translate(p.x, p.y, p.z);
}

void CColorTransform3::ReflectX(void)//X�ᷴ��任
{
	Identity();
	M[1][1] = -1, M[2][2] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ReflectY(void)//Y�ᷴ��任
{
	Identity();
	M[0][0] = -1, M[2][2] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ReflectZ(void)//Z�ᷴ��任
{
	Identity();
	M[0][0] = -1, M[1][1] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ReflectXOY(void)//XOY�淴��任
{
	Identity();
	M[2][2] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ReflectYOZ(void)//YOZ�淴��任
{
	Identity();
	M[0][0] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ReflectZOX(void)//ZOX�淴��任
{
	Identity();
	M[1][1] = -1;
	MultiplyMatrix();
}

void CColorTransform3::ShearX(double b, double c)//X������б任
{
	Identity();
	M[0][1] = b, M[0][2] = c;
	MultiplyMatrix();
}

void CColorTransform3::ShearY(double d, double f)//Y������б任
{
	Identity();
	M[1][0] = d, M[1][2] = f;
	MultiplyMatrix();
}

void CColorTransform3::ShearZ(double g, double h)//Z������б任
{
	Identity();
	M[2][0] = g, M[2][1] = h;
	MultiplyMatrix();
}

void CColorTransform3::MultiplyMatrix(void)//�������
{
	CColorP3* PTemp = new CColorP3[ptNumber];
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

void CColorTransform3::LeftMultiplyMatrix(double M1[4][4], double M2[4][4])//��˾���M2 = M1 * M2
{
	double MTemp[4][4];//��ʱ����
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			MTemp[i][j] = M1[i][0] * M2[0][j] + M1[i][1] * M2[1][j] + M1[i][2] * M2[2][j] + M1[i][3] * M2[3][j];
	for (i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			M2[i][j] = MTemp[i][j];
}

void CColorTransform3::ArbitraryDirection(double beta, double nx, double ny, double nz)//�����ⷽ�����ת
{//betaΪ��ת�ǣ�n={nx��ny��nz}Ϊ����ʸ����4����������4Ԫ��
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
	for (int i = 0; i < 4; i++)//���������˽��ת�����任����M��
		for (int j = 0; j < 4; j++)
			M[i][j] = M1[i][j];	
	MultiplyMatrix();
}