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
		            int yScale = 1,      // 读入点表。
					int zScale = 1);     // 默认为长宽高都为1的单位立方体。
	void ReadFace(void);                 // 读入面表
	void Draw(CDC* pDC, 
		BOOL isDrawMainDiagonal = TRUE); // 绘制正八面体
public:
	CP3 V[8];    // 顶点数组
	CFace F[6];  // 表面数组
public:
	int m_xScale, m_yScale, m_zScale;     // 长宽高。
};

#endif // !defined(AFX_GEOMETRYCUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

