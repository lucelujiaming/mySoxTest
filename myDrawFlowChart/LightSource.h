#if !defined(AFX_LIGHT_SOURCE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_LIGHT_SOURCE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"ColorP3.h"

class CLightSource
{
public:
	CLightSource(void);
	virtual ~CLightSource(void);
	void SetDiffuse(CRGB diffuse);//���ù�Դ���������
	void SetSpecular(CRGB specular);//���ù�Դ�ľ��淴���
	void SetPosition(double x, double y, double z);//���ù�Դ��λ��
	void SetAttenuationFactor(double c0, double c1, double c2);//���ù�ǿ��˥������
	void SetOnOff(BOOL onoff);//���ù�Դ����״̬	
public:
	CRGB L_Diffuse;//���������ɫ	
	CRGB L_Specular;//���淴�����ɫ
	CColorP3 L_Position;//��Դλ��
	double L_C0;//����˥������
	double L_C1;//����˥������
	double L_C2;//����˥������
	BOOL L_OnOff;//��Դ������ر�
};

#endif // !defined(AFX_LIGHT_SOURCE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
