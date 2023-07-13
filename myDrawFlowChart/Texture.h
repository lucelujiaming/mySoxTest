#if !defined(AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

// 这个类封装了一个位图。通过调用ReadBitmap读取位图。
// 返回两个成员变量：
//   一个保存了位图的格式信息。
//   一个保存了实际的图像数据。
class CTexture
{
public:
	CTexture(void);
	virtual~CTexture(void);
	void ReadBitmap(UINT nIDResource);	// 读入位图
	void DeleteObject(void);			// 释放位图
public:
	BYTE* image;		// 位图的图像数据。
	BITMAP bmp;			// BITMAP结构体变量。包含位图的格式信息。
};

#endif // !defined(AFX_TEXTURE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
