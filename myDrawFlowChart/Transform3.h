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
	~CTransform3(void);
	void SetMatrix(CP3*P,int ptNumber);
	void Identity();//��ʼ�任��λ����
	void Translate(double tx,double ty,double tz);//ƽ�Ʊ任����
	void RotateX(double beta);//��x����ת�任����
	void RotateY(double beta);//��x����ת�任����
	void RotateZ(double beta);//��x����ת�任����
	void MultiplyMatrix();//�任������˶�������
public:
	double T[4][4];//��ά�任����
	CP3*P;//��̬���鶨�嶥������
	int ptNumber;//�������
};

#endif // !defined(AFX_CP3_H__83A9A336_3155_4360_ACD5_39ABFFB9442A__INCLUDED_)

