#if !defined(AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"

class CBicubicBezierPatch
{
public:
	CBicubicBezierPatch(void);
	virtual ~CBicubicBezierPatch(void);
	void ReadControlPoint(CP3 P[4][4]);//����16�����Ƶ�
	void DrawCurvedPatch(CDC* pDC);//����˫����Bezier����Ƭ
	void DrawControlGrid(CDC* pDC);//���ƿ�������
private:
	void LeftMultiplyMatrix(double M[4][4],CP3 P[4][4]);//��˶������
	void RightMultiplyMatrix(CP3 P[4][4],double M[4][4]);//�ҳ˶������
	void TransposeMatrix(double M[4][4]);//ת�þ���
public:
	CP3 P[4][4];//��ά���Ƶ�
	CProjection   projection; // ͶӰ����
	
};

#endif // !defined(AFX_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
