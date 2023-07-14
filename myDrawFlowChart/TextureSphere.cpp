#include "stdafx.h"
#include "TextureSphere.h"
#include "math.h"

CTextureSphere::CTextureSphere(void)
{

}

CTextureSphere::~CTextureSphere(void)
{

}

void CTextureSphere::ReadVertex(void)//读入点表
{
	//第1卦限控制点
	Ver[0].x = 0, Ver[0].y = 1, Ver[0].z = 0;
	Ver[1].x = 0, Ver[1].y = 1, Ver[1].z = 1;
	Ver[2].x = 0, Ver[2].y = 0, Ver[2].z = 1;
	Ver[3].x = 1, Ver[3].y = 1, Ver[3].z = 1;
	Ver[4].x = 1, Ver[4].y = 0, Ver[4].z = 1;
	Ver[5].x = 1, Ver[5].y = 1, Ver[5].z = 0;
	Ver[6].x = 1, Ver[6].y = 0, Ver[6].z = 0;
	Ver[7].x = 1, Ver[7].y = 1, Ver[7].z = -1;
	Ver[8].x = 1, Ver[8].y = 0, Ver[8].z = -1;
	Ver[9].x = 0, Ver[9].y = 1, Ver[9].z = -1;
	Ver[10].x = 0, Ver[10].y = 0, Ver[10].z = -1;
	Ver[11].x = -1, Ver[11].y = 1, Ver[11].z = -1;
	Ver[12].x = -1, Ver[12].y = 0, Ver[12].z = -1;
	Ver[13].x = -1, Ver[13].y = 1, Ver[13].z = 0;
	Ver[14].x = -1, Ver[14].y = 0, Ver[14].z = 0;
	Ver[15].x = -1, Ver[15].y = 1, Ver[15].z = 1;
	Ver[16].x = -1, Ver[16].y = 0, Ver[16].z = 1;
	Ver[17].x = 0, Ver[17].y = -1, Ver[17].z = 1;
	Ver[18].x = 1, Ver[18].y = -1, Ver[18].z = 1;
	Ver[19].x = 1, Ver[19].y = -1, Ver[19].z = 0;
	Ver[20].x = 1, Ver[20].y = -1, Ver[20].z = -1;
	Ver[21].x = 0, Ver[21].y = -1, Ver[21].z = -1;
	Ver[22].x = -1, Ver[22].y = -1, Ver[22].z = -1;
	Ver[23].x = -1, Ver[23].y = -1, Ver[23].z = 0;
	Ver[24].x = -1, Ver[24].y = -1, Ver[24].z = 1;
	Ver[25].x = 0, Ver[25].y = -1, Ver[25].z = 0;
}

void CTextureSphere::ReadFace(void)//读入曲面表
{
	//第1卦限面片
	Pat[0].ptIndex[0] = 2, Pat[0].ptIndex[1] = 4, Pat[0].ptIndex[2] = 6;
	Pat[0].ptIndex[3] = 1, Pat[0].ptIndex[4] = 3, Pat[0].ptIndex[5] = 5;
	Pat[0].ptIndex[6] = 0, Pat[0].ptIndex[7] = 0, Pat[0].ptIndex[8] = 0;
	//第2卦限面片
	Pat[1].ptIndex[0] = 6, Pat[1].ptIndex[1] = 8, Pat[1].ptIndex[2] = 10;
	Pat[1].ptIndex[3] = 5, Pat[1].ptIndex[4] = 7, Pat[1].ptIndex[5] = 9;
	Pat[1].ptIndex[6] = 0, Pat[1].ptIndex[7] = 0, Pat[1].ptIndex[8] = 0;
	//第3卦限面片
	Pat[2].ptIndex[0] = 10, Pat[2].ptIndex[1] = 12, Pat[2].ptIndex[2] = 14;
	Pat[2].ptIndex[3] = 9, Pat[2].ptIndex[4] = 11, Pat[2].ptIndex[5] = 13;
	Pat[2].ptIndex[6] = 0, Pat[2].ptIndex[7] = 0, Pat[2].ptIndex[8] = 0;
	//第4卦限面片
	Pat[3].ptIndex[0] = 14, Pat[3].ptIndex[1] = 16, Pat[3].ptIndex[2] = 2;
	Pat[3].ptIndex[3] = 13, Pat[3].ptIndex[4] = 15, Pat[3].ptIndex[5] = 1;
	Pat[3].ptIndex[6] = 0, Pat[3].ptIndex[7] = 0, Pat[3].ptIndex[8] = 0;
	//第5卦限面片
	Pat[4].ptIndex[0] = 25, Pat[4].ptIndex[1] = 25, Pat[4].ptIndex[2] = 25;
	Pat[4].ptIndex[3] = 17, Pat[4].ptIndex[4] = 18, Pat[4].ptIndex[5] = 19;
	Pat[4].ptIndex[6] = 2, Pat[4].ptIndex[7] = 4, Pat[4].ptIndex[8] = 6;
	//第6卦限面片
	Pat[5].ptIndex[0] = 25, Pat[5].ptIndex[1] = 25, Pat[5].ptIndex[2] = 25;
	Pat[5].ptIndex[3] = 19, Pat[5].ptIndex[4] = 20, Pat[5].ptIndex[5] = 21;
	Pat[5].ptIndex[6] = 6, Pat[5].ptIndex[7] = 8, Pat[5].ptIndex[8] = 10;
	//第7卦限面片
	Pat[6].ptIndex[0] = 25, Pat[6].ptIndex[1] = 25, Pat[6].ptIndex[2] = 25;
	Pat[6].ptIndex[3] = 21, Pat[6].ptIndex[4] = 22, Pat[6].ptIndex[5] = 23;
	Pat[6].ptIndex[6] = 10, Pat[6].ptIndex[7] = 12, Pat[6].ptIndex[8] = 14;
	//第8卦限面片
	Pat[7].ptIndex[0] = 25, Pat[7].ptIndex[1] = 25, Pat[7].ptIndex[2] = 25;
	Pat[7].ptIndex[3] = 23, Pat[7].ptIndex[4] = 24, Pat[7].ptIndex[5] = 17;
	Pat[7].ptIndex[6] = 14, Pat[7].ptIndex[7] = 16, Pat[7].ptIndex[8] = 2;
}

void CTextureSphere::Draw(CDC* pDC, CTextureZBuffer* pZBuffer)//绘制图形
{
	CColorP3 P[3][3];//三维顶点
	double W[3][3];//权因子
	const double c = sqrt(2.0) / 2.0;
	W[0][0] = 1, W[0][1] = c,	  W[0][2] = 1;
	W[1][0] = c, W[1][1] = c * c, W[1][2] = c;
	W[2][0] = 1, W[2][1] = c,	  W[2][2] = 1;
	for (int nPatch = 0; nPatch < 8; nPatch++)
	{
		for (int i = 0; i < 3; i++)
        {
			for (int j = 0; j < 3; j++)
            {
				P[i][j] = Ver[Pat[nPatch].ptIndex[i * 3 + j]]; 
            }
        }
		bezier.ReadControlPoint(P);
		bezier.ReadWeight(W);
		bezier.Draw(pDC, pZBuffer);
	}
}
