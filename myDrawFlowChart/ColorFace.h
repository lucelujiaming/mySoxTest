#if !defined(AFX_COLOR_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLOR_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RGB.h"

class CColorFace
{
public:
	CColorFace(void);
	virtual ~CColorFace(void);
	void SetPtNumber(int ptNumber);
public:
	int ptNumber;		// ��Ķ�����
	int ptIndex[4];		// ��Ķ���������
	CRGB c;				// �����ɫ
// ����Obj�ļ�
public:	
	int Index[3];	//��Ķ���������
};

#endif // !defined(AFX_COLOR_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
