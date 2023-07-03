#if !defined(AFX_COLOR_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"ColorP3.h"
#include"ColorFace.h"

class CColorCube
{
public:
	CColorCube(void);
	virtual ~CColorCube(void);
	void ReadVertex(int xScale = 1, 
		            int yScale = 1,      // 读入点表。
					int zScale = 1);     // 默认为长宽高都为1的单位立方体。
	void ReadFace(void);                 // 读入面表
public:
	CColorP3 V[8];    // 顶点数组
	CColorFace F[6];  // 表面数组
public:
	int m_xScale, m_yScale, m_zScale;     // 长宽高。
};

#endif // !defined(AFX_COLOR_CUBE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

