#include "stdafx.h"
#include "Transform2.h"
#include "math.h"

#define PI 3.1415926//Բ����

CTransform2::CTransform2(void)
{

}

CTransform2::~CTransform2(void)
{

}

void CTransform2::SetMatrix(CP2* pointVertex,int iPointNumber)
{
	this->m_pointVertex = pointVertex;
	this->m_iPointNumber = iPointNumber;
}

// ��λ����
void CTransform2::Identity(void) 
{
	m_matrixTransform[0][0] = 1.0, m_matrixTransform[0][1] = 0.0, m_matrixTransform[0][2] = 0.0;
	m_matrixTransform[1][0] = 0.0, m_matrixTransform[1][1] = 1.0, m_matrixTransform[1][2] = 0.0;
	m_matrixTransform[2][0] = 0.0, m_matrixTransform[2][1] = 0.0, m_matrixTransform[2][2] = 1.0;
}

/************************************************************************/
/* ��ά�任����ΪM������M1��ͼ�ν���ƽ�Ʊ任��                          */
/*     | a b e |             | e |                                      */
/* M = | c d f |        M1 = | f |                                      */
/*     | p q s |                                                        */
/* ����ϣ���άƽ�Ʊ任����Ϊ��                                         */
/*     | 1 0 Tx |                                                       */
/* M = | 0 1 Ty |                                                       */
/*     | 0 0  1 |                                                       */
/************************************************************************/
void CTransform2::Translate(double dTx, double dTy)//ƽ�Ʊ任����
{
	Identity();
	m_matrixTransform[0][2] = dTx; 
	m_matrixTransform[1][2] = dTy; 
	MultiplyMatrix();	
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν��б����任����                        */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ���ά�����任����Ϊ��                                         */
/*     | Sx  0 0 |                                                      */
/* M = |  0 Sy 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::Scale(double dSx, double dSy)//�����任����
{
	Identity();
	m_matrixTransform[0][0] = dSx; 
	m_matrixTransform[1][1] = dSy; 
	MultiplyMatrix();	
}

/************************************************************************/
/* ����������ı����任����ı任����Ϊ��                             */
/* ���ȰѾ����ƶ����õ㡣֮����б����任��֮�����ƶ���ȥ��             */
/************************************************************************/
void CTransform2::Scale(double sx,double sy, CP2 pointDirection) 
{
	Translate(-pointDirection.x, -pointDirection.y);
	Scale(sx, sy);
	Translate(pointDirection.x, pointDirection.y);	
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν�����ת�任��                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ���ά��ת�任����Ϊ��                                         */
/*     | cos(beta) -sin(beta) 0 |                                       */
/* M = | sin(beta)  cos(beta) 0 |                                       */
/*     |        0          0  1 |                                       */
/************************************************************************/
void CTransform2::Rotate(double beta) // ��ת�任����
{
	Identity();
	m_matrixTransform[0][0] = cos(beta * PI/180), m_matrixTransform[0][1] =-sin(beta * PI/180);
	m_matrixTransform[1][0] = sin(beta * PI/180), m_matrixTransform[1][1] = cos(beta * PI/180);
	MultiplyMatrix();	
}

/************************************************************************/
/* �������������ת�任����ı任����Ϊ��                             */
/* ���ȰѾ����ƶ����õ㡣֮�������ת�任��֮�����ƶ���ȥ��             */
/************************************************************************/
void CTransform2::Rotate(double beta, CP2 pointDirection)
{
	Translate(-pointDirection.x, -pointDirection.y);
	Rotate(beta);
	Translate(pointDirection.x, pointDirection.y);	
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν��з���任��                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ�ԭ�㷴��任����Ϊ��                                         */
/*     | -1  0 0 |                                                      */
/* M = |  0 -1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectOrg(void) // ԭ�㷴��任����
{
	Identity();
	m_matrixTransform[0][0] = -1;
	m_matrixTransform[1][1] = -1;
	MultiplyMatrix();
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν��з���任��                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ� X�ᷴ��任����Ϊ��                                         */
/*     |  1  0 0 |                                                      */
/* M = |  0 -1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectX(void) // X�ᷴ��任����
{
	Identity();
	m_matrixTransform[0][0] =  1;
	m_matrixTransform[1][1] = -1;
	MultiplyMatrix();	
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν��з���任��                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ� Y�ᷴ��任����Ϊ��                                         */
/*     | -1  0 0 |                                                      */
/* M = |  0  1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectY(void) // Y�ᷴ��任����
{
	Identity();
	m_matrixTransform[0][0]=-1;
	m_matrixTransform[1][1]=1;
	MultiplyMatrix();	
}

/************************************************************************/
/* ��ά�任����ΪM������M0��ͼ�ν��д��б任��                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* ����ϣ� ���б任����Ϊ��                                            */
/*     | 1 b 0 |                                                        */
/* M = | c 1 0 |                                                        */
/*     | 0 0 1 |                                                        */
/************************************************************************/
void CTransform2::Shear(double b,double c) // ���б任����
{
	Identity();
	m_matrixTransform[0][1]=b;
	m_matrixTransform[1][0]=c;
	MultiplyMatrix();	
}

void CTransform2::MultiplyMatrix(void) // �������
{
	CP2* pointReplica = new CP2[m_iPointNumber];
	for(int i = 0;i < m_iPointNumber;i++)
		pointReplica[i] = m_pointVertex[i];
	for( i = 0;i < m_iPointNumber;i++)
	{
		m_pointVertex[i].x = m_matrixTransform[0][0] * pointReplica[i].x 
						   + m_matrixTransform[0][1] * pointReplica[i].y 
						   + m_matrixTransform[0][2] * pointReplica[i].w;
		m_pointVertex[i].y = m_matrixTransform[1][0] * pointReplica[i].x 
						   + m_matrixTransform[1][1] * pointReplica[i].y 
						   + m_matrixTransform[1][2] * pointReplica[i].w;
		m_pointVertex[i].w = m_matrixTransform[2][0] * pointReplica[i].x 
						   + m_matrixTransform[2][1] * pointReplica[i].y 
						   + m_matrixTransform[2][2] * pointReplica[i].w;
	}
	delete []pointReplica;
}