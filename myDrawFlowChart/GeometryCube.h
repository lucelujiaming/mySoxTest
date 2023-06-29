#if !defined(AFX_GEOMETRYCUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GEOMETRYCUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P3.h"
#include"Face.h"

class CGeometryCube
{
public:
	CGeometryCube(void);
	virtual ~CGeometryCube(void);
	void ReadVertex(int xScale = 1, 
		            int yScale = 1,      // ������
					int zScale = 1);     // Ĭ��Ϊ����߶�Ϊ1�ĵ�λ�����塣
	void ReadFace(void);                 // �������
	void Draw(CDC* pDC, 
		BOOL isDrawMainDiagonal = TRUE); // ������������
public:
	CP3 V[8];    // ��������
	CFace F[6];  // ��������
public:
	int m_xScale, m_yScale, m_zScale;     // ����ߡ�
};

#endif // !defined(AFX_GEOMETRYCUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

