#if !defined(AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 这个地方我没有修改，因为我对比了纹理映射和凹凸纹理映射。感觉应该可以用一个类。
#include "BumpTextureBiquatricBezierPatch.h"
#include "Patch.h"
#include "BumpTextureZBuffer.h"

class CBumpTextureSphere//球类
{
public:
	CBumpTextureSphere(void);
	virtual ~CBumpTextureSphere(void);
	void ReadVertex(void);//读入控制点表
	void ReadFace(void);//读入曲面片表
	void Draw(CDC* pDC, CBumpTextureZBuffer* pZBuffer);//绘制图形
public:
	CColorP3 Ver[26];//球面二次控制点
	CBumpTextureBiquatricBezierPatch bezier;//曲面片
private:
	CPatch Pat[8];//球面曲面片数	
};

#endif // !defined(AFX_BUMP_TEXTURE_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
