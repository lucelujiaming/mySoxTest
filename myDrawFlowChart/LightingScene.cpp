#include "stdafx.h"
#include "LightingScene.h"
#include "math.h"

CLightingScene::CLightingScene(void)
{
	nLightNumber = 1;
	pLightSource = new CLightSource[nLightNumber];
	// �������ǳ���
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
/* �������	                                                            */
/*     ViewPoint - �ӵ�                                                 */
/*     Point     - ����                                                 */
/*     ptNormal  - �����������ʾ                                       */
/*     pMaterial - ��������Ĳ���                                       */
/* �������ʵ���˼򵥹���ģ�͡�                                         */
/************************************************************************/
/* �򵥹���ģ�ͱ�ʾΪ��                                                 */
/*     I = Ie + Id + Is                                                 */
/* ���У�                                                               */
/*     I  - ��ʾ�������һ�㷴�䵽�ӵ�Ĺ�ǿ��                          */
/*     Ie - ��ʾ������ǿ��                                              */
/*     Id - ��ʾ�������ǿ��                                            */
/*     Is - ��ʾ���淴���ǿ��                                          */
/* ���У�                                                               */
/*     ������ǿ     = ������Ambient * ������ķ�����M_Ambient   (8-2)   */
/*     �������ǿ   = (�����ǿ * cos(�����))NdotL             (8-3)   */
/*                    * ��������M_Diffuse��                             */
/*                  = ��Դ��ɫ * ������������ * �������Ĺ�ǿ          */
/*     ���淴���ǿ = �����ǿ * ���淴����M_Specular                   */
/*                    * (cos(Alpha)^n)                           (8-6)  */
/*     Alpha�Ķ��壺                                                    */
/*       �辵�淴������ΪR�����浽�ӵ������ΪV��AlphaΪR��V�ļнǡ�    */
/*       ����������λ������˵��cos(Alpha)Ϊ�����������ĵ����           */
/************************************************************************/
/* ���������Ĺ�ǿ���Ź�Դ������֮���������Ӷ�������                 */
/* ǿ�Ȱ��չ�Դ���������d��1/(d^2)����˥����                           */
/* ���ڵ��Դ����ǿ˥��Ϊd�Ķ��κ����ĵ������μ���ʽ(8-14)              */
/*     f(d) = min(1, 1/(c0 + c1 * d + c2 * d * d))                      */
/* ����ϣ�ǰ��ļ򵥹���ģ�͹�ʽI = Ie + Id + Is ������Ϊ��            */
/*     I = Ie + f(d) * (Id + Is)                                        */
/* Ҳ�������ϵĹ�ʽ(8-15)                                               */
/************************************************************************/
/* �⺯�����ݹ�ʽ(8-15)���ȼ����������ǿId��֮����㾵�淴���ǿIs��   */
/* ֮���ǹ�ǿ˥���������㻷����ǿIe��                               */
/************************************************************************/
CRGB CLightingScene::Illuminate(CColorP3 ViewPoint, CColorP3 Point, CVector3 ptNormal, CMaterial* pMaterial)
{
	CRGB ResultI = pMaterial->M_Emission;//����������Ϊ��ʼֵ
	for (int loop = 0; loop < nLightNumber; loop++)//����Դ����״̬
	{
		if (pLightSource[loop].L_OnOff)//��Դ��
		{
			// I�������䡱��ǿ
			CRGB I = CRGB(0.0, 0.0, 0.0);
			// LΪ��ʸ����Ҳ����Point�͹�Դλ�õ����ʸ����
			CVector3 L(Point, pLightSource[loop].L_Position);
			// dΪ�⴫���ľ���
			double d = L.Magnitude(); 
			L = L.Normalize();//��һ����ʸ����
			// NΪ���������
			CVector3 N = ptNormal;
			N = N.Normalize();//��һ����ʸ��

			// ��1��������������⡣�ο���ʽ(8-3)��
			// ���Ϊ��˵�������Ϊ0 - 90�ȡ��������Ϊ����
			// ˵������Ǵ���90�ȣ���Ըö���Ĺ�ǿ����Ϊ�㡣
			// ��������max�����ĺ��塣�ο���ʽ(8-5)��
			// ��ΪN��L���ǹ�һ����ĵ�λ����������ϣ�N��L�ĵ�����Ǽнǵ����ҡ�
			double NdotL = max(DotProduct(N, L), 0);
			// ���㡰���䡱��ǿ = ��Դ��ɫ * ������������ * �������Ĺ�ǿ
			I += pLightSource[loop].L_Diffuse * pMaterial->M_Diffuse * NdotL;

			//��2�������뾵�淴��⡣�μ���ʽ(8-6)
			CVector3 V(Point, ViewPoint);  // VΪ�۲�ʸ��
			V = V.Normalize();             // ��һ���۲�ʸ��
			CVector3 H = (L + V) / (L + V).Magnitude();  // HΪ��ֵʸ��
			// �辵�淴������ΪR�����浽�ӵ������ΪV��
			// ��ΪN��L���ǹ�һ����ĵ�λ������ʹ�õ�����Լ���R��V�ļнǵ����ҡ�
			double NdotH = max(DotProduct(N, H), 0);
			// ����(cos(Alpha)^n)�����Ƶ��������淴���Ŀռ�ֲ���
			double Rs = pow(NdotH, pMaterial->M_n);
			// ʹ�ù�ʽ(8-6)���㾵�淴���ǿ
			I += pLightSource[loop].L_Specular * pMaterial->M_Specular * Rs;

			//��3������ǿ˥��
			double c0 = pLightSource[loop].L_C0;//c0Ϊ����˥������
			double c1 = pLightSource[loop].L_C1;//c1Ϊ����˥������
			double c2 = pLightSource[loop].L_C2;//c2Ϊ����˥������
			// ���ڵ��Դ����ǿ˥��Ϊd�Ķ��κ����ĵ������μ���ʽ(8-14)
			double f = (1.0 / (c0 + c1 * d + c2 * d * d));//��ǿ˥������
			f = min(1.0, f);
			ResultI += I * f;
		}
		else
		{
			ResultI += Point.c;//����������ɫ
		}
	}
	//��4������������뻷���⡣��Ϊ������ܹ�ԴӰ�졣
	ResultI += Ambient * pMaterial->M_Ambient;
	//��5������ǿ��һ����[0,1]����
	ResultI.Normalize();
	//��6�������������㶥������յĹ�ǿ��ɫ
	return ResultI;
}