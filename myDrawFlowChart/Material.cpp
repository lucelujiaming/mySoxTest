#include "stdafx.h"
#include "Material.h"

CMaterial::CMaterial(void)
{
	M_Ambient = CRGB(0.2, 0.2, 0.2);  // 材质的环境反射率。为灰色。
	M_Diffuse = CRGB(0.8, 0.8, 0.8);  // 材质的漫反射率。为灰色。
	M_Specular = CRGB(0.0, 0.0, 0.0); // 材质的镜面反射率。为白色。
	M_Emission = CRGB(0.0, 0.0, 0.0); // 材质的辐射光。为白色。
	M_n = 1.0;                        // 高光指数
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
