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
	void SetDiffuse(CRGB diffuse);                              // 设置光源的漫反射光
	void SetSpecular(CRGB specular);                            // 设置光源的镜面反射光
	void SetPosition(double x, double y, double z);             // 设置光源的位置
	void SetAttenuationFactor(double c0, double c1, double c2); // 设置光强的衰减因子
	void SetOnOff(BOOL onoff);                                  //设置光源开关状态	
public:
	CRGB L_Diffuse;              //漫反射光颜色	
	CRGB L_Specular;             //镜面反射光颜色
	CColorP3 L_Position;         //光源位置
	double L_C0;                 //常数衰减因子
	double L_C1;                 //线性衰减因子
	double L_C2;                 //二次衰减因子
	BOOL L_OnOff;                //光源开启或关闭
};

#endif // !defined(AFX_LIGHT_SOURCE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
