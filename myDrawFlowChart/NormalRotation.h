#if !defined(AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"Vector3.h"

class CNormalRotation//��ʸ����ת��
{
public:
	CNormalRotation(void);
	virtual ~CNormalRotation(void);
	void Identity(void);//��λ����
	void SetMatrix(CVector3* N, int nNumber);//��ʸ�������ʼ��
	void RotateX(double beta);//��X����ת�任
	void RotateY(double beta);//��Y����ת�任
	void RotateZ(double beta);//��Z����ת�任
	void InverseMatrix(double M[3][3]);//�����
	void TransposeMatrix(double M[3][3]);//ת�þ���
	void MultiplyMatrix(void);//�������
public:
	double M[3][3];//��ʸ���任����
	CVector3* N;//��ʸ��������
	int	nNormalNumber;//��ʸ������
};

#endif // !defined(AFX_NORMAL_ROTATION_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
