#if !defined(AFX_SMOOTHFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_SMOOTHFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorPoint.h"//����ɫ�Ķ�ά��������

class CSmoothFillTriangle//�����������
{
public:
	CSmoothFillTriangle(void);
	CSmoothFillTriangle(CColorPoint P0, CColorPoint P1, CColorPoint P2);//�����㹹��������
	virtual ~CSmoothFillTriangle(void);
	CRGB Interp(double m, double m0, double m1, CRGB c0, CRGB c1); // ��ɫ���Բ�ֵ
	void Fill(CDC* pDC);//���������
private:
	void EdgeFlag(CColorPoint PStart, CColorPoint PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
private:
	CColorPoint  m_P[3];         // �����ε���������
	CColorPoint* m_SpanLeft;     // ��ȵ�����־����
	CColorPoint* m_SpanRight;    // ��ȵ��յ��־����
	int          m_nIndex;       // ɨ��������
};

#endif // !defined(AFX_SMOOTHFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
