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

void CTransform2::SetMatrix(CP2* pointVertex,int iPointNumber)
{
	this->m_pointVertex = pointVertex;
	this->m_iPointNumber = iPointNumber;
}

// 单位矩阵
void CTransform2::Identity(void) 
{
	m_matrixTransform[0][0] = 1.0, m_matrixTransform[0][1] = 0.0, m_matrixTransform[0][2] = 0.0;
	m_matrixTransform[1][0] = 0.0, m_matrixTransform[1][1] = 1.0, m_matrixTransform[1][2] = 0.0;
	m_matrixTransform[2][0] = 0.0, m_matrixTransform[2][1] = 0.0, m_matrixTransform[2][2] = 1.0;
}

/************************************************************************/
/* 二维变换矩阵为M。其中M1对图形进行平移变换。                          */
/*     | a b e |             | e |                                      */
/* M = | c d f |        M1 = | f |                                      */
/*     | p q s |                                                        */
/* 因此上，二维平移变换矩阵为：                                         */
/*     | 1 0 Tx |                                                       */
/* M = | 0 1 Ty |                                                       */
/*     | 0 0  1 |                                                       */
/************************************************************************/
void CTransform2::Translate(double dTx, double dTy)//平移变换矩阵
{
	Identity();
	m_matrixTransform[0][2] = dTx; 
	m_matrixTransform[1][2] = dTy; 
	MultiplyMatrix();	
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行比例变换。。                        */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上，二维比例变换矩阵为：                                         */
/*     | Sx  0 0 |                                                      */
/* M = |  0 Sy 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::Scale(double dSx, double dSy)//比例变换矩阵
{
	Identity();
	m_matrixTransform[0][0] = dSx; 
	m_matrixTransform[1][1] = dSy; 
	MultiplyMatrix();	
}

/************************************************************************/
/* 相对于任意点的比例变换矩阵的变换方法为：                             */
/* 首先把矩阵移动到该点。之后进行比例变换。之后再移动回去。             */
/************************************************************************/
void CTransform2::Scale(double sx,double sy, CP2 pointDirection) 
{
	Translate(-pointDirection.x, -pointDirection.y);
	Scale(sx, sy);
	Translate(pointDirection.x, pointDirection.y);	
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行旋转变换。                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上，二维旋转变换矩阵为：                                         */
/*     | cos(beta) -sin(beta) 0 |                                       */
/* M = | sin(beta)  cos(beta) 0 |                                       */
/*     |        0          0  1 |                                       */
/************************************************************************/
void CTransform2::Rotate(double beta) // 旋转变换矩阵
{
	Identity();
	m_matrixTransform[0][0] = cos(beta * PI/180), m_matrixTransform[0][1] =-sin(beta * PI/180);
	m_matrixTransform[1][0] = sin(beta * PI/180), m_matrixTransform[1][1] = cos(beta * PI/180);
	MultiplyMatrix();	
}

/************************************************************************/
/* 相对于任意点的旋转变换矩阵的变换方法为：                             */
/* 首先把矩阵移动到该点。之后进行旋转变换。之后再移动回去。             */
/************************************************************************/
void CTransform2::Rotate(double beta, CP2 pointDirection)
{
	Translate(-pointDirection.x, -pointDirection.y);
	Rotate(beta);
	Translate(pointDirection.x, pointDirection.y);	
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行反射变换。                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上，原点反射变换矩阵为：                                         */
/*     | -1  0 0 |                                                      */
/* M = |  0 -1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectOrg(void) // 原点反射变换矩阵
{
	Identity();
	m_matrixTransform[0][0] = -1;
	m_matrixTransform[1][1] = -1;
	MultiplyMatrix();
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行反射变换。                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上， X轴反射变换矩阵为：                                         */
/*     |  1  0 0 |                                                      */
/* M = |  0 -1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectX(void) // X轴反射变换矩阵
{
	Identity();
	m_matrixTransform[0][0] =  1;
	m_matrixTransform[1][1] = -1;
	MultiplyMatrix();	
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行反射变换。                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上， Y轴反射变换矩阵为：                                         */
/*     | -1  0 0 |                                                      */
/* M = |  0  1 0 |                                                      */
/*     |  0  0 1 |                                                      */
/************************************************************************/
void CTransform2::ReflectY(void) // Y轴反射变换矩阵
{
	Identity();
	m_matrixTransform[0][0]=-1;
	m_matrixTransform[1][1]=1;
	MultiplyMatrix();	
}

/************************************************************************/
/* 二维变换矩阵为M。其中M0对图形进行错切变换。                          */
/*     | a b e |             | a b |                                    */
/* M = | c d f |        M0 = | c d |                                    */
/*     | p q s |                                                        */
/* 因此上， 错切变换矩阵为：                                            */
/*     | 1 b 0 |                                                        */
/* M = | c 1 0 |                                                        */
/*     | 0 0 1 |                                                        */
/************************************************************************/
void CTransform2::Shear(double b,double c) // 错切变换矩阵
{
	Identity();
	m_matrixTransform[0][1]=b;
	m_matrixTransform[1][0]=c;
	MultiplyMatrix();	
}

void CTransform2::MultiplyMatrix(void) // 矩阵相乘
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