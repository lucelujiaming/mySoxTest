// DocumentGraph.h: interface for the CDocumentGraph class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Graph.h"
#include "LogFile.h"
/************************************************************************/
/* 通过观察Visio中带下方波浪线的文档图元发现：                          */
/* 当图元变长的时候，下方波浪线的弯曲程度不受影响。                     */
/* 但是当图元变得扁平的时候，下方波浪线如果仍然维持原有的弯曲程度，     */
/* 就会造成波浪线和图元上沿相交的尴尬状况。因此上，Visio做了处理。      */
/*                                                                      */
/* 使用Visio的导出功能，把这三个图元导出成svg矢量图。                   */
/* 通过阅读svg描述脚本可知。这个图元的下方波浪线是一个圆弧。            */
/*                                                                      */
/* 继续通过阅读分析svg描述脚本，总结发现：                              */
/* 1. 当图元不处于扁平状态的时候，图元宽度等于圆弧半径的2.4倍左右。     */
/*    因此上，高度为H，宽度为W的图元，圆弧高度的计算公式为：            */
/*          W/2.4 - sqrt((W/2.4) * (W/2.4) - (W/4) * (W/4))             */
/* 2. 当图元处于扁平状态的时候，如果还按照上面的算法。                  */
/*    圆弧高度就会大于图元高度。因此上，需要使用图元高度反算圆弧半径。  */
/*    对于高度为H，宽度为W的图元，为了计算圆弧半径R，                   */
/*    首先令圆弧高度为图元高度的一半。根据上面的计算，有：              */
/*          H/2 = ArcHeight = R - sqrt(R * R - (W/4) * (W/4))           */
/*     整理有：                                                         */
/*          R = H/4 + W * W/(16 * H)                                    */
/* 3. 为了判断图元是否偏平，首先计算默认圆弧高度。                      */
/*    之后判断这个圆弧高度是否大于图元高度的一半。                      */
/*    如果计算出来的默认圆弧高度大于高度的一半，则为扁平图元。          */
/*  这就是Draw的基本逻辑和下面这个2.4的来历。                           */
/************************************************************************/
#define  MAGIC_DOCUMENT_RADIUS_RATIO                 2.4
// 余量
#define CUSTOM_DOCUMENTGRAPH_TEXT_XBORDER            8    // X方向文字边框宽度
#define CUSTOM_DOCUMENTGRAPH_TEXT_YBORDER            8    // Y方向文字边框宽度


class CDocumentGraph : public CGraph  
{
	DECLARE_SERIAL(CDocumentGraph)

public:
	CDocumentGraph();
	virtual ~CDocumentGraph();
	CString GetTypeName() { return CString("CDocumentGraph"); }

public:
	void Draw( CDC *pdc );
	void DrawFocus( CDC *pdc );
	void Move( int cx, int cy );
	void AdjustSize( CPoint &pt );

	void Serialize(CArchive& ar);

	bool IsIn( CPoint &pt );
	bool IsOn( CPoint &pt );
	int  IsConnectOn(CAdjustPoint *pt);

	int GetAdjustPoint();

private:
	void AdjustStartAndEnd();
	void AdjustFocusPoint();

private:
	int m_AdjustPoint;		// 被修改的调整点标号。

	//CObArray m_Points;
	CLogFile m_objLogFile;
};

#endif // !defined(AFX_DOCUMENTGRAPH_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
