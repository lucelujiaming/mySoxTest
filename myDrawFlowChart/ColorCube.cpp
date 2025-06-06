#include "stdafx.h"
#include "ColorCube.h"
#define ROUND(d) int(d + 0.5)//四舍五入的宏定义

CColorCube::CColorCube(void)
{
    m_xScale = m_yScale = m_zScale = 1;
}

CColorCube::~CColorCube(void)
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
/* 这个函数之前给出的是长宽高都为1的单位立方体。我加入了长宽高。        */
/* 使代码看起来更加清晰一点。否则全是1这种魔法数字。更加难以理解。      */
/************************************************************************/
void CColorCube::ReadVertex(int xScale, int yScale, int zScale) // 顶点表
{
	m_xScale = xScale;
	m_yScale = yScale;
	m_zScale = zScale;
	V[0].x = 0;      V[0].y = 0;      V[0].z = 0;
	V[1].x = xScale, V[1].y = 0;      V[1].z = 0;
	V[2].x = xScale, V[2].y = yScale; V[2].z = 0;
	V[3].x = 0;      V[3].y = yScale; V[3].z = 0;
	V[4].x = 0;      V[4].y = 0;      V[4].z = zScale;
	V[5].x = xScale; V[5].y = 0;      V[5].z = zScale;
	V[6].x = xScale; V[6].y = yScale; V[6].z = zScale;
	V[7].x = 0;      V[7].y = yScale; V[7].z = zScale;
}

// 根据上面的分析可知，X轴向右，Y轴向上，Z轴指向我们。因此可知：
void CColorCube::ReadFace(void) // 面表
{
	// 朝向我们的立方体表面由4,5,6,7四个点组成。
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7; // 前面
	// 背对我们的立方体表面由0,3,2,1四个点组成。
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1; // 后面
	// 立方体的左侧表面由0,4,7,3四个点组成。
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3; // 左面
	// 立方体的右侧表面由1,2,6,5四个点组成。
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5; // 右面
	// 立方体的顶端表面由2,3,7,6四个点组成。
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6; // 顶面
	// 立方体的底端表面由0,1,5,4四个点组成。
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4; // 底面
}
