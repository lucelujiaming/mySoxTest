#if !defined(AFX_GOURAUD_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_)
#define AFX_GOURAUD_ZBUFFER_BIQUATRIC_BEZIER_SPHERE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GouraudBiquatricBezierPatch.h"
#include "Patch.h"
#include "ZBuffer.h"

// ʹ��Gouraud���������㷨���Ƶ�����˫����Bezier��
// ˫����Bezier��ͨ������8Ƭ����Ƭ��ɵġ�
class CGouraudZBufferBiquatricBezierSphere // ����
{
public:
	CGouraudZBufferBiquatricBezierSphere(void);
	virtual ~CGouraudZBufferBiquatricBezierSphere(void);
	void ReadVertex(void);              // ������Ƶ��
	void ReadFace(void);                // ��������Ƭ��
	void Draw(CDC* pDC, CZBuffer* pZBuffer); // ����ͼ��
public:
	CColorP3 Ver[26];//������ο��Ƶ�
	CGouraudBiquatricBezierPatch bezier;//����Ƭ
private:
	CPatch Pat[8]; // ��������Ƭ��
};

#endif // !defined(AFX_GOURAUD_ZBUFFER_RATIONALBIQUATRIC_BEZIER_SPHERE_H_)
