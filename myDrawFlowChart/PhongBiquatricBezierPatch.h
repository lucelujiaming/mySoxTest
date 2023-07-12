#if !defined(AFX_PHONG_BIQUATRICBEZIER_PATCH_H_)
#define AFX_PHONG_BIQUATRICBEZIER_PATCH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Projection.h"//ͶӰ��
#include "ColorFace.h"//ƽ��Ƭ��
#include "Vector3ZBuffer.h"
#include "LightingScene.h"

/************************************************************************/
/* ���������������֡�һ��������˫����Bezier����Ƭ�Ļ��ơ�             */
/* �ⲿ�ֺ�CRationalBiquatricBezierPatch�е�ʵ��һ����                  */
/* ���Ǻ�CRationalBiquatricBezierPatch��ͬ���ǣ������ڼ�������Ժ�    */
/* ����ֱ�ӽ��л��ơ����Ǳ�������������ZBuffer���ơ�                    */
/* ����ϣ�����������С�涥���С�����                               */
/* ��������˫����Bezier����Ƭ�Ķ���p(u, v)������u��v����0��1�����֡�    */
/* ���ǰ�u��v�ֳ�10�ݡ�ʹ�ù�ʽ�������㡣�õ���100�����㡣              */
/* ��100���������С�涥�㡣����100�����㹹����81���ı��ε��棬         */
/* �����С�����                                                     */
/* ��ΪZBuffer���Ƶ�ʱ�򣬲���ֱ��ʹ�����񶥵���Ƶġ�                  */
/* ���ǰ��������ν��л��Ƶġ�                                           */
/* ��һ�����־��������˹���ģ�͡������������ں�����Illuminate������   */
/* ���о��������ʹ�õ���CVector3ZBuffer���л��ơ���CVector3ZBuffer��   */
/* ʹ����Phong���������㷨��                                            */
/************************************************************************/
class CPhongBiquatricBezierPatch//����˫����Bezier����Ƭ��
{
public:
	CPhongBiquatricBezierPatch(void);
	virtual ~CPhongBiquatricBezierPatch(void);
	void ReadControlPoint(CColorP3 P[3][3]);//����9�����Ƶ�
	void ReadWeight(double W[3][3]);//����9��Ȩ����
	void SetLightingScene(CLightingScene* pScene);//���ù��ճ���
	void SaveFacetData(void);//����˫����Bezier����Ƭ
	void Draw(CDC* pDC, CVector3ZBuffer* pZBuffer);//��������
private:
	void LeftMultiplyMatrix(double M[3][3],CColorP3 P[3][3]);//��˶������
	void RightMultiplyMatrix(CColorP3 P[3][3],double M[3][3]);//�ҳ˶������
	void LeftMultiplyMatrix(double M[3][3],double W[3][3]);//���Ȩ���Ӿ���
	void RightMultiplyMatrix(double W[3][3],double M[3][3]);//�ҳ�Ȩ���Ӿ���
	void TransposeMatrix(double M[3][3]);//ת�þ���
public:
	CColorP3 P[3][3];//��ά���Ƶ�
	CColorP3 V[100];//С�涥������10��10
	CColorFace F[81];//С���������
	double W[3][3];//Ȩ����
	CProjection projection;//ͶӰ
	CLightingScene* pScene;//����
};

#endif // !defined(AFX_PHONG_BIQUATRICBEZIER_PATCH_H_)
