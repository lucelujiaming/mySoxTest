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
/* �������ν�ı任�����������ǳ����ء�                                 */
/* ��ʵ���ǰ����ǵ�����任��ʽ��д���˾�����ʽ���ѡ�                   */
/* �������һ������ƽ��(Tx, Ty, Tz)�Ĺ�ʽΪ��                           */
/*     x' = x + Tx                                                      */
/*     y' = y + Ty                                                      */
/*     z' = z + Tz                                                      */
/* ���ǿ��԰������ʽд�ɱ任����ĳ˷���ʽ��                           */
/*    | x' |   | 1 0 0 Tx |   | x |                                     */
/*    | y' | = | 0 1 0 Ty | * | y |                                     */
/*    | z' |   | 0 0 1 Tz |   | z |                                     */
/*    | 1  |   | 0 0 0 1  |   | 1 |                                     */
/* ���������������ơ�                                                 */
/************************************************************************/
/* ����ı任����M[4][4]�������ϵ���ά�任����Ϊ��ʽ(5-1)��           */
/*     | a b e l |                                                      */
/* M = | c d f m |                                                      */
/*     | g h i n |                                                      */
/*     | p q r s |                                                      */
/* ����M0��ͼ�ν��б�������ת������ʹ��б任��                         */
/*        | a b e |                                                     */
/*   M0 = | c d f |                                                     */
/*        | g h i |                                                     */
/* ����M1��ͼ�ν���ƽ�Ʊ任��                                           */
/*        | l |                                                         */
/*   M1 = | m |                                                         */
/*        | n |                                                         */
/* ����M2��ͼ�ν���ͶӰ�任��M2 = | p q r |                             */
/* ������ά�任��p = 0, q = 0, r = 0, M3 = | s |                        */
/* ��ͼ�ν�����������任��                                             */
/************************************************************************/
class CTransform3
{
public:
	CTransform3(void);
	virtual ~CTransform3(void);
	void SetMatrix(CP3* P, int ptNumber);                  // ���붥������        
	void Identity(void);                                   // ���ɵ�λ����
	void Translate(double tx, double ty, double tz);       // ƽ�Ʊ任
	void Scale(double sx, double sy, double sz);           // �����任
	void Scale(double sx, double sy, double sz, CP3 p);    // ����������ı����任
	void RotateX(double beta);                             // ��X����ת�任
	void RotateX(double beta, CP3 p);                      // �������������X����ת�任
	void RotateY(double beta);                             // ��Y����ת�任
	void RotateY(double beta, CP3 p);                      // �������������Y����ת�任
	void RotateZ(double beta);                             // ��Z����ת�任����
	void RotateZ(double beta, CP3 p);                      // �������������Z����ת�任
	void ReflectX(void);                                   // X�ᷴ��任
	void ReflectY(void);                                   // Y�ᷴ��任
	void ReflectZ(void);                                   // Z�ᷴ��任
	void ReflectXOY(void);                                 // XOY�淴��任
	void ReflectYOZ(void);                                 // YOZ�淴��任
	void ReflectZOX(void);                                 // ZOX�淴��任
	void ShearX(double d, double g);                       // X������б任
	void ShearY(double b, double h);                       // Y������б任
	void ShearZ(double c, double f);                       // Z������б任
	void ArbitraryDirection(double beta, double nx,        
						    double ny, double nz);         // ��ԭ��������������ת
	void MultiplyMatrix(void);                             // �������
private:
	void LeftMultiplyMatrix(double M1[4][4], 
	                        double M2[4][4]);              // ��˾���M2 = M1 * M2
private:
	double M[4][4];    // ��ά�任����
	CP3* P;            // ��ά����������
	int ptNumber;      // ��ά�������
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)

