#if !defined(AFX_OBJFILE_TEAPOT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_OBJFILE_TEAPOT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorFace.h"
#include "Projection.h"
#include "Vector3.h"
#include "TextureCoordinate.h"
#include "DepthLinearInterpZBuffer.h"
#include "LightingScene.h"

class CObjFileTeapot
{
public:
	CObjFileTeapot(void);
	virtual ~CObjFileTeapot(void);
	void ReadNumber(void);//����Ԫ�ظ���
	void ReadVertex(void);//���붥��
	void ReadTexture(void);//��������
	void ReadNormal(void);//���뷨ʸ��
	void ReadFace(void);//�������
	CColorP3* GetVertexName(void);//�õ�����������
	int GetVertexNumber(void);//�õ��������
	CVector3* GetNormalName(void);//�õ���ʸ��������
	int GetNormalNumber(void);//�õ���ʸ������
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void Draw(CDC* pDC, CDepthLinearInterpZBuffer* pZBuffer);//���Ʋ��
private:
	CString FileName;
	CColorP3* V;//���
	CVector3* N;//��ʸ��
	CTextureCoordinate* T;//����
	CColorFace* F;//���
	int nTotalVertex;//�������
	int nTotalNormal;//��ʸ������
	int nTotalTexture;//�������
	int nTotalFace;//�������
	CProjection projection;//ͶӰ
	CLightingScene* pScene;//����
};

#endif // !defined(AFX_OBJFILE_TEAPOT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
