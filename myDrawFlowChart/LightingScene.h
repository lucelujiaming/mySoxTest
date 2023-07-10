#if !defined(AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"LightSource.h"
#include"Material.h"
#include"Vector3.h"

// �����ʵ���˼򵥹���ģ�͡�
class CLightingScene
{
public:
	CLightingScene(void);
	CLightingScene(int nLightNumber);
	virtual ~CLightingScene(void);
	void SetLightNumber(int nLightNumber);//���ù�Դ����
	CRGB Illuminate(CColorP3 ViewPoint, CColorP3 Point, CVector3 ptNormal, CMaterial* pMaterial);//�������	
public:
	int nLightNumber;//��Դ����
	CLightSource* pLightSource;//��Դ����
	CRGB Ambient;//������
	CMaterial* pMaterial;//����
};

#endif // !defined(AFX_LIGHTING_SCENE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
