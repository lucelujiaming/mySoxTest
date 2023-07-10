#include "stdafx.h"
#include "LightingScene.h"
#include "math.h"

CLightingScene::CLightingScene(void)
{
	nLightNumber = 1;
	pLightSource = new CLightSource[nLightNumber];
	// 环境光是常数
	Ambient = CRGB(0.3, 0.3, 0.3);
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

/************************************************************************/
/* 计算光照	                                                            */
/*     ViewPoint - 视点                                                 */
/*     Point     - 顶点                                                 */
/*     ptNormal  - 顶点的向量表示                                       */
/*     pMaterial - 绘制物体的材质                                       */
/* 这个函数实现了简单光照模型。                                         */
/************************************************************************/
/* 简单光照模型表示为：                                                 */
/*     I = Ie + Id + Is                                                 */
/* 其中：                                                               */
/*     I  - 表示物体表面一点反射到视点的光强。                          */
/*     Ie - 表示环境光强。                                              */
/*     Id - 表示漫反射光强。                                            */
/*     Is - 表示镜面反射光强。                                          */
/* 其中：                                                               */
/*     环境光强     = 环境光Ambient * 环境光的反射率M_Ambient   (8-2)   */
/*     漫反射光强   = (入射光强 * cos(入射角))NdotL             (8-3)   */
/*                    * 漫反射率M_Diffuse。                             */
/*                  = 光源颜色 * 材质漫反射率 * 漫反射光的光强          */
/*     镜面反射光强 = 入射光强 * 镜面反射率M_Specular                   */
/*                    * (cos(Alpha)^n)                           (8-6)  */
/*     Alpha的定义：                                                    */
/*       设镜面反射向量为R，表面到视点的向量为V。Alpha为R和V的夹角。    */
/*       对于两个单位向量来说，cos(Alpha)为这两个向量的点积。           */
/************************************************************************/
/* 另外入射光的光强随着光源与物体之间距离的增加而减弱。                 */
/* 强度按照光源到物体距离d的1/(d^2)进行衰减。                           */
/* 对于点光源，光强衰减为d的二次函数的倒数。参见公式(8-14)              */
/*     f(d) = min(1, 1/(c0 + c1 * d + c2 * d * d))                      */
/* 因此上，前面的简单光照模型公式I = Ie + Id + Is 被修正为：            */
/*     I = Ie + f(d) * (Id + Is)                                        */
/* 也就是书上的公式(8-15)                                               */
/************************************************************************/
/* 这函数根据公式(8-15)首先计算漫反射光强Id，之后计算镜面反射光强Is，   */
/* 之后考虑光强衰减，最后计算环境光强Ie。                               */
/************************************************************************/
CRGB CLightingScene::Illuminate(CColorP3 ViewPoint, CColorP3 Point, CVector3 ptNormal, CMaterial* pMaterial)
{
	CRGB ResultI = pMaterial->M_Emission;//材质自身发光为初始值
	for (int loop = 0; loop < nLightNumber; loop++)//检查光源开关状态
	{
		if (pLightSource[loop].L_OnOff)//光源开
		{
			// I代表“反射”光强
			CRGB I = CRGB(0.0, 0.0, 0.0);
			// L为光矢量。也就是Point和光源位置的相对矢量。
			CVector3 L(Point, pLightSource[loop].L_Position);
			// d为光传播的距离
			double d = L.Magnitude(); 
			L = L.Normalize();//归一化光矢量、
			// N为顶点的向量
			CVector3 N = ptNormal;
			N = N.Normalize();//归一化法矢量

			// 第1步，加入漫反射光。参考公式(8-3)。
			// 如果为正说明入射角为0 - 90度。否则如果为负，
			// 说明入射角大于90度，则对该顶点的光强贡献为零。
			// 这就是这个max函数的含义。参考公式(8-5)。
			// 因为N和L都是归一化后的单位向量，因此上，N和L的点积就是夹角的余弦。
			double NdotL = max(DotProduct(N, L), 0);
			// 计算“反射”光强 = 光源颜色 * 材质漫反射率 * 漫反射光的光强
			I += pLightSource[loop].L_Diffuse * pMaterial->M_Diffuse * NdotL;

			//第2步，加入镜面反射光。参见公式(8-6)
			CVector3 V(Point, ViewPoint);  // V为观察矢量
			V = V.Normalize();             // 归一化观察矢量
			CVector3 H = (L + V) / (L + V).Magnitude();  // H为中值矢量
			// 设镜面反射向量为R，表面到视点的向量为V。
			// 因为N和L都是归一化后的单位向量，使用点积可以计算R和V的夹角的余弦。
			double NdotH = max(DotProduct(N, H), 0);
			// 计算(cos(Alpha)^n)来近似的描述镜面反射光的空间分布。
			double Rs = pow(NdotH, pMaterial->M_n);
			// 使用公式(8-6)计算镜面反射光强
			I += pLightSource[loop].L_Specular * pMaterial->M_Specular * Rs;

			//第3步，光强衰减
			double c0 = pLightSource[loop].L_C0;//c0为常数衰减因子
			double c1 = pLightSource[loop].L_C1;//c1为线性衰减因子
			double c2 = pLightSource[loop].L_C2;//c2为二次衰减因子
			// 对于点光源，光强衰减为d的二次函数的倒数。参见公式(8-14)
			double f = (1.0 / (c0 + c1 * d + c2 * d * d));//光强衰减函数
			f = min(1.0, f);
			ResultI += I * f;
		}
		else
		{
			ResultI += Point.c;//物体自身颜色
		}
	}
	//第4步，最后加入加入环境光。因为这个不受光源影响。
	ResultI += Ambient * pMaterial->M_Ambient;
	//第5步，光强归一化到[0,1]区间
	ResultI.Normalize();
	//第6步，返回所计算顶点的最终的光强颜色
	return ResultI;
}