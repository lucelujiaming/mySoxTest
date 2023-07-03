#if !defined(AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P2.h"// ��ά��ε���

/************************************************************************/
/* �������ν�ı任�����������ǳ����ء�                                 */
/* ��ʵ���ǰ����ǵ�����任��ʽ��д���˾�����ʽ���ѡ�                   */
/* �������һ������ƽ��(Tx, Ty)�Ĺ�ʽΪ��                               */
/*     x' = x + Tx                                                      */
/*     y' = y + Ty                                                      */
/* ���ǿ��԰������ʽд�ɱ任����ĳ˷���ʽ��                           */
/*    | x' |   | 1 0 Tx |   | x |                                       */
/*    | y' | = | 0 1 Ty | * | y |                                       */
/*    | 1  |   | 0 0 1  |   | 1 |                                       */
/* ���������������ơ�                                                 */
/************************************************************************/
/* �任����M[3][3]�μ����ϵĶ�ά�任����Ĺ�ʽ(4-10)��                  */
/*     | a b e |                                                        */
/* M = | c d f |                                                        */
/*     | p q s |                                                        */
/* ����M0��ͼ�ν��б�������ת������ʹ��б任��                         */
/*        | a b |                                                       */
/*   M0 = | c d |                                                       */
/* ����M1��ͼ�ν���ƽ�Ʊ任��                                           */
/*        | e |                                                         */
/*   M1 = | f |                                                         */
/* ����M2��ͼ�ν���ͶӰ�任��M2 = | p q |                               */
/* ���ڶ�ά�任��p = 0, q = 0, M3 = | s |��ͼ�ν�����������任��       */
/************************************************************************/
class CTransform2
{
public:
	CTransform2(void);
	virtual ~CTransform2(void);
	void SetMatrix(CP2* pointVertex, int iPointNumber);    // ���ö�ά��������
	void Identity(void);                                   // ��λ����
	void Translate(double dTx, double dTy);                // ƽ�Ʊ任
	void Scale(double dSx, double dSy);                    // �����任
	void Scale(double sx, double sy, CP2 pointDirection);  // ����������ı����任
	void Rotate(double beta);                              // ��ת�任����
	void Rotate(double beta, CP2 pointDirection);          // �������������ת�任
	void ReflectOrg(void);                                 // ����ԭ�㷴��任
	void ReflectX(void);                                   // ����X�ᷴ��任
	void ReflectY(void);                                   // ����Y�ᷴ��任
	void Shear(double b, double c);                        // ���б任
	void MultiplyMatrix(void);                             // �������
private:
	double m_matrixTransform[3][3];   // �任����
	CP2*   m_pointVertex;             // ��������
	int    m_iPointNumber;            // �������
};

#endif // !defined(AFX_TRANSFORM2_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

