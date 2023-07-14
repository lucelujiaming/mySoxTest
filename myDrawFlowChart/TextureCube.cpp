
#include "stdafx.h"
#include "TextureCube.h"

CTextureCube::CTextureCube(void)
{

}

CTextureCube::~CTextureCube(void)
{

}

/************************************************************************/
/* ���������Ķ����������ϵ�е�ʾ��ͼ���£�                           */
/*             ��Y                                                      */
/*           3 ��                                                       */
/*           ����-------��2                                             */
/*         ��  ��     ����                                              */
/*     7 ��    ��  6��  ��                                              */
/*       |-----ʮ---    ��                                              */
/*       |   0 ʮ��ʮ�������� X                                         */
/*       |   ��    |  �� 1                                              */
/*     4 | ��      |��                                                  */
/*       ��-------- 5                                                   */
/*   Z �L                                                               */
/*                                                                      */
/* ���Կ�������X�����ң�Y�����ϣ�Z��ָ�����ǡ�                          */
/* ��������������ǳ���߶�Ϊ1�ĵ�λ�����塣                            */
/************************************************************************/
void CTextureCube::ReadVertex(void)//���
{
	V[0].x = 0, V[0].y = 0, V[0].z = 0;
	V[1].x = 1, V[1].y = 0, V[1].z = 0;
	V[2].x = 1, V[2].y = 1, V[2].z = 0;
	V[3].x = 0, V[3].y = 1, V[3].z = 0;
	V[4].x = 0, V[4].y = 0, V[4].z = 1;
	V[5].x = 1, V[5].y = 0, V[5].z = 1;
	V[6].x = 1, V[6].y = 1, V[6].z = 1;
	V[7].x = 0, V[7].y = 1, V[7].z = 1;
}

/************************************************************************/
/* ��������ķ�����֪��X�����ң�Y�����ϣ�Z��ָ�����ǡ�                  */
/* ����������˸�����6����Ķ������⣬��������6������ͼ���ꡣ           */
/* Ҳ���ǽ�λͼ��uv��ͼ����󶨵�������6����ı�����С�                */
/************************************************************************/
/* ���������������uv��ͼ���ꡣ����Ϊ�����۷��㣬�������Ƚ��й�һ����   */
/* Ҳ���Ǽ���λͼ��һ������Ϊ1��ͼƬ��                                */
/* ֮���������չ����һ��������չ���кܶ��ַ��������ǳ��õ������֣�     */
/*              +--------+                                              */
/*              |        |                                              */
/*              |   ��   |                                              */
/*              |        |                                              */
/*     +--------+--------+--------+--------+                            */
/*     |        |        |        |        |                            */
/*     |  ��    |   ǰ   |   ��   |   ��   |                            */
/*     |        |        |        |        |                            */
/*     +--------+--------+--------+--------+                            */
/*              |        |                                              */
/*              |   ��   |                                              */
/*              |        |                                              */
/*             -+--------+                                              */
/* ������չ����ʽ�������³���Ϊ1��λͼ�ϡ�                            */
/* ���Կ�������X����Ϊ�ĵȷ֣���Y����Ϊ���ȷ֡�                         */
/* �����uv��ͼ����ͺ����׵õ��ˡ�������λͼ�ϱ�����£�               */
/*   +-------+--------+-----------------+                               */
/*   |(0,1)  |(0.25,  |(0.5,1)          |(1, 1)                         */
/*   |       |    1)  |                 |                               */
/*   |       |        |                 |                               */
/*   +-------+--------+--------+--------+  (1,                          */
/*   | (0,   |(0.25,  |(0.5,   |(0.75,  | 0.33)                         */
/*   |0.66)  | 0.66)  | 0.66)  | 0.66)  |                               */
/*   |       |        |        |        |                               */
/*   +-------+--------+--------+--------+                               */
/*   | (0,   |(0.25,  |(0.5,   (0.75,   |                               */
/*   |0.33)  | 0.33)  | 0.33)   0.33)   |                               */
/*   |       |        |                 |                               */
/*   +-------+--------+-----------------+                               */
/*  (0, 0) (0.25, 0) (0.5, 0)           (1, 0)                          */
/* ��������������ַ����Щ���ֵ���Դ��                                 */
/************************************************************************/
void CTextureCube::ReadFace(void)//���
{
    // ��������ķ�����֪��
	// �������ǵ������������4,5,6,7�ĸ�����ɡ�
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7;  // ǰ��
	F[0].t[0] = CTextureCoordinate(0.25, 0.33);
	F[0].t[1] = CTextureCoordinate(0.5, 0.33);
	F[0].t[2] = CTextureCoordinate(0.5, 0.66);
	F[0].t[3] = CTextureCoordinate(0.25, 0.66); // ǰ�������ַ
	// �������ǵ������������0,3,2,1�ĸ�����ɡ�
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1;  // ����
	F[1].t[0] = CTextureCoordinate(1, 0.33);
	F[1].t[1] = CTextureCoordinate(1, 0.66);
	F[1].t[2] = CTextureCoordinate(0.75, 0.66);
	F[1].t[3] = CTextureCoordinate(0.75, 0.33); // ���������ַ
	// ���������������0,4,7,3�ĸ�����ɡ�
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3;  // ����
	F[2].t[0] = CTextureCoordinate(0, 0.33);
	F[2].t[1] = CTextureCoordinate(0.25, 0.33);
	F[2].t[2] = CTextureCoordinate(0.25, 0.66);
	F[2].t[3] = CTextureCoordinate(0, 0.66);    // ���������ַ
	// ��������Ҳ������1,2,6,5�ĸ�����ɡ�
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5;  // ����
	F[3].t[0] = CTextureCoordinate(0.75, 0.33);
	F[3].t[1] = CTextureCoordinate(0.75, 0.66);
	F[3].t[2] = CTextureCoordinate(0.5, 0.66);
	F[3].t[3] = CTextureCoordinate(0.5, 0.33); // ���������ַ
	// ������Ķ��˱�����2,3,7,6�ĸ�����ɡ�
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6;   // ����
	F[4].t[0] = CTextureCoordinate(0.5, 1);
	F[4].t[1] = CTextureCoordinate(0.25, 1);
	F[4].t[2] = CTextureCoordinate(0.25, 0.66);
	F[4].t[3] = CTextureCoordinate(0.5, 0.66);//���������ַ
	// ������ĵ׶˱�����0,1,5,4�ĸ�����ɡ�
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4;   // ����
	F[5].t[0] = CTextureCoordinate(0.25, 0);
	F[5].t[1] = CTextureCoordinate(0.5, 0);
	F[5].t[2] = CTextureCoordinate(0.5, 0.33);
	F[5].t[3] = CTextureCoordinate(0.25, 0.33);	// ���������ַ
}

void CTextureCube::SetLightingScene(CLightingScene* pScene)
{
	this->pScene = pScene;
}

void CTextureCube::SetTexture(CTexture* pTexture)
{
	this->pTexture = pTexture;
}


/************************************************************************/
/* �����������������Ļ��������ǾͿ��Ի����������ˡ�                     */
/* CZBufferCubeʹ�ô����CZBuffer���л��ơ�                             */
/* ��������������һ�������߼�������                                   */
// ��������������                                                       */
// pDC      - ��ͼ�����                                                */
// pZBuffer - ��Ȼ�����������ʹ��ͬһ����Ȼ��������ƶ�����塣        */
//            ��Ϊ��Ȼ�������������֪���Լ�Ҫ���Ƶ���ʲô��            */
//            ���ͻ���������Ϣ��������Ρ�                            */
/************************************************************************/
void CTextureCube::Draw(CDC* pDC, CTextureZBuffer* pZBuffer)
{
	CColorP3 ScreenPoints[4];						// ÿһ�������άͶӰ��
	CColorP3 Eye = projection.GetColorEye();		// �ӵ�
	CVector3 vectorVertexsOfFace[4];				// ÿһ������ĸ�����ķ�ʸ��
	CTextureCoordinate objCoordinateOfFace[4];		// ÿһ������ĸ��������������
	// ��Ȼ�������ڻ��Ƶ�����������򵥵���ά���塣
	// ��������Ҳ��Ҫ���ջ��Ƹ�����ά����Ĳ�����л��ơ�
    // Ҳ����һ����һ������ơ�
	// ��ѭ����Ҳ����һ����һ������ơ�
	for (int nFace = 0; nFace < 6; nFace++)
	{
		// һ��������ĸ����㡣���0�����㵽��1������������ߵ�ʸ��
		CVector3 Vector01(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[1]]);
		// ���0�����㵽��2���������һ���ߵ�ʸ��
		CVector3 Vector02(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[2]]);
		// �����������ߵĲ�ˣ��õ���ķ�ʸ��
		CVector3 FaceNormal = CrossProduct(Vector01, Vector02);
		// ��һ����ʸ��
		FaceNormal = FaceNormal.Normalize();
		// �õ���������ж�������ꡣ
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // ����ѭ��
		{
			// ʹ����ά͸��ͶӰ�õ�͸�����ꡣ
			ScreenPoints[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// ÿ������ĸ����㷨ʸ�����趨Ϊ�����ķ�ʸ����
			vectorVertexsOfFace[nPoint] = FaceNormal;
			// Ϊ�˽�������ӳ�䣬������Ҫ��u�������λͼ�Ŀ�ȡ�
			objCoordinateOfFace[nPoint].u = F[nFace].t[nPoint].u * (pTexture->bmp.bmWidth - 1);
			// ��v�������λͼ�ĸ߶ȡ�
			objCoordinateOfFace[nPoint].v = F[nFace].t[nPoint].v * (pTexture->bmp.bmHeight - 1);
		}
		// �������ÿһ���涼��һ�����Σ����ǰ����ֳ����������ν�����ʾ��
		// ׼���������������Σ�Ϊ�������������ɶ���P
		CColorP3 objTopLeftPoint[3] = { ScreenPoints[0] ,ScreenPoints[2] ,ScreenPoints[3] };
		// Ϊ�������������ɷ�ʸ��N
		CVector3 objTopLeftNormal[3] = 
				{ vectorVertexsOfFace[0], vectorVertexsOfFace[2], vectorVertexsOfFace[3] };
		// Ϊ�������������������ַT
		CTextureCoordinate objTopLeftTexture[3] = 
				{ objCoordinateOfFace[0], objCoordinateOfFace[2], objCoordinateOfFace[3] };
		// Ϊ���������δ��붥��P����ʸ��N�������ַT
		pZBuffer->SetPoint(objTopLeftPoint, objTopLeftNormal, objTopLeftTexture);
		// ʹ���ӵ㳡���������������������
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
		
		// ׼���������������Σ�Ϊ�������������ɶ���P
		CColorP3 objDownRightPoint[3] = { ScreenPoints[0] ,ScreenPoints[1] ,ScreenPoints[2] };
		// Ϊ�������������ɷ�ʸ��N
		CVector3 objDownRightNormal[3] = { vectorVertexsOfFace[0], vectorVertexsOfFace[1], vectorVertexsOfFace[2] };
		// Ϊ�������������������ַT
		CTextureCoordinate objDownRightTexture[3] = { objCoordinateOfFace[0], objCoordinateOfFace[1], objCoordinateOfFace[2] };
		// Ϊ���������δ��붥��P����ʸ��N�������ַT
		pZBuffer->SetPoint(objDownRightPoint, objDownRightNormal, objDownRightTexture);
		// ʹ���ӵ㳡���������������������
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
	}
}