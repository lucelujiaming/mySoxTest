#include "stdafx.h"
#include "ZBufferCube.h"
#define ROUND(d) int(d + 0.5)//四舍五入的宏定义

CZBufferCube::CZBufferCube(void)
{

}

CZBufferCube::~CZBufferCube(void)
{

}

/************************************************************************/
/* 这个立方体的顶点表在坐标系中的示意图如下：                           */
/*             ↑Y                                                      */
/*           3 ｜                                                       */
/*           ／｜-------｜2                                             */
/*         ／  ｜     ／｜                                              */
/*     7 ／    ｜  6／  ｜                                              */
/*       |-----十---    ｜                                              */
/*       |   0 十－十－－－→ X                                         */
/*       |   ／    |  ／ 1                                              */
/*     4 | ／      |／                                                  */
/*       ／-------- 5                                                   */
/*   Z ↙                                                               */
/*                                                                      */
/* 可以看出来，X轴向右，Y轴向上，Z轴指向我们。                          */
/* 这个函数给出的是长宽高都为1的单位立方体。                            */
/************************************************************************/
void CZBufferCube::ReadVertex(void)//点表
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

// 根据上面的分析可知，X轴向右，Y轴向上，Z轴指向我们。
void CZBufferCube::ReadFace(void) // 面表
{
    // 根据上面的分析可知：
	// 朝向我们的立方体表面由4,5,6,7四个点组成。
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7;  // 前面
	F[0].c = CRGB(1.0, 0.0, 0.0);              // 前面的颜色：红
	// 背对我们的立方体表面由0,3,2,1四个点组成。
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1;  // 后面
	F[1].c = CRGB(0.0, 1.0, 0.0);              // 后面的颜色：绿
	// 立方体的左侧表面由0,4,7,3四个点组成。
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3;  // 左面
	F[2].c = CRGB(0.0, 0.0, 1.0);              // 左面的颜色：蓝
	// 立方体的右侧表面由1,2,6,5四个点组成。
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5;  // 右面
	F[3].c = CRGB(1.0, 1.0, 0.0);              // 右面的颜色：黄
	// 立方体的顶端表面由2,3,7,6四个点组成。
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6;   // 顶面
	F[4].c = CRGB(1.0, 0.0, 1.0);               // 顶面的颜色：品红
	// 立方体的底端表面由0,1,5,4四个点组成。
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4;   // 底面
	F[5].c = CRGB(0.0, 1.0, 1.0);               // 底面的颜色：青
}

/************************************************************************/
/* 有了上面两个函数的基础，我们就可以绘制立方体了。                     */
/* CZBufferCube使用传入的CZBuffer进行绘制。                             */
/* 因此上这个函数是一个核心逻辑函数。                                   */
// 传入三个参数：                                                       */
// pDC      - 绘图句柄。                                                */
// pZBuffer - 深度缓冲器。我们使用同一个深度缓冲器绘制多个物体。        */
//            作为深度缓冲器，它并不知道自己要绘制的是什么。            */
//            他就会根据深度信息填充三角形。                            */
// nSign    - 立方体编号。用于分配不同的颜色。                          */
/************************************************************************/
void CZBufferCube::Draw(CDC* pDC, CZBuffer* pZBuffer, int nSign)
{
	CColorP3 ScreenPoint[4];//三维投影点
	// 虽然我们现在绘制的立方体是最简单的三维物体。
	// 但是我们也需要按照绘制复杂三维物体的步骤进行绘制。
    // 也就是一个面一个面绘制。
	// 面循环：也就是一个面一个面绘制。
	for (int nFace = 0; nFace < 6; nFace++)
	{
		// 得到这个面所有顶点的坐标。
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // 顶点循环
		{
			// 使用三维透视投影得到透视坐标。
			ScreenPoint[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// 根据立方体编号分配不同的颜色。 
//			if (nSign == 0)
//				ScreenPoint[nPoint].c = CRGB(0.5, 0.0, 0.0);   // 红
//			else if (nSign == 1)
//				ScreenPoint[nPoint].c = CRGB(0.0, 0.5, 0.0);   // 绿
//			else if (nSign == 2)
//				ScreenPoint[nPoint].c = CRGB(0.0, 0.0, 0.5);   // 蓝
//			else
				ScreenPoint[nPoint].c = F[nFace].c;
		}
		// 立方体的每一个面都是一个矩形，我们把他分成两个三角形进行显示。
        // 上三角形
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[2], ScreenPoint[3]); 
		pZBuffer->FillTriangle(pDC);
        // 下三角形
		pZBuffer->SetPoint(ScreenPoint[0], ScreenPoint[1], ScreenPoint[2]);
		pZBuffer->FillTriangle(pDC);
	}
}