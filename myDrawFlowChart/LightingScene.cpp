#include "stdafx.h"
#include "LightingScene.h"
#include "math.h"

CLightingScene::CLightingScene(void)
{
	nLightNumber = 1;
	pLightSource = new CLightSource[nLightNumber];
	Ambient = CRGB(0.3, 0.3, 0.3);//环境光是常数
}

CLightingScene::CLightingScene(int nLightNumber)
{
	this->nLightNumber = nLightNumber;
	pLightSource = new CLightSource[nLightNumber];
	Ambient = CRGB(0.3, 0.3, 0.3);
}

CLightingScene::~CLightingScene(void)
{
	if (pLightSource)
	{
		delete[]pLightSource;
		pLightSource = NULL;
	}
	if (pMaterial)
	{
		delete pMaterial;
		pMaterial = NULL;
	}
}

void CLightingScene::SetLightNumber(int nLightNumber)
{
	if (pLightSource)
		delete[]pLightSource;
	this->nLightNumber = nLightNumber;
	pLightSource = new CLightSource[nLightNumber];
}

CRGB CLightingScene::Illuminate(CColorP3 ViewPoint, CColorP3 Point, CVector3 ptNormal, CMaterial* pMaterial)
{
	CRGB ResultI = pMaterial->M_Emission;//材质自身发光为初始值
	for (int loop = 0; loop < nLightNumber; loop++)//检查光源开关状态
	{
		if (pLightSource[loop].L_OnOff)//光源开
		{
			CRGB I = CRGB(0.0, 0.0, 0.0);//I代表“反射”光强
			CVector3 L(Point, pLightSource[loop].L_Position);//L为光矢量
			double d = L.Magnitude();//d为光传播的距离 
			L = L.Normalize();//归一化光矢量
			CVector3 N = ptNormal;
			N = N.Normalize();//归一化法矢量
			//第1步，加入漫反射光
			double NdotL = max(DotProduct(N, L), 0);
			I += pLightSource[loop].L_Diffuse * pMaterial->M_Diffuse * NdotL;
			//第2步，加入镜面反射光
			CVector3 V(Point, ViewPoint);//V为观察矢量
			V = V.Normalize();//归一化观察矢量
			CVector3 H = (L + V) / (L + V).Magnitude();//H为中值矢量
			double NdotH = max(DotProduct(N, H), 0);
			double Rs = pow(NdotH, pMaterial->M_n);
			I += pLightSource[loop].L_Specular * pMaterial->M_Specular * Rs;
			//第3步，光强衰减
			double c0 = pLightSource[loop].L_C0;//c0为常数衰减因子
			double c1 = pLightSource[loop].L_C1;//c1为线性衰减因子
			double c2 = pLightSource[loop].L_C2;//c2为二次衰减因子
			double f = (1.0 / (c0 + c1 * d + c2 * d * d));//光强衰减函数
			f = min(1.0, f);
			ResultI += I * f;
		}
		else
			ResultI += Point.c;//物体自身颜色
	}
	//第4步，加入环境光
	ResultI += Ambient * pMaterial->M_Ambient;
	//第5步，光强归一化到[0,1]区间
	ResultI.Normalize();
	//第6步，返回所计算顶点的最终的光强颜色
	return ResultI;
}