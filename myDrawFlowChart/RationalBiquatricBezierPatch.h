#if !defined(AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P3.h"
#include"Projection.h"

class CRationalBiquatricBezierPatch//����˫����Bezier����Ƭ��
{
public:
	CRationalBiquatricBezierPatch(void);
	virtual ~CRationalBiquatricBezierPatch(void);
	void ReadControlPoint(CP3 P[3][3]);//����9�����Ƶ�
	void ReadWeight(double W[3][3]);//����9��Ȩ����
	void DrawCurvedPatch(CDC* pDC);//����˫����Bezier����Ƭ
	void DrawControlGrid(CDC *pDC);//���ƿ�������
private:
	void LeftMultiplyMatrix(double M[3][3],CP3 P[3][3]);//��˶������
	void RightMultiplyMatrix(CP3 P[3][3],double M[3][3]);//�ҳ˶������
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//���Ȩ���Ӿ���
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//�ҳ�Ȩ���Ӿ���
	void TransposeMatrix(double M[3][3]);//ת�þ���
public:
	CP3 P[3][3];//��ά���Ƶ�
	double W[3][3];//Ȩ����
	CProjection projection;//ͶӰ
};

#endif // !defined(AFX_RATIONAL_QUADRATIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

