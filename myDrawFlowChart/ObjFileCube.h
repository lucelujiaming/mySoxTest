#if !defined(AFX_OBJFILE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_OBJFILE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include"P3.h"
#include "Face.h"
#include "Projection.h"
#include "Vector3.h"
#include "TextureCoordinate.h"

class CObjFileCube
{
public:
	CObjFileCube(void);
	virtual ~CObjFileCube(void);
	void ReadNumber(void);//读入元素个数
	void ReadVertex(void);//读入顶点	
	void ReadTexture(void);//读入纹理
	void ReadNormal(void);//读入法矢量
	void ReadFace(void);//读入表面
	CP3* GetVertexName(void);//得到顶点数组名
	int GetVertexNumber(void);//得到顶点个数
	void Draw(CDC* pDC);//绘制立方体线框
private:
	CString FileName;
	CP3* V;//点表
	CVector3* N;//法矢量
	CTextureCoordinate* T;//纹理
	CFace* F;//面表
	int nTotalVertex;//顶点个数
	int nTotalNormal;//法矢量个数
	int nTotalTexture;//纹理个数
	int nTotalFace;//表面个数
	CProjection projection;//投影
};

#endif // !defined(AFX_OBJFILE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
