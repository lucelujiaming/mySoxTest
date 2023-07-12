#if !defined(AFX_PHONG_BIQUATRICBEZIER_PATCH_H_)
#define AFX_PHONG_BIQUATRICBEZIER_PATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//投影类
#include "ColorFace.h"//平面片类
#include "Vector3ZBuffer.h"
#include "LightingScene.h"

/************************************************************************/
/* 这个类包含两个部分。一个是有理双二次Bezier曲面片的绘制。             */
/* 这部分和CRationalBiquatricBezierPatch中的实现一样。                  */
/* 但是和CRationalBiquatricBezierPatch不同的是，我们在计算好了以后，    */
/* 不是直接进行绘制。而是保存起来。交给ZBuffer绘制。                    */
/* 因此上，我们引入了小面顶点和小面面表。                               */
/* 根据有理双二次Bezier曲面片的定义p(u, v)，其中u和v都是0到1的数字。    */
/* 我们把u和v分成10份。使用公式进行运算。得到了100个顶点。              */
/* 这100个顶点就是小面顶点。而这100个顶点构成了81个四边形的面，         */
/* 这就是小面面表。                                                     */
/* 因为ZBuffer绘制的时候，不是直接使用网格顶点绘制的。                  */
/* 而是按照三角形进行绘制的。                                           */
/* 另一个部分就是引入了光照模型。这个函数的入口函数是Illuminate函数。   */
/* 还有就是这个类使用的是CVector3ZBuffer进行绘制。在CVector3ZBuffer中   */
/* 使用了Phong明暗处理算法。                                            */
/************************************************************************/
class CPhongBiquatricBezierPatch//有理双二次Bezier曲面片类
{
public:
	CPhongBiquatricBezierPatch(void);
	virtual ~CPhongBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//读入9个控制点
	void ReadWeight(double W[3][3]);//读入9个权因子
	void SetLightingScene(CLightingScene* pScene);//设置光照场景
	void SaveFacetData(void);//绘制双二次Bezier曲面片
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//绘制曲面
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//左乘顶点矩阵
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//右乘顶点矩阵
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//左乘权因子矩阵
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//右乘权因子矩阵
	void TransposeMatrix(double M[3][3]);//转置矩阵
public:
	CColorP3 P[3][3];//三维控制点
	CColorP3 V[100];//小面顶点数组10×10
	CColorFace F[81];//小面面表数组
	double W[3][3];//权因子
	CProjection projection;//投影
	CLightingScene* pScene;//光照
};

#endif // !defined(AFX_PHONG_BIQUATRICBEZIER_PATCH_H_)
