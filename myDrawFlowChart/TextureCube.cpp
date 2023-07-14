
#include "stdafx.h"
#include "TextureCube.h"

CTextureCube::CTextureCube(void)
{

}

CTextureCube::~CTextureCube(void)
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
void CTextureCube::ReadVertex(void)//点表
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
/* 根据上面的分析可知，X轴向右，Y轴向上，Z轴指向我们。                  */
/* 这个函数除了给出了6个面的顶点以外，还给出了6个面贴图坐标。           */
/* 也就是将位图的uv贴图坐标绑定到立方体6个面的表面表中。                */
/************************************************************************/
/* 下面来看如何生成uv贴图坐标。首先为了讨论方便，我们首先进行归一化，   */
/* 也就是假设位图是一个长宽都为1的图片。                                */
/* 之后把立方体展开。一个立方体展开有很多种方法。我们常用的是这种：     */
/*              +--------+                                              */
/*              |        |                                              */
/*              |   顶   |                                              */
/*              |        |                                              */
/*     +--------+--------+--------+--------+                            */
/*     |        |        |        |        |                            */
/*     |  左    |   前   |   右   |   后   |                            */
/*     |        |        |        |        |                            */
/*     +--------+--------+--------+--------+                            */
/*              |        |                                              */
/*              |   底   |                                              */
/*              |        |                                              */
/*             -+--------+                                              */
/* 把这种展开方式放在如下长宽都为1的位图上。                            */
/* 可以看到，在X方向为四等分，在Y方向为三等分。                         */
/* 因此上uv贴图坐标就很容易得到了。我们在位图上标记如下：               */
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
/* 这就是下面纹理地址中这些数字的来源。                                 */
/************************************************************************/
void CTextureCube::ReadFace(void)//面表
{
    // 根据上面的分析可知：
	// 朝向我们的立方体表面由4,5,6,7四个点组成。
	F[0].ptNumber = 4;
	F[0].ptIndex[0] = 4, F[0].ptIndex[1] = 5;
	F[0].ptIndex[2] = 6, F[0].ptIndex[3] = 7;  // 前面
	F[0].t[0] = CTextureCoordinate(0.25, 0.33);
	F[0].t[1] = CTextureCoordinate(0.5, 0.33);
	F[0].t[2] = CTextureCoordinate(0.5, 0.66);
	F[0].t[3] = CTextureCoordinate(0.25, 0.66); // 前面纹理地址
	// 背对我们的立方体表面由0,3,2,1四个点组成。
	F[1].ptNumber = 4;
	F[1].ptIndex[0] = 0, F[1].ptIndex[1] = 3;
	F[1].ptIndex[2] = 2, F[1].ptIndex[3] = 1;  // 后面
	F[1].t[0] = CTextureCoordinate(1, 0.33);
	F[1].t[1] = CTextureCoordinate(1, 0.66);
	F[1].t[2] = CTextureCoordinate(0.75, 0.66);
	F[1].t[3] = CTextureCoordinate(0.75, 0.33); // 后面纹理地址
	// 立方体的左侧表面由0,4,7,3四个点组成。
	F[2].ptNumber = 4;
	F[2].ptIndex[0] = 0, F[2].ptIndex[1] = 4;
	F[2].ptIndex[2] = 7, F[2].ptIndex[3] = 3;  // 左面
	F[2].t[0] = CTextureCoordinate(0, 0.33);
	F[2].t[1] = CTextureCoordinate(0.25, 0.33);
	F[2].t[2] = CTextureCoordinate(0.25, 0.66);
	F[2].t[3] = CTextureCoordinate(0, 0.66);    // 左面纹理地址
	// 立方体的右侧表面由1,2,6,5四个点组成。
	F[3].ptNumber = 4;
	F[3].ptIndex[0] = 1, F[3].ptIndex[1] = 2;
	F[3].ptIndex[2] = 6, F[3].ptIndex[3] = 5;  // 右面
	F[3].t[0] = CTextureCoordinate(0.75, 0.33);
	F[3].t[1] = CTextureCoordinate(0.75, 0.66);
	F[3].t[2] = CTextureCoordinate(0.5, 0.66);
	F[3].t[3] = CTextureCoordinate(0.5, 0.33); // 右面纹理地址
	// 立方体的顶端表面由2,3,7,6四个点组成。
	F[4].ptNumber = 4;
	F[4].ptIndex[0] = 2, F[4].ptIndex[1] = 3;
	F[4].ptIndex[2] = 7, F[4].ptIndex[3] = 6;   // 顶面
	F[4].t[0] = CTextureCoordinate(0.5, 1);
	F[4].t[1] = CTextureCoordinate(0.25, 1);
	F[4].t[2] = CTextureCoordinate(0.25, 0.66);
	F[4].t[3] = CTextureCoordinate(0.5, 0.66);//顶面纹理地址
	// 立方体的底端表面由0,1,5,4四个点组成。
	F[5].ptNumber = 4;
	F[5].ptIndex[0] = 0, F[5].ptIndex[1] = 1;
	F[5].ptIndex[2] = 5, F[5].ptIndex[3] = 4;   // 底面
	F[5].t[0] = CTextureCoordinate(0.25, 0);
	F[5].t[1] = CTextureCoordinate(0.5, 0);
	F[5].t[2] = CTextureCoordinate(0.5, 0.33);
	F[5].t[3] = CTextureCoordinate(0.25, 0.33);	// 底面纹理地址
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
/* 有了上面两个函数的基础，我们就可以绘制立方体了。                     */
/* CZBufferCube使用传入的CZBuffer进行绘制。                             */
/* 因此上这个函数是一个核心逻辑函数。                                   */
// 传入三个参数：                                                       */
// pDC      - 绘图句柄。                                                */
// pZBuffer - 深度缓冲器。我们使用同一个深度缓冲器绘制多个物体。        */
//            作为深度缓冲器，它并不知道自己要绘制的是什么。            */
//            他就会根据深度信息填充三角形。                            */
/************************************************************************/
void CTextureCube::Draw(CDC* pDC, CTextureZBuffer* pZBuffer)
{
	CColorP3 ScreenPoints[4];						// 每一个面的三维投影点
	CColorP3 Eye = projection.GetColorEye();		// 视点
	CVector3 vectorVertexsOfFace[4];				// 每一个面的四个顶点的法矢量
	CTextureCoordinate objCoordinateOfFace[4];		// 每一个面的四个顶点的纹理坐标
	// 虽然我们现在绘制的立方体是最简单的三维物体。
	// 但是我们也需要按照绘制复杂三维物体的步骤进行绘制。
    // 也就是一个面一个面绘制。
	// 面循环：也就是一个面一个面绘制。
	for (int nFace = 0; nFace < 6; nFace++)
	{
		// 一个面包括四个顶点。求第0个顶点到第1个顶点的这条边的矢量
		CVector3 Vector01(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[1]]);
		// 求第0个顶点到第2个顶点的另一个边的矢量
		CVector3 Vector02(V[F[nFace].ptIndex[0]], V[F[nFace].ptIndex[2]]);
		// 求上面两条边的叉乘，得到面的法矢量
		CVector3 FaceNormal = CrossProduct(Vector01, Vector02);
		// 归一化法矢量
		FaceNormal = FaceNormal.Normalize();
		// 得到这个面所有顶点的坐标。
		for (int nPoint = 0; nPoint < F[nFace].ptNumber; nPoint++) // 顶点循环
		{
			// 使用三维透视投影得到透视坐标。
			ScreenPoints[nPoint] = projection.ThreeDimColorPerspectiveProjection(V[F[nFace].ptIndex[nPoint]]);
			// 每个面的四个顶点法矢量都设定为这个面的法矢量。
			vectorVertexsOfFace[nPoint] = FaceNormal;
			// 为了进行纹理映射，我们需要将u坐标乘以位图的宽度。
			objCoordinateOfFace[nPoint].u = F[nFace].t[nPoint].u * (pTexture->bmp.bmWidth - 1);
			// 将v坐标乘以位图的高度。
			objCoordinateOfFace[nPoint].v = F[nFace].t[nPoint].v * (pTexture->bmp.bmHeight - 1);
		}
		// 立方体的每一个面都是一个矩形，我们把他分成两个三角形进行显示。
		// 准备绘制左上三角形，为左上三角形生成顶点P
		CColorP3 objTopLeftPoint[3] = { ScreenPoints[0] ,ScreenPoints[2] ,ScreenPoints[3] };
		// 为左上三角形生成法矢量N
		CVector3 objTopLeftNormal[3] = 
				{ vectorVertexsOfFace[0], vectorVertexsOfFace[2], vectorVertexsOfFace[3] };
		// 为左上三角形生成纹理地址T
		CTextureCoordinate objTopLeftTexture[3] = 
				{ objCoordinateOfFace[0], objCoordinateOfFace[2], objCoordinateOfFace[3] };
		// 为左上三角形传入顶点P，法矢量N和纹理地址T
		pZBuffer->SetPoint(objTopLeftPoint, objTopLeftNormal, objTopLeftTexture);
		// 使用视点场景和纹理绘制左上三角形
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
		
		// 准备绘制右下三角形，为右下三角形生成顶点P
		CColorP3 objDownRightPoint[3] = { ScreenPoints[0] ,ScreenPoints[1] ,ScreenPoints[2] };
		// 为右下三角形生成法矢量N
		CVector3 objDownRightNormal[3] = { vectorVertexsOfFace[0], vectorVertexsOfFace[1], vectorVertexsOfFace[2] };
		// 为右下三角形生成纹理地址T
		CTextureCoordinate objDownRightTexture[3] = { objCoordinateOfFace[0], objCoordinateOfFace[1], objCoordinateOfFace[2] };
		// 为右下三角形传入顶点P，法矢量N和纹理地址T
		pZBuffer->SetPoint(objDownRightPoint, objDownRightNormal, objDownRightTexture);
		// 使用视点场景和纹理绘制右下三角形
		pZBuffer->FillTriangle(pDC, Eye, pScene, pTexture);
	}
}