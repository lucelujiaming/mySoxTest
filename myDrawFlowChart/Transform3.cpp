#include "StdAfx.h"
#include "Transform3.h"


CTransform3::CTransform3(void)
{
}


CTransform3::~CTransform3(void)
{
}

void CTransform3::SetMatrix(CP3*P,int ptNumber)
{
	this->P=P;
	this->ptNumber=ptNumber;
}
void CTransform3::Identity()
{
	T[0][0]=1.0;T[0][1]=0;T[0][2]=0;T[0][3]=0;
	T[1][0]=0;T[1][1]=1.0;T[1][2]=0;T[1][3]=0;
	T[2][0]=0;T[2][1]=0;T[2][2]=1.0;T[2][3]=0;
	T[3][0]=0;T[3][1]=0;T[3][2]=0;T[3][3]=1.0;
}

void CTransform3::Translate(double tx,double ty,double tz)
{
	Identity();
	T[0][3]=tx;
	T[1][3]=ty;
	T[2][3]=tz;
	MultiplyMatrix();
}

void CTransform3::RotateX(double beta)
{
	Identity();
	double rad=beta*PI/180;
	T[1][1]=cos(rad);T[1][2]=-sin(rad);
	T[2][1]=sin(rad);T[1][2]=cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateY(double beta)
{
	Identity();
	double rad=beta*PI/180;
	T[0][0]=cos(rad);T[0][2]=sin(rad);
	T[2][0]=-sin(rad);T[2][2]=cos(rad);
	MultiplyMatrix();
}

void CTransform3::RotateZ(double beta)
{
	Identity();
	double rad=beta*PI/180;
	T[0][0]=cos(rad);T[0][1]=-sin(rad);
	T[1][0]=sin(rad);T[1][1]=cos(rad);
	MultiplyMatrix();
}

void CTransform3::MultiplyMatrix()
{
	CP3*PTemp=new CP3[ptNumber];
	for(int i=0;i<ptNumber;i++)
		PTemp[i]=P[i];
	for(int j=0;j<ptNumber;j++)
	{
		P[j].x=T[0][0]*PTemp[j].x+T[0][1]*PTemp[j].y+T[0][2]*PTemp[j].z+T[0][3]*PTemp[j].w;
		P[j].y=T[1][0]*PTemp[j].x+T[1][1]*PTemp[j].y+T[1][2]*PTemp[j].z+T[1][3]*PTemp[j].w;
		P[j].z=T[2][0]*PTemp[j].x+T[2][1]*PTemp[j].y+T[2][2]*PTemp[j].z+T[2][3]*PTemp[j].w;
		P[j].w=T[3][0]*PTemp[j].x+T[3][1]*PTemp[j].y+T[3][2]*PTemp[j].z+T[3][3]*PTemp[j].w;
	}
	delete [] PTemp;
}
