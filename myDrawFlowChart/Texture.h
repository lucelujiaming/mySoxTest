#if !defined(AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

// ������װ��һ��λͼ��ͨ������ReadBitmap��ȡλͼ��
// ����������Ա������
//   һ��������λͼ�ĸ�ʽ��Ϣ��
//   һ��������ʵ�ʵ�ͼ�����ݡ�
class CTexture
{
public:
	CTexture(void);
	virtual~CTexture(void);
	void ReadBitmap(UINT nIDResource);	// ����λͼ
	void DeleteObject(void);			// �ͷ�λͼ
public:
	BYTE* image;		// λͼ��ͼ�����ݡ�
	BITMAP bmp;			// BITMAP�ṹ�����������λͼ�ĸ�ʽ��Ϣ��
};

#endif // !defined(AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
