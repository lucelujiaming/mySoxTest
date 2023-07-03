#include "stdafx.h"
#include "BackfaceCullCube.h"
#define ROUND(d) int(d + 0.5)//四舍五入的宏定义

CBackfaceCullCube::CBackfaceCullCube(void)
{

}

CBackfaceCullCube::~CBackfaceCullCube(void)
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
void CBackfaceCullCube::ReadVertex(void)//点表
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

// 根据上面的分析可知，X轴向右，Y轴向上，Z轴指向我们。因此可知：
void CBackfaceCullCube::ReadFace(void) // 面表
{
	// 朝向我们的立方体表面由4,5,6,7四个点组成。
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7; // 前面
	F[1].ptNumber = 4;
	// 背对我们的立方体表面由0,3,2,1四个点组成。
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1; // 后面
	F[2].ptNumber = 4;
	// 立方体的左侧表面由0,4,7,3四个点组成。
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

// 有了上面两个函数的基础，我们就可以绘制立方体了。
void CBackfaceCullCube::Draw(CDC* pDC)
{
	CP2 ScreenPoint[4]; // 二维投影点
	// 获得透视投影的视点。
	CP3 Eye = projection.GetEye();
	// 虽然我们现在绘制的立方体是最简单的三维物体。
	// 但是我们也需要按照绘制复杂三维物体的步骤进行绘制。
    // 也就是一个面一个面绘制。
	// 面循环：也就是一个面一个面绘制。
	for (int nFace = 0; nFace < 6; nFace++)//面循环
	{
		CVector3 ViewVector(V[F[nFace].ptIndex[0]], Eye);//面的视矢量
		ViewVector = ViewVector.Normalize();//归一化视矢量
		CVector3 Vector01(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[1]]);//面的一个边矢量
		CVector3 Vector02(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[2]]);//面的另一个边矢量
		CVector3 FaceNormal = CrossProduct(Vector01, Vector02);//面的法矢量
		FaceNormal = FaceNormal.Normalize();//归一化法矢量
		if (DotProduct(ViewVector, FaceNormal) >= 0)//背面剔除
		{
		    // 得到这个面所有顶点的坐标。
			for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // 顶点循环
			{
			    // 我们这里使用透视投影计算投影点。
				ScreenPoint[nPoint] = projection.PerspectiveProjection(V[F[nFace].ptIndex[nPoint]]); // 透视投影
		    }
			// 所谓的绘制一个面。其实就是使用MoveTo和LineTo绘制一个多边形。
			// 绘制多边形
			pDC->MoveTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));
			pDC->LineTo(ROUND(ScreenPoint[1].x), ROUND(ScreenPoint[1].y));
			pDC->LineTo(ROUND(ScreenPoint[2].x), ROUND(ScreenPoint[2].y));
			pDC->LineTo(ROUND(ScreenPoint[3].x), ROUND(ScreenPoint[3].y));
			pDC->LineTo(ROUND(ScreenPoint[0].x), ROUND(ScreenPoint[0].y));			
		}
	}
}