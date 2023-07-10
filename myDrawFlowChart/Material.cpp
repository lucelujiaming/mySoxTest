#include "stdafx.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	M_Ambient = CRGB(0.2, 0.2, 0.2);  // ���ʵĻ��������ʡ�Ϊ��ɫ��
	M_Diffuse = CRGB(0.8, 0.8, 0.8);  // ���ʵ��������ʡ�Ϊ��ɫ��
	M_Specular = CRGB(0.0, 0.0, 0.0); // ���ʵľ��淴���ʡ�Ϊ��ɫ��
	M_Emission = CRGB(0.0, 0.0, 0.0); // ���ʵķ���⡣Ϊ��ɫ��
	M_n = 1.0;                        // �߹�ָ��
}

CMaterial::~CMaterial(void)
{

}

void CMaterial::SetAmbient(CRGB c)
{
	M_Ambient = c;
}

void CMaterial::SetDiffuse(CRGB c)
{
	M_Diffuse = c;
}

void CMaterial::SetSpecular(CRGB c)
{
	M_Specular = c;
}

void CMaterial::SetEmission(CRGB c)
{
	M_Emission = c;
}

void CMaterial::SetExponent(double n)
{
	M_n = n;
}
