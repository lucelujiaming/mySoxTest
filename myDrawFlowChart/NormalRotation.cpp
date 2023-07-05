#include "stdafx.h"
#include "NormalRotation.h"
#include "math.h"
#define PI 3.1415926//Բ����

CNormalRotation::CNormalRotation(void)
{

}

CNormalRotation::~CNormalRotation(void)
{

}

void CNormalRotation::Identity(void)//��λ�����ʼ��
{
	M[0][0] = 1.0, M[0][1] = 0.0, M[0][2] = 0.0;
	M[1][0] = 0.0, M[1][1] = 1.0, M[1][2] = 0.0;
	M[2][0] = 0.0, M[2][1] = 0.0, M[2][2] = 1.0;	
}

void CNormalRotation::SetMatrix(CVector3* N, int nNormalNumber)//��ʸ�������ʼ��
{
	this->N = N;
	this->nNormalNumber = nNormalNumber;
}

void CNormalRotation::RotateX(double beta)//��X����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[1][1] = cos(rad), M[1][2] = -sin(rad);
	M[2][1] = sin(rad), M[2][2] = cos(rad);
	InverseMatrix(M);
	TransposeMatrix(M);
	MultiplyMatrix();
}

void CNormalRotation::RotateY(double beta)//��Y����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad),  M[0][2] = sin(rad);
	M[2][0] = -sin(rad), M[2][2] = cos(rad);
	InverseMatrix(M);
	TransposeMatrix(M);
	MultiplyMatrix();
}

void CNormalRotation::RotateZ(double beta)//��Z����ת�任
{
	Identity();
	double rad = beta * PI / 180;
	M[0][0] = cos(rad), M[0][1] = -sin(rad);
	M[1][0] = sin(rad), M[1][1] = cos(rad);
	InverseMatrix(M);
	TransposeMatrix(M);
	MultiplyMatrix();
}

void CNormalRotation::InverseMatrix(double M[3][3])//�����
{
	double deltA = M[0][0] * M[1][1] * M[2][2] + M[0][1] * M[1][2] * M[2][0] + M[0][2] * M[1][0] * M[2][1]
		- M[0][2] * M[1][1] * M[2][0] - M[0][1] * M[1][0] * M[2][2] - M[0][0] * M[1][2] * M[2][1];
	double Temp[3][3];//��ʱ����
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			Temp[j][i] = M[i][j];
	//ע��������Ԫ�صķ���λ��
	Temp[0][0] = M[1][1] * M[2][2] - M[1][2] * M[2][1];//��������ʽ
	Temp[1][0] = M[1][0] * M[2][2] - M[1][2] * M[2][0];
	Temp[2][0] = M[1][0] * M[2][1] - M[1][1] * M[2][0];
	Temp[0][1] = M[0][1] * M[2][2] - M[0][2] * M[2][1];
	Temp[1][1] = M[0][0] * M[2][2] - M[0][2] * M[2][0];
	Temp[2][1] = M[0][0] * M[2][1] - M[0][1] * M[2][0];
	Temp[0][2] = M[0][1] * M[1][2] - M[1][1] * M[0][2];
	Temp[1][2] = M[0][0] * M[1][2] - M[1][0] * M[0][2];
	Temp[2][2] = M[0][0] * M[1][1] - M[0][1] * M[1][0];
	for (i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M[i][j] = pow(-1.0, double(i + j + 2)) * (Temp[i][j] / deltA);
}

void CNormalRotation::TransposeMatrix(double M[3][3])//ת�þ���
{
	double PTemp[3][3];//��ʱ����
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			PTemp[j][i] = M[i][j];
	for (i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			M[i][j] = PTemp[i][j];
}

void CNormalRotation::MultiplyMatrix(void)//�������
{
	CVector3* NTemp = new CVector3[nNormalNumber];
	for (int i = 0; i < nNormalNumber; i++)
		NTemp[i] = N[i];
	for (i = 0; i < nNormalNumber; i++)
	{
		N[i].x = M[0][0] * NTemp[i].x + M[0][1] * NTemp[i].y + M[0][2] * NTemp[i].z;
		N[i].y = M[1][0] * NTemp[i].x + M[1][1] * NTemp[i].y + M[1][2] * NTemp[i].z;
		N[i].z = M[2][0] * NTemp[i].x + M[2][1] * NTemp[i].y + M[2][2] * NTemp[i].z;
	}
	delete[]NTemp;
}
