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

class CTransform3
{
public:
	CTransform3(void);
	virtual ~CTransform3(void);
	void SetMatrix(CP3* P, int ptNumber);
	void Identity(void);
	void Translate(double tx, double ty, double tz);//ƽ�Ʊ任
	void Scale(double sx, double sy, double sz);//�����任
	void Scale(double sx, double sy, double sz, CP3 p);//����������ı����任
	void RotateX(double beta);//��X����ת�任
	void RotateX(double beta, CP3 p);//�������������X����ת�任
	void RotateY(double beta);//��Y����ת�任
	void RotateY(double beta, CP3 p);//�������������Y����ת�任
	void RotateZ(double beta);//��Z����ת�任����
	void RotateZ(double beta, CP3 p);//�������������Z����ת�任
	void ReflectX(void);//X�ᷴ��任
	void ReflectY(void);//Y�ᷴ��任
	void ReflectZ(void);//Z�ᷴ��任
	void ReflectXOY(void);//XOY�淴��任
	void ReflectYOZ(void);//YOZ�淴��任
	void ReflectZOX(void);//ZOX�淴��任
	void ShearX(double d, double g);//X������б任
	void ShearY(double b, double h);//Y������б任
	void ShearZ(double c, double f);//Z������б任
	void ArbitraryDirection(double beta, double nx, double ny, double nz);//��ԭ��������������ת
	void MultiplyMatrix(void);//�������
private:
	void LeftMultiplyMatrix(double M1[4][4], double M2[4][4]);//��˾���M2 = M1 * M2
private:
	double M[4][4];
	CP3* P;//��ά����������
	int ptNumber;//��ά�������
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)

