#pragma once

#include "stdafx.h"

class CTriangle//�����������
{
public:
	CTriangle(void);
	CTriangle(CPoint P0, CPoint P1, CPoint P2);//�����㹹��������
	virtual ~CTriangle(void);
	void Fill(CDC* pDC);//���������
private:
	void EdgeFlag(CPoint PStart, CPoint PEnd, BOOL bFeature);//�߱���㷨
	void SortPoint(void);//��������
private:
	CPoint P[3];//����
	CPoint* SpanLeft; //��ȵ�����־����
	CPoint* SpanRight;//��ȵ��յ��־����
	int nIndex;//ɨ��������
};