#if !defined(AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdafx.h"

/************************************************************************/
/* �����װ��ƽ����ɫ������������㷨                                   */
/* ��Ϊ���۶�ô���ӵ����壬���ն�����ʹ��������С��ƽ���               */
/* �������������ε����⣬�ͽ�������������ɫ�����⡣                 */
/* ����������һ��͹����Ρ�ɨ�������������ֻཻ����һ�Խ��㡣           */
/* ���γ�һ���ཻ���䡣��Ϊ���Span��                                   */
/************************************************************************/
class CFlatFillTriangle // �����������
{
public:
	CFlatFillTriangle(void);
	CFlatFillTriangle(CPoint P0, CPoint P1, CPoint P2);//�����㹹��������
	virtual ~CFlatFillTriangle(void);
	void Fill(CDC* pDC);//���������
private:
	void EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
private:
	CPoint   m_P[3];       // �����ε���������
	CPoint*  m_SpanLeft;   // ��ȵ�����־����
	CPoint*  m_SpanRight;  // ��ȵ��յ��־����
	int      m_nIndex;     // ɨ��������
};

#endif // !defined(AFX_FLATFILLTRIANGLE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
