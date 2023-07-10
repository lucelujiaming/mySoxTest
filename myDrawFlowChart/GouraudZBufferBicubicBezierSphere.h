#if !defined(AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GouraudBicubicBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

// ʹ��Gouraud���������㷨���Ƶ�˫����Bezier��
// ˫����Bezier��ͨ������8Ƭ����Ƭ��ɵġ�
class CGouraudZBufferBicubicBezierSphere // ����
{
public:
	CGouraudZBufferBicubicBezierSphere(void);
	virtual ~CGouraudZBufferBicubicBezierSphere(void);
	void ReadVertex(void);						// ������Ƶ��
	void ReadFace(void);						// ��������Ƭ��
	void Draw(CDC* pDC, CZBuffer* pZBuffer);	// ����ͼ��
public:
	CColorP3 Ver[62];							// ������Ƶ�
	CGouraudBicubicBezierPatch bezier;			// ����Ƭ
private:
	CPatch Pat[8];								// ��������Ƭ��	
};

#endif // !defined(AFX_GOURAUD_ZBUFFER_BICUBICBEZIER_SPHERE_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
