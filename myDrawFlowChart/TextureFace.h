#if !defined(AFX_TEXTURE_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RGB.h"
#include "TextureCoordinate.h"

class CTextureFace
{
public:
	CTextureFace(void);
	virtual ~CTextureFace(void);
	void SetPtNumber(int ptNumber);
public:
	int ptNumber; //��Ķ�����
	int ptIndex[4];//��Ķ���������
	CTextureCoordinate t[4];//�����ַ
};

#endif // !defined(AFX_TEXTURE_FACE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
