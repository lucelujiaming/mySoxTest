#if !defined(AFX_DEPTH_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_DEPTH_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"

class CDepthFace
{
public:
	CDepthFace(void);
	virtual ~CDepthFace(void);
	void SetPtNumber(int ptNumber);
public:
	int ptNumber; //面的顶点数
	int ptIndex[4];//面的顶点索引号
	double fMaxDepth;//面的深度
	CColorP3 Point[4];//顶点
};

#endif // !defined(AFX_DEPTH_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
