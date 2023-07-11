#if !defined(AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//ͶӰ��
#include "ColorFace.h"//ƽ��Ƭ��
#include "ZBuffer.h"
#include "LightingScene.h"

/************************************************************************/
/* ���������������֡�һ����˫���α���������Ƭ�Ļ��ơ�                 */
/* �ⲿ�ֺ�CBicubicBezierPatch�е�ʵ��һ����                            */
/* ���Ǻ�CBicubicBezierPatch��ͬ���ǣ������ڼ�������Ժ�              */
/* ����ֱ�ӽ��л��ơ����Ǳ�������������ZBuffer���ơ�                    */
/* ����ϣ�����������С�涥���С�����                               */
/* ����˫���α���������Ƭ�Ķ���p(u, v)������u��v����0��1�����֡�        */
/* ���ǰ�u��v�ֳ�10�ݡ�ʹ�ù�ʽ�������㡣�õ���100�����㡣              */
/* ��100���������С�涥�㡣����100�����㹹����81���ı��ε��棬         */
/* �����С�����                                                     */
/* ��ΪZBuffer���Ƶ�ʱ�򣬲���ֱ��ʹ�����񶥵���Ƶġ�                  */
/* ���ǰ��������ν��л��Ƶġ�                                           */
/* ��һ�����־��������˹���ģ�͡������������ں�����Illuminate������   */
/************************************************************************/
class CGouraudBicubicBezierPatch
{
public:
	CGouraudBicubicBezierPatch(void);
	virtual ~CGouraudBicubicBezierPatch(void);
	void ReadControlPoint(CColorP3 P[4][4]);			// ����16�����Ƶ�
	void SetLightingScene(CLightingScene* pScene);		// ���ù��ճ���
	void SaveFacetData(void);							// �������������
	void Draw(CDC* pDC, CZBuffer* pZBuffer);			// ��������
private:
	void LeftMultiplyMatrix(double M[4][4],CColorP3 P[4][4]);	// ��˶������
	void RightMultiplyMatrix(CColorP3 P[4][4],double M[4][4]);	// �ҳ˶������
	void TransposeMatrix(double M[4][4]);						// ת�þ���
public:
	CColorP3 m_objBezierControlPoint[4][4];			// ����Ƭ���Ƶ����ά����(������ɫ)
	CColorP3 m_objBezierPatchPoint[100];			// С�涥������10��10
													// ����Ƭ�������ά����(������ɫ)
	CColorFace m_objBezierPatchFace[81];			// С���������
	CProjection projection;					// ͶӰ����
	CLightingScene* pScene;					// ���ճ���
};

#endif // !defined(AFX_GOURAUD_BICUBIC_BEZIER_PATCH_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
