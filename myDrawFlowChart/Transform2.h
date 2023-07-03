#if !defined(AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P2.h"// 二维齐次点类

/************************************************************************/
/* 这里的所谓的变换矩阵听起来非常神秘。                                 */
/* 其实就是把我们的坐标变换公式改写成了矩阵形式而已。                   */
/* 例如对于一个坐标平移(Tx, Ty)的公式为：                               */
/*     x' = x + Tx                                                      */
/*     y' = y + Ty                                                      */
/* 我们可以把这个公式写成变换矩阵的乘法形式：                           */
/*    | x' |   | 1 0 Tx |   | x |                                       */
/*    | y' | = | 0 1 Ty | * | y |                                       */
/*    | 1  |   | 0 0 1  |   | 1 |                                       */
/* 其他的情况与此类似。                                                 */
/************************************************************************/
/* 变换矩阵M[3][3]参见书上的二维变换矩阵的公式(4-10)。                  */
/*     | a b e |                                                        */
/* M = | c d f |                                                        */
/*     | p q s |                                                        */
/* 其中M0对图形进行比例，旋转，反射和错切变换。                         */
/*        | a b |                                                       */
/*   M0 = | c d |                                                       */
/* 其中M1对图形进行平移变换。                                           */
/*        | e |                                                         */
/*   M1 = | f |                                                         */
/* 其中M2对图形进行投影变换。M2 = | p q |                               */
/* 对于二维变换，p = 0, q = 0, M3 = | s |对图形进行整体比例变换。       */
/************************************************************************/
class CTransform2
{
public:
	CTransform2(void);
	virtual ~CTransform2(void);
	void SetMatrix(CP2* pointVertex, int iPointNumber);    // 设置二维顶点数组
	void Identity(void);                                   // 单位矩阵
	void Translate(double dTx, double dTy);                // 平移变换
	void Scale(double dSx, double dSy);                    // 比例变换
	void Scale(double sx, double sy, CP2 pointDirection);  // 相对于任意点的比例变换
	void Rotate(double beta);                              // 旋转变换矩阵
	void Rotate(double beta, CP2 pointDirection);          // 相对于任意点的旋转变换
	void ReflectOrg(void);                                 // 关于原点反射变换
	void ReflectX(void);                                   // 关于X轴反射变换
	void ReflectY(void);                                   // 关于Y轴反射变换
	void Shear(double b, double c);                        // 错切变换
	void MultiplyMatrix(void);                             // 矩阵相乘
private:
	double m_matrixTransform[3][3];   // 变换矩阵
	CP2*   m_pointVertex;             // 顶点数组
	int    m_iPointNumber;            // 顶点个数
};

#endif // !defined(AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

