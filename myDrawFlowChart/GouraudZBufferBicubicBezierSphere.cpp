#include "stdafx.h"
#include "GouraudZBufferBicubicBezierSphere.h"

CGouraudZBufferBicubicBezierSphere::CGouraudZBufferBicubicBezierSphere(void)
{

}

CGouraudZBufferBicubicBezierSphere::~CGouraudZBufferBicubicBezierSphere(void)
{

}

// 读入点表
void CGouraudZBufferBicubicBezierSphere::ReadVertex(void)
{
	const double m = 0.5523;			//	魔术常数
	// 第1卦限控制点。共需13个控制点（北极点有4个重点）。只有三个位于曲面上。
	Ver[0].x  = 0.0,	Ver[0].y  = 1.0,	Ver[0].z  = 0.0;
	Ver[1].x  = 0.0,	Ver[1].y  = 1.0,	Ver[1].z  = m;
	Ver[2].x  = 0.0,	Ver[2].y  = m,		Ver[2].z  = 1.0;
	Ver[3].x  = 0.0,	Ver[3].y  = 0.0,	Ver[3].z  = 1.0;
	Ver[4].x  = m * m,	Ver[4].y  = 1.0,	Ver[4].z  = m;
	Ver[5].x  = m,		Ver[5].y  = m,		Ver[5].z  = 1.0;
	Ver[6].x  = m,		Ver[6].y  = 0.0,	Ver[6].z  = 1.0;
	Ver[7].x  = m,		Ver[7].y  = 1.0,	Ver[7].z  = m * m;
	Ver[8].x  = 1.0,	Ver[8].y  = m,		Ver[8].z  = m;
	Ver[9].x  = 1.0,	Ver[9].y  = 0.0,	Ver[9].z  = m;
	Ver[10].x = m,		Ver[10].y = 1.0,	Ver[10].z = 0.0;
	Ver[11].x = 1.0,	Ver[11].y = m,		Ver[11].z = 0.0;
	Ver[12].x = 1.0,	Ver[12].y = 0.0,	Ver[12].z = 0.0;
	//第2卦限控制点。共需9个控制点。
	Ver[13].x = m,		Ver[13].y = 1.0,	Ver[13].z = -m * m;
	Ver[14].x = 1.0,	Ver[14].y = m,		Ver[14].z = -m;
	Ver[15].x = 1.0,	Ver[15].y = 0.0,	Ver[15].z = -m;
	Ver[16].x = m * m,	Ver[16].y = 1.0,	Ver[16].z = -m;
	Ver[17].x = m,		Ver[17].y = m,		Ver[17].z = -1.0;
	Ver[18].x = m,		Ver[18].y = 0.0,	Ver[18].z = -1.0;
	Ver[19].x = 0.0,	Ver[19].y = 1.0,	Ver[19].z = -m;
	Ver[20].x = 0.0,	Ver[20].y = m,		Ver[20].z = -1.0;
	Ver[21].x = 0.0,	Ver[21].y = 0.0,	Ver[21].z = -1.0;
	//第3卦限控制点。共需9个控制点
	Ver[22].x =-m * m,	Ver[22].y = 1.0,	Ver[22].z = -m;
	Ver[23].x =-m,		Ver[23].y = m,		Ver[23].z = -1.0;
	Ver[24].x =-m,		Ver[24].y = 0.0,	Ver[24].z = -1.0;
	Ver[25].x =-m,		Ver[25].y = 1.0,	Ver[25].z = -m * m;
	Ver[26].x = -1.0,	Ver[26].y = m,		Ver[26].z = -m;
	Ver[27].x = -1.0,	Ver[27].y = 0.0,	Ver[27].z = -m;
	Ver[28].x =-m,		Ver[28].y = 1.0,	Ver[28].z = 0.0;
	Ver[29].x = -1.0,	Ver[29].y = m,		Ver[29].z = 0.0;
	Ver[30].x = -1.0,	Ver[30].y = 0.0,	Ver[30].z = 0.0;
	//第4卦限控制点。共需9个控制点
	Ver[31].x =-m,		Ver[31].y = 1.0,	Ver[31].z = m * m;
	Ver[32].x = -1.0,	Ver[32].y = m,		Ver[32].z = m;
	Ver[33].x = -1.0,	Ver[33].y = 0.0,	Ver[33].z = m;
	Ver[34].x =-m * m,	Ver[34].y = 1.0,	Ver[34].z = m;
	Ver[35].x =-m,		Ver[35].y = m,		Ver[35].z = 1.0;
	Ver[36].x =-m,		Ver[36].y = 0.0,	Ver[36].z = 1.0;
	//第5卦限控制点。共需9个控制点
	Ver[37].x = 0.0,	Ver[37].y =-m,		Ver[37].z = 1.0;
	Ver[38].x = 0.0,	Ver[38].y = -1.0,	Ver[38].z = m;
	Ver[39].x = m,		Ver[39].y =-m,		Ver[39].z = 1.0;
	Ver[40].x = m * m,	Ver[40].y = -1.0,	Ver[40].z = m;
	Ver[41].x = 1.0,	Ver[41].y =-m,		Ver[41].z = m;
	Ver[42].x = m,		Ver[42].y = -1.0,	Ver[42].z = m * m;
	Ver[43].x =  1.0,	Ver[43].y =-m,		Ver[43].z = 0.0;
	Ver[44].x = m,		Ver[44].y = -1.0,	Ver[44].z = 0.0;
	//第6卦限控制点。共需6个控制点
	Ver[45].x = 1.0,	Ver[45].y =-m,		Ver[45].z =-m;
	Ver[46].x = m,		Ver[46].y = -1.0,	Ver[46].z =-m * m;
	Ver[47].x = m,		Ver[47].y =-m,		Ver[47].z = -1.0;
	Ver[48].x = m * m,	Ver[48].y = -1.0,	Ver[48].z = -m;
	Ver[49].x =  0.0,	Ver[49].y =-m,		Ver[49].z = -1.0;
	Ver[50].x =  0.0,	Ver[50].y = -1.0,	Ver[50].z = -m;
	//第7卦限控制点。共需6个控制点
	Ver[51].x =-m,		Ver[51].y =-m,		Ver[51].z = -1.0;
	Ver[52].x =-m * m,	Ver[52].y = -1.0,	Ver[52].z = -m;
	Ver[53].x = -1.0,	Ver[53].y =-m,		Ver[53].z = -m;
	Ver[54].x =-m,		Ver[54].y = -1.0,	Ver[54].z =-m * m;
	Ver[55].x = -1.0,	Ver[55].y =-m,		Ver[55].z = -0.0;
	Ver[56].x =-m,		Ver[56].y = -1.0,	Ver[56].z = -0.0;
	//第8卦限控制点。共需5个控制点
	Ver[57].x = -1.0,	Ver[57].y =-m,		Ver[57].z = m;
	Ver[58].x =-m,		Ver[58].y = -1.0,	Ver[58].z = m * m;
	Ver[59].x =-m,		Ver[59].y =-m,		Ver[59].z =  1.0;
	Ver[60].x =-m * m,	Ver[60].y =  -1.0,	Ver[60].z = m;
	Ver[61].x = 0.0,	Ver[61].y = -1.0,	Ver[61].z = 0;
}

// 读入曲面表。也就是给出每一个卦限面片的顶点索引
void CGouraudZBufferBicubicBezierSphere::ReadFace(void)
{
	//第1卦限面片
	Pat[0].ptIndex[0]  = 3,  Pat[0].ptIndex[1]  = 6,  Pat[0].ptIndex[2]  = 9,  Pat[0].ptIndex[3]  = 12;
	Pat[0].ptIndex[4]  = 2,  Pat[0].ptIndex[5]  = 5,  Pat[0].ptIndex[6]  = 8,  Pat[0].ptIndex[7]  = 11;
	Pat[0].ptIndex[8]  = 1,  Pat[0].ptIndex[9]  = 4;  Pat[0].ptIndex[10] = 7;  Pat[0].ptIndex[11] = 10;
	Pat[0].ptIndex[12] = 0,  Pat[0].ptIndex[13] = 0,  Pat[0].ptIndex[14] = 0,  Pat[0].ptIndex[15] = 0;
	//第2卦限面片
	Pat[1].ptIndex[0]  = 12, Pat[1].ptIndex[1]  = 15, Pat[1].ptIndex[2]  = 18, Pat[1].ptIndex[3]  = 21;
	Pat[1].ptIndex[4]  = 11, Pat[1].ptIndex[5]  = 14, Pat[1].ptIndex[6]  = 17, Pat[1].ptIndex[7]  = 20;
	Pat[1].ptIndex[8]  = 10, Pat[1].ptIndex[9]  = 13, Pat[1].ptIndex[10] = 16, Pat[1].ptIndex[11] = 19;
	Pat[1].ptIndex[12] = 0,  Pat[1].ptIndex[13] = 0,  Pat[1].ptIndex[14] = 0,  Pat[1].ptIndex[15] = 0;
	//第3卦限面片
	Pat[2].ptIndex[0]  = 21, Pat[2].ptIndex[1]  = 24, Pat[2].ptIndex[2]  = 27, Pat[2].ptIndex[3]  = 30;
	Pat[2].ptIndex[4]  = 20, Pat[2].ptIndex[5]  = 23, Pat[2].ptIndex[6]  = 26, Pat[2].ptIndex[7]  = 29;
	Pat[2].ptIndex[8]  = 19, Pat[2].ptIndex[9]  = 22, Pat[2].ptIndex[10] = 25, Pat[2].ptIndex[11] = 28;
	Pat[2].ptIndex[12] = 0,  Pat[2].ptIndex[13] = 0,  Pat[2].ptIndex[14] = 0,  Pat[2].ptIndex[15] = 0;
	//第4卦限面片
	Pat[3].ptIndex[0] = 30, Pat[3].ptIndex[1] = 33, Pat[3].ptIndex[2] = 36, Pat[3].ptIndex[3] = 3;
	Pat[3].ptIndex[4] = 29, Pat[3].ptIndex[5] = 32, Pat[3].ptIndex[6] = 35, Pat[3].ptIndex[7] = 2;
	Pat[3].ptIndex[8] = 28, Pat[3].ptIndex[9] = 31, Pat[3].ptIndex[10] = 34, Pat[3].ptIndex[11] = 1;
	Pat[3].ptIndex[12] = 0, Pat[3].ptIndex[13] = 0, Pat[3].ptIndex[14] = 0, Pat[3].ptIndex[15] = 0;
	//第5卦限面片
	Pat[4].ptIndex[0] = 61, Pat[4].ptIndex[1] = 61, Pat[4].ptIndex[2] = 61, Pat[4].ptIndex[3] = 61;
	Pat[4].ptIndex[4] = 38, Pat[4].ptIndex[5] = 40, Pat[4].ptIndex[6] = 42, Pat[4].ptIndex[7] = 44;
	Pat[4].ptIndex[8] = 37, Pat[4].ptIndex[9] = 39, Pat[4].ptIndex[10] = 41, Pat[4].ptIndex[11] = 43;
	Pat[4].ptIndex[12] = 3, Pat[4].ptIndex[13] = 6, Pat[4].ptIndex[14] = 9, Pat[4].ptIndex[15] = 12;
	//第6卦限面片
	Pat[5].ptIndex[0] = 61, Pat[5].ptIndex[1] = 61, Pat[5].ptIndex[2] = 61, Pat[5].ptIndex[3] = 61;
	Pat[5].ptIndex[4] = 44, Pat[5].ptIndex[5] = 46, Pat[5].ptIndex[6] = 48, Pat[5].ptIndex[7] = 50;
	Pat[5].ptIndex[8] = 43, Pat[5].ptIndex[9] = 45, Pat[5].ptIndex[10] = 47, Pat[5].ptIndex[11] = 49;
	Pat[5].ptIndex[12] = 12, Pat[5].ptIndex[13] = 15, Pat[5].ptIndex[14] = 18, Pat[5].ptIndex[15] = 21;
	//第7卦限面片
	Pat[6].ptIndex[0] = 61, Pat[6].ptIndex[1] = 61, Pat[6].ptIndex[2] = 61, Pat[6].ptIndex[3] = 61;
	Pat[6].ptIndex[4] = 50, Pat[6].ptIndex[5] = 52, Pat[6].ptIndex[6] = 54, Pat[6].ptIndex[7] = 56;
	Pat[6].ptIndex[8] = 49, Pat[6].ptIndex[9] = 51, Pat[6].ptIndex[10] = 53, Pat[6].ptIndex[11] = 55;
	Pat[6].ptIndex[12] = 21, Pat[6].ptIndex[13] = 24, Pat[6].ptIndex[14] = 27, Pat[6].ptIndex[15] = 30;
	//第8卦限面片//
	Pat[7].ptIndex[0] = 61, Pat[7].ptIndex[1] = 61, Pat[7].ptIndex[2] = 61, Pat[7].ptIndex[3] = 61;
	Pat[7].ptIndex[4] = 56, Pat[7].ptIndex[5] = 58, Pat[7].ptIndex[6] = 60, Pat[7].ptIndex[7] = 38;
	Pat[7].ptIndex[8] = 55, Pat[7].ptIndex[9] = 57, Pat[7].ptIndex[10] = 59, Pat[7].ptIndex[11] = 37;
	Pat[7].ptIndex[12] = 30, Pat[7].ptIndex[13] = 33, Pat[7].ptIndex[14] = 36, Pat[7].ptIndex[15] = 3;
}

// 双三次Bezier球通常是由8片曲面片组成的。下面一个一个曲面片进行绘制。
void CGouraudZBufferBicubicBezierSphere::Draw(CDC* pDC, CZBuffer* pZBuffer) // 绘制图形
{
	CColorP3 P[4][4]; // 一片曲面的控制点
	// 双三次Bezier球通常是由8片曲面片组成的。
	for(int nPatch = 0; nPatch < 8; nPatch++)
	{
		for(int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				P[i][j] = Ver[Pat[nPatch].ptIndex[i * 4 + j]];
			}
		}
		// 设定控制点。
		bezier.ReadControlPoint(P);
		// 根据控制点绘制面片。
		bezier.Draw(pDC, pZBuffer);
	}
}