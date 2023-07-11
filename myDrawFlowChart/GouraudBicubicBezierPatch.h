#if !defined(AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//投影类
#include "ColorFace.h"//平面片类
#include "ZBuffer.h"
#include "LightingScene.h"

/************************************************************************/
/* 这个类包含两个部分。一个是双三次贝塞尔曲面片的绘制。                 */
/* 这部分和CBicubicBezierPatch中的实现一样。                            */
/* 但是和CBicubicBezierPatch不同的是，我们在计算好了以后，              */
/* 不是直接进行绘制。而是保存起来。交给ZBuffer绘制。                    */
/* 因此上，我们引入了小面顶点和小面面表。                               */
/* 根据双三次贝塞尔曲面片的定义p(u, v)，其中u和v都是0到1的数字。        */
/* 我们把u和v分成10份。使用公式进行运算。得到了100个顶点。              */
/* 这100个顶点就是小面顶点。而这100个顶点构成了81个四边形的面，         */
/* 这就是小面面表。                                                     */
/* 因为ZBuffer绘制的时候，不是直接使用网格顶点绘制的。                  */
/* 而是按照三角形进行绘制的。                                           */
/* 另一个部分就是引入了光照模型。这个函数的入口函数是Illuminate函数。   */
/************************************************************************/
class CGouraudBicubicBezierPatch
{
public:
	CGouraudBicubicBezierPatch(void);
	virtual ~CGouraudBicubicBezierPatch(void);
	void ReadControlPoint(CColorP3 P[4][4]);			// 读入16个控制点
	void SetLightingScene(CLightingScene* pScene);		// 设置光照场景
	void SaveFacetData(void);							// 绘制网格点数据
	void Draw(CDC* pDC, CZBuffer* pZBuffer);			// 绘制曲面
private:
	void LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4]);	// 左乘顶点矩阵
	void RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4]);	// 右乘顶点矩阵
	void TransposeMatrix(double M[4][4]);						// 转置矩阵
public:
	CColorP3 m_objBezierControlPoint[4][4];			// 曲面片控制点的三维坐标(包含颜色)
	CColorP3 m_objBezierPatchPoint[100];			// 小面顶点数组10×10
													// 曲面片顶点的三维坐标(包含颜色)
	CColorFace m_objBezierPatchFace[81];			// 小面面表数组
	CProjection projection;					// 投影对象
	CLightingScene* pScene;					// 光照场景
};

#endif // !defined(AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
