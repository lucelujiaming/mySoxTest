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
	int ptNumber; //��Ķ�����
	int ptIndex[4];//��Ķ���������
	double fMaxDepth;//������
	CColorP3 Point[4];//����
};

#endif // !defined(AFX_DEPTH_FACE_H_6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
