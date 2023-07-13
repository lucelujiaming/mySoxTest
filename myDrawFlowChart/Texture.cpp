#include "stdafx.h"
#include "Texture.h"

CTexture::CTexture(void)
{
	image = NULL;
}

CTexture::~CTexture(void)
{
}

void CTexture::ReadBitmap(UINT nIDResource)//读入位图
{
	CBitmap NewBitmap;//DDB
	NewBitmap.LoadBitmap(nIDResource);
	// 将CBitmap的信息保存到Bitmap结构体中
	NewBitmap.GetBitmap(&bmp);
	int nbytesize = bmp.bmWidthBytes * bmp.bmHeight;
	image = new BYTE[nbytesize];
	// 将图像数据保存到image中
	NewBitmap.GetBitmapBits(nbytesize, (LPVOID)image);
}

void CTexture::DeleteObject(void)//释放位图
{
	if(NULL != image)
		delete []image;
}