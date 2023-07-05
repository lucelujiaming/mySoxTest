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
	void ReadNumber(void);//读入元素个数
	void ReadVertex(void);//读入顶点
	void ReadTexture(void);//读入纹理
	void ReadNormal(void);//读入法矢量
	void ReadFace(void);//读入表面
	CColorP3* GetVertexName(void);//得到顶点数组名
	int GetVertexNumber(void);//得到顶点个数
	CVector3* GetNormalName(void);//得到法矢量数组名
	int GetNormalNumber(void);//得到法矢量个数
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void Draw(CDC* pDC, CDepthLinearInterpZBuffer* pZBuffer);//绘制茶壶
private:
	CString FileName;
	CColorP3* V;//点表
	CVector3* N;//法矢量
	CTextureCoordinate* T;//纹理
	CColorFace* F;//面表
	int nTotalVertex;//顶点个数
	int nTotalNormal;//法矢量个数
	int nTotalTexture;//纹理个数
	int nTotalFace;//表面个数
	CProjection projection;//投影
	CLightingScene* pScene;//光照
};

#endif // !defined(AFX_OBJFILE_TEAPOT_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
