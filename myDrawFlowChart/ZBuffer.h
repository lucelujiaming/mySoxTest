#if !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ColorP3.h"
#include "ColorPoint3.h"//带颜色的二维整数点类
#include "ColorVector3.h"

/************************************************************************/
/* 要绘制真实感图形，需要建立物体的表面模型并消隐。                     */
/* 从视点的角度解决表面的遮挡问题，离视点近的表面遮挡了离视点远的表面， */
/* 屏幕上绘制的结果为所有可见表面投影的集合。                           */
/* 常用算法有两种，这两种算法都考察了物体表面的深度坐标。               */
/* 一种是深度缓冲器算法，另一种是深度排序算法，又称画家算法。           */
/* Catmull于1974年提出了深度缓冲器算法。                                */
/************************************************************************/
/* 深度缓冲器算法是像空间消隐算法，首先在三维空间中记住物体的深度信息， */
/* 将物体投影到二维平面后，用一个全局的缓冲器存储每个像素对应的Z值，    */
/* 这样只绘制距离相机最近的表面。                                       */
/* 因为缓冲器存储每个像素对应的Z值，所以该算法又被称为ZBuffer算法。     */
/************************************************************************/
/* Z-Buffer算法需要建立两个缓冲器：                                     */
/*   一个是指定大小的深度缓冲器(zBuffer)，初始化为最大深度值。          */
/*   另一个是同样大小的帧缓冲器(fBuffer)，初始化为背景色。              */
/* 原理：                                                               */
/*   1. Z-Buffer算法计算准备写入帧缓冲器的当前像素的深度值，            */
/*      并与已经存储在深度缓冲器中的原可见像素的深度值进行比较。        */
/*   2. 如果当前像素的深度值小于原可见像素的深度值，                    */
/*      表明当前像素更靠近观察者且遮挡了原像素，                        */
/*      则将当前像素的颜色值写入帧缓冲器，                              */
/*   3. 同时用当前像素的深度值更新深度缓冲器。                          */
/*   4. 否则，不作更改。                                                */
/* 因此上，Buffer算法的实质就是对于一个给定视线上的(x, y)，             */
/* 查找距离视点最近的z(x, y)值。                                        */
/************************************************************************/
/* 2.算法设计                                                           */
/*   (1) 设置帧缓冲器(fBuffer)初始值为背景色。                          */
/*   (2) 确定深度缓冲器(zBuffer)的宽度、高度和初始深度。                */
/*       一般将初始深度置为最大深度值。                                 */
/*   (3) 对于多边形表面中的每一像素(x,y), 计算其深度值z(x, y)。         */
/*   (4) 将z与存储在深度缓冲器中的(x,y)处的深度值zBuffer(x, y)做比较。  */
/*   (5) 如果z(x,y)<=zBuffer(x,y),                                      */
/*       则将此像素的颜色写入帧缓冲器fBuffer(x, y)。                    */
/*       且用z(x, y)重置zBuffer(x, y)。                                 */
/************************************************************************/
/* 也就是说，我们不对物体表面的可见性进行检测，                         */
/* 只根据表面上各个像素的深度值，确定表面所覆盖的屏幕的颜色。           */
/* 另外为了完成这个类。我们还需要参考CSmoothFillTriangle类。            */
/* 也就是书上第三章中提到的三角形填充算法。                             */
/* 因为无论多么复杂的物体，最终都可以使用三角形小面逼近。               */
/* 解决了填充三角形的问题，就解决了物体表面着色的问题。                 */
/* 因此上，和三角形填充算法一样，我们也需要定义跨度Span和扫描线索引。   */
/************************************************************************/
/* 因为在绘制多个物体时，我们会使用同一个深度缓冲器。                   */
/* 而且深度缓冲器其实并不知道自己要绘制的是什么物体。                   */
/* 因此，深度缓冲器只需实现根据三角形的坐标信息和深度信息填充三角形。   */
/************************************************************************/
/* 因此上，这个的绘制过程是这样的。                                     */
/* 首先调用InitialDepthBuffer为ZBuffer分配空间。                        */
/* 之后，我们一次只能绘制一个三角形。                                   */
/* 因此上，我们首先调用SetPoint传入三角形的三个坐标。                   */
/* 之后调用FillTriangle填充这个三角形。                                 */
/* 这个填充过程和三角形填充算法是一样的。                               */
/*   1. 首先判断是左三角形还是右三角形。                                */
/*   2. 之后使用DDA算法线性插值填充跨度Span的起点与终点数组。           */
/*   3. 之后如果是三角形填充算法。我们就可以直接跨度信息使用绘图了。    */
/*      但是对于ZBuffer算法，我们需要计算当前像素点(x, y)处的深度值。   */
/*   4. 之后把计算出来的当前像素点(x, y)处的深度值，                    */
/*      与已经存储在深度缓冲器中的原可见像素的深度值相对比。            */
/*      如果当前采样点的深度小于帧缓冲器中原采样点的深度，              */
/*      表明当前像素更靠近观察者 并遮挡了原像素。则：                   */
/*      4.1. 使用当前采样点的深度更新深度缓冲器。                       */
/*      4.2. 调用SetPixelV将当前像素的颜色值写入帧缓冲器。              */
/* 其中深度缓冲器就是double** zBuffer。而帧缓冲器不需要实现。           */
/* 只需要调用pDC->SetPixelV写当前像素颜色值即可。                       */
/************************************************************************/
/* 另外这个类除了在EdgeFlag函数中实现了线性插值来填充跨度Span以外，     */
/* 还在Interp函数中实现了双线性差值。 用于设置三角形的绘制颜色。        */
/************************************************************************/
class CZBuffer // ZBuffer类
{
public:
    CZBuffer(void);
    virtual ~CZBuffer(void);
    void SetPoint(CColorP3 P0, 
                  CColorP3 P1, CColorP3 P2);              // 三顶点构造三角形
    void InitialDepthBuffer(int nWidth, 
                  int nHeight, double zDepth);            // 初始化深度缓冲器
    void FillTriangle(CDC* pDC);                          // 填充三角形
private:
    void EdgeFlag(CColorPoint2 PStart, 
                  CColorPoint2 PEnd, BOOL bFeature);      // 边标记算法
    void SortPoint(void);                                 // 顶点排序
    CRGB Interp(double m, double m0, double m1, 
				CRGB c0, CRGB c1);					      // 颜色线性插值
	CVector3 Interp(double m, double m0, double m1, 
				CVector3 N0, CVector3 N1);				  // 法矢量线性插值。未使用。
	double Interp(double m, double m0, double m1, 
				double z0, double z1);                    // 深度线性插值。未使用。
public:
	void SetDrawPosition(CPoint ptStart) { m_ptDrawPosition = ptStart; }
private:
    CColorP3 P0, P1, P2;                    // 三角形的浮点坐标
    CColorPoint3 point0, point1, point2;    // 三角形的整数顶点坐标
    CColorPoint2* SpanLeft;                 // 跨度的起点数组标志
    CColorPoint2* SpanRight;                // 跨度的终点数组标志
    int nIndex;                             // 扫描线索引。记录扫描线条数
    double** zBuffer;                       // 深度缓冲区
    int nWidth, nHeight;                    // 缓冲区宽度和高度
private:
	CPoint m_ptDrawPosition;
};

#endif // !defined(AFX_ZBUFFER_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
