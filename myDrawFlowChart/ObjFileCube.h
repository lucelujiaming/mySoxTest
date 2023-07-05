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
	void ReadNumber(void);//����Ԫ�ظ���
	void ReadVertex(void);//���붥��	
	void ReadTexture(void);//��������
	void ReadNormal(void);//���뷨ʸ��
	void ReadFace(void);//�������
	CP3* GetVertexName(void);//�õ�����������
	int GetVertexNumber(void);//�õ��������
	void Draw(CDC* pDC);//�����������߿�
private:
	CString FileName;
	CP3* V;//���
	CVector3* N;//��ʸ��
	CTextureCoordinate* T;//����
	CFace* F;//���
	int nTotalVertex;//�������
	int nTotalNormal;//��ʸ������
	int nTotalTexture;//�������
	int nTotalFace;//�������
	CProjection projection;//ͶӰ
};

#endif // !defined(AFX_OBJFILE_CUBE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
