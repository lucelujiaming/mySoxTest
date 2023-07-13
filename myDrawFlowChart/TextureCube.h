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

// ʹ��ͼ�������㷨�㷨���Ƶ�������
class CTextureCube
{
public:
	CTextureCube(void);
	virtual ~CTextureCube(void);
	void ReadVertex(void);								// ������
	void ReadFace(void);								// �������
	void SetLightingScene(CLightingScene* pScene);		// ���ù��ճ���
	void SetTexture(CTexture* pTexture);				// ��������
	void Draw(CDC* pDC, CTextureZBuffer* pZBuffer);		// ����������
public:
	CColorP3 V[8];							// ��������
	CTextureFace F[6];						// ��������
	CProjection projection;					// ͶӰ����
	CLightingScene* pScene;					// ���ճ���
	CTexture* pTexture;						// ����
};

#endif // !defined(AFX_TEXTURE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
