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


/************************************************************************/
/* 这里的所谓的变换矩阵听起来非常神秘。                                 */
/* 其实就是把我们的坐标变换公式改写成了矩阵形式而已。                   */
/* 例如对于一个坐标平移(Tx, Ty, Tz)的公式为：                           */
/*     x' = x + Tx                                                      */
/*     y' = y + Ty                                                      */
/*     z' = z + Tz                                                      */
/* 我们可以把这个公式写成变换矩阵的乘法形式：                           */
/*    | x' |   | 1 0 0 Tx |   | x |                                     */
/*    | y' | = | 0 1 0 Ty | * | y |                                     */
/*    | z' |   | 0 0 1 Tz |   | z |                                     */
/*    | 1  |   | 0 0 0 1  |   | 1 |                                     */
/* 其他的情况与此类似。                                                 */
/************************************************************************/
/* 这里的变换矩阵M[4][4]就是书上的三维变换矩阵。为公式(5-1)。           */
/*     | a b e l |                                                      */
/* M = | c d f m |                                                      */
/*     | g h i n |                                                      */
/*     | p q r s |                                                      */
/* 其中M0对图形进行比例，旋转，反射和错切变换。                         */
/*        | a b e |                                                     */
/*   M0 = | c d f |                                                     */
/*        | g h i |                                                     */
/* 其中M1对图形进行平移变换。                                           */
/*        | l |                                                         */
/*   M1 = | m |                                                         */
/*        | n |                                                         */
/* 其中M2对图形进行投影变换。M2 = | p q r |                             */
/* 对于三维变换，p = 0, q = 0, r = 0, M3 = | s |                        */
/* 对图形进行整体比例变换。                                             */
/************************************************************************/
class CTransform3
{
public:
	CTransform3(void);
	virtual ~CTransform3(void);
	void SetMatrix(CP3* P, int ptNumber);                  // 读入顶点数组        
	void Identity(void);                                   // 生成单位矩阵
	void Translate(double tx, double ty, double tz);       // 平移变换
	void Scale(double sx, double sy, double sz);           // 比例变换
	void Scale(double sx, double sy, double sz, CP3 p);    // 相对于任意点的比例变换
	void RotateX(double beta);                             // 绕X轴旋转变换
	void RotateX(double beta, CP3 p);                      // 相对于任意点的绕X轴旋转变换
	void RotateY(double beta);                             // 绕Y轴旋转变换
	void RotateY(double beta, CP3 p);                      // 相对于任意点的绕Y轴旋转变换
	void RotateZ(double beta);                             // 绕Z轴旋转变换矩阵
	void RotateZ(double beta, CP3 p);                      // 相对于任意点的绕Z轴旋转变换
	void ReflectX(void);                                   // X轴反射变换
	void ReflectY(void);                                   // Y轴反射变换
	void ReflectZ(void);                                   // Z轴反射变换
	void ReflectXOY(void);                                 // XOY面反射变换
	void ReflectYOZ(void);                                 // YOZ面反射变换
	void ReflectZOX(void);                                 // ZOX面反射变换
	void ShearX(double d, double g);                       // X方向错切变换
	void ShearY(double b, double h);                       // Y方向错切变换
	void ShearZ(double c, double f);                       // Z方向错切变换
	void ArbitraryDirection(double beta, double nx,        
						    double ny, double nz);         // 过原点的绕任意轴的旋转
	void MultiplyMatrix(void);                             // 矩阵相乘
private:
	void LeftMultiplyMatrix(double M1[4][4], 
	                        double M2[4][4]);              // 左乘矩阵M2 = M1 * M2
private:
	double M[4][4];    // 三维变换矩阵。
	CP3* P;            // 三维顶点数组名
	int ptNumber;      // 三维顶点个数
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)

