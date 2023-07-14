#if !defined(AFX_MATERIAL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_MATERIAL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"RGB.h"

// ������
// ����Ĳ���������ָ���������ڹ�����գ������Ͷ������ܡ�
// ���ڼ򵥹�����˵������ֻ���ǲ��ʵķ������ԡ�
// ���������ɻ�����ķ����ʣ��������ķ����ʣ����淴���ķ����ʵȷ�����ɡ�
class CMaterial
{
public:
	CMaterial(void);
	virtual~CMaterial(void);
	void SetAmbient(CRGB c);     // ���û�����ķ����ʡ�ע�������趨��һ����ɫֵ��	
	void SetDiffuse(CRGB c);     // �����������ķ����ʡ�ע�������趨��һ����ɫֵ��	
	void SetSpecular(CRGB c);    // ���þ��淴���ķ����ʡ�ע�������趨��һ����ɫֵ��	
	void SetEmission(CRGB c);    // ��������������ɫ��ע�������趨��һ����ɫֵ��	
	void SetExponent(double n);  // ���ø߹�ָ����ע�������趨��һ����ɫֵ��	
public:
	CRGB M_Ambient;              // ������ķ����ʡ�ע��������һ����ɫֵ��	
	CRGB M_Diffuse;              // �������ķ����ʡ�ע��������һ����ɫֵ��	
	CRGB M_Specular;             // ���淴���ķ����ʡ�ע��������һ����ɫֵ��	
	CRGB M_Emission;             // ����������ɫ��ע��������һ����ɫֵ��	
	double M_n;                  // �߹�ָ��
};

#endif // !defined(AFX_MATERIAL_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
