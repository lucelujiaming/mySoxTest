#include "stdafx.h"
#include "LightingScene.h"
#include "math.h"

CLightingScene::CLightingScene(void)
{
	nLightNumber = 1;
	pLightSource = new CLightSource[nLightNumber];
	Ambient = CRGB(0.3, 0.3, 0.3);//�������ǳ���
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
	CRGB ResultI = pMaterial->M_Emission;//����������Ϊ��ʼֵ
	for (int loop = 0; loop < nLightNumber; loop++)//����Դ����״̬
	{
		if (pLightSource[loop].L_OnOff)//��Դ��
		{
			CRGB I = CRGB(0.0, 0.0, 0.0);//I�������䡱��ǿ
			CVector3 L(Point, pLightSource[loop].L_Position);//LΪ��ʸ��
			double d = L.Magnitude();//dΪ�⴫���ľ��� 
			L = L.Normalize();//��һ����ʸ��
			CVector3 N = ptNormal;
			N = N.Normalize();//��һ����ʸ��
			//��1���������������
			double NdotL = max(DotProduct(N, L), 0);
			I += pLightSource[loop].L_Diffuse * pMaterial->M_Diffuse * NdotL;
			//��2�������뾵�淴���
			CVector3 V(Point, ViewPoint);//VΪ�۲�ʸ��
			V = V.Normalize();//��һ���۲�ʸ��
			CVector3 H = (L + V) / (L + V).Magnitude();//HΪ��ֵʸ��
			double NdotH = max(DotProduct(N, H), 0);
			double Rs = pow(NdotH, pMaterial->M_n);
			I += pLightSource[loop].L_Specular * pMaterial->M_Specular * Rs;
			//��3������ǿ˥��
			double c0 = pLightSource[loop].L_C0;//c0Ϊ����˥������
			double c1 = pLightSource[loop].L_C1;//c1Ϊ����˥������
			double c2 = pLightSource[loop].L_C2;//c2Ϊ����˥������
			double f = (1.0 / (c0 + c1 * d + c2 * d * d));//��ǿ˥������
			f = min(1.0, f);
			ResultI += I * f;
		}
		else
			ResultI += Point.c;//����������ɫ
	}
	//��4�������뻷����
	ResultI += Ambient * pMaterial->M_Ambient;
	//��5������ǿ��һ����[0,1]����
	ResultI.Normalize();
	//��6�������������㶥������յĹ�ǿ��ɫ
	return ResultI;
}