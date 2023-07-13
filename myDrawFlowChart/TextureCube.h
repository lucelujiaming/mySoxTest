#if !defined(AFX_TEXTURE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_TEXTURE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TextureFace.h"
#include "Projection.h"
#include "Vector3.h"
#include "LightingScene.h"
#include "Texture.h"
#include "TextureZBuffer.h"

// 使用图像纹理算法算法绘制的立方体
class CTextureCube
{
public:
	CTextureCube(void);
	virtual ~CTextureCube(void);
	void ReadVertex(void);								// 读入点表
	void ReadFace(void);								// 读入面表
	void SetLightingScene(CLightingScene* pScene);		// 设置光照场景
	void SetTexture(CTexture* pTexture);				// 设置纹理
	void Draw(CDC* pDC, CTextureZBuffer* pZBuffer);		// 绘制立方体
public:
	CColorP3 V[8];							// 顶点数组
	CTextureFace F[6];						// 表面数组
	CProjection projection;					// 投影对象
	CLightingScene* pScene;					// 光照场景
	CTexture* pTexture;						// 纹理
};

#endif // !defined(AFX_TEXTURE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
