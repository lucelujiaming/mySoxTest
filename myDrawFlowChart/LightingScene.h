#if !defined(AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"LightSource.h"
#include"Material.h"
#include"Vector3.h"

// 这个类实现了简单光照模型。
class CLightingScene
{
public:
	CLightingScene(void);
	CLightingScene(int nLightNumber);
	virtual ~CLightingScene(void);
	void SetLightNumber(int nLightNumber);//设置光源数量
	CRGB Illuminate(CColorP3 ViewPoint, CColorP3 Point, CVector3 ptNormal, CMaterial* pMaterial);//计算光照	
public:
	int nLightNumber;//光源数量
	CLightSource* pLightSource;//光源数组
	CRGB Ambient;//环境光
	CMaterial* pMaterial;//材质
};

#endif // !defined(AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
