#if !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//����ɫ�Ķ�ά��������
#include "ColorVector3.h"

/************************************************************************/
// ��Ȼ������㷨����ռ������㷨����������ά�ռ��м�ס����������Ϣ�� */
// ������ͶӰ����άƽ�����һ��ȫ�ֵĻ������洢ÿ�����ض�Ӧ��Zֵ��    */
// ����ֻ���ƾ����������ı��档                                       */
// ��Ϊ�������洢ÿ�����ض�Ӧ��Zֵ�����Ը��㷨�ֱ���ΪZBuffer�㷨��     */
/************************************************************************/
// Ҳ����˵�����ǲ����������Ŀɼ��Խ��м�⣬                         */
/* ֻ���ݱ����ϸ������ص����ֵ��ȷ�����������ǵ���Ļ����ɫ��           */
// ����Ϊ���������ࡣ���ǻ���Ҫ�ο�CSmoothFillTriangle�ࡣ            */
// Ҳ�������ϵ��������ᵽ������������㷨��                             */
/* ��Ϊ���۶�ô���ӵ����壬���ն�����ʹ��������С��ƽ���               */
/* �������������ε����⣬�ͽ�������������ɫ�����⡣                 */
/* ����ϣ�������������㷨һ��������Ҳ��Ҫ������Span��ɨ����������   */
/************************************************************************/
/* ����ϣ�����Ļ��ƹ����������ġ�                                     */
/* ���ȵ���InitialDepthBufferΪZBuffer����ռ䡣                        */
/* ֮������һ��ֻ�ܻ���һ�������Ρ�                                   */
/* ����ϣ��������ȵ���SetPoint���������ε��������ꡣ                   */
/* ֮�����FillTriangle�����������Ρ�                                 */
// ��������̺�����������㷨��һ���ġ�                               */
//   1. �����ж����������λ����������Ρ�                                */
//   2. ֮��ʹ��DDA�㷨�����Span��������յ����顣                   */
//   3. ֮�����������������㷨�����ǾͿ���ֱ�ӿ����Ϣʹ�û�ͼ�ˡ�    */
//      ���Ƕ���ZBuffer�㷨��������Ҫ���㵱ǰ���ص�(x, y)�������ֵ��   */
//   4. ֮��Ѽ�������ĵ�ǰ���ص�(x, y)�������ֵ��                    */
//      ���Ѿ��洢����Ȼ������е�ԭ�ɼ����ص����ֵ��Աȡ�            */
//      �����ǰ����������С��֡��������ԭ���������ȣ�              */
//      ������ǰ���ظ������۲��� ���ڵ���ԭ���ء���                   */
//      4.1. ʹ�õ�ǰ���������ȸ�����Ȼ�������                       */
//      4.2. ����SetPixelV����ǰ���ص���ɫֵд��֡��������              */
/************************************************************************/
class CZBuffer // ZBuffer��
{
public:
    CZBuffer(void);
    virtual ~CZBuffer(void);
    void SetPoint(CColorP3 P0, 
                  CColorP3 P1, CColorP3 P2);              // �����㹹��������
    void InitialDepthBuffer(int nWidth, 
                  int nHeight, double zDepth);            // ��ʼ����Ȼ�����
    void FillTriangle(CDC* pDC);                          // ���������
private:
    void EdgeFlag(CColorPoint2 PStart, 
                  CColorPoint2 PEnd, BOOL bFeature);      // �߱���㷨
    void SortPoint(void);                                 // ��������
    CRGB Interp(double m, double m0, double m1, 
				CRGB c0, CRGB c1);					      // ��ɫ���Բ�ֵ
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
    CColorP3 P0, P1, P2;                    // �����εĸ�������
    CColorPoint3 point0, point1, point2;    // �����ε�������������
    CColorPoint2* SpanLeft;                 // ��ȵ���������־
    CColorPoint2* SpanRight;                // ��ȵ��յ������־
    int nIndex;                             // ɨ������������¼ɨ��������
    double** zBuffer;                       // ��Ȼ�����
    int nWidth, nHeight;                    // ��������Ⱥ͸߶�
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
