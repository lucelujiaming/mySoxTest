// ConnectPoint.h: interface for the CConnectPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_)
#define AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogFile.h"
// ����
#define CCONNECTPOINT_POSITIVE_X_MARGIN          (6)     // X��������
#define CCONNECTPOINT_NEGATIVE_X_MARGIN          (-6)    // X��������
#define CCONNECTPOINT_POSITIVE_Y_MARGIN          (6)     // Y��������
#define CCONNECTPOINT_NEGATIVE_Y_MARGIN          (-6)    // Y��������

#define CONNECTPOINT_POSITIVE_RADIUS	(3)	     // ��Ļ��ư뾶
#define CONNECTPOINT_NEGATIVE_RADIUS	(-3)	 // ��Ļ��ư뾶

#define CONNECTPOINT_COMMON             1         // ֻ�ܵ�����С���������ӡ�
#define CONNECTPOINT_CONNECT            2         // ���Ե�����С������

class CConnectPoint : public CObject
{
	DECLARE_SERIAL(CConnectPoint)

public:
	CConnectPoint();
	virtual ~CConnectPoint();

public:
	void Draw(CDC *pdc);
	void SetPoint(CPoint &pt);
	CPoint GetPoint();
	bool IsOn(CPoint &pt);

	void SetType(bool type);

	void Serialize(CArchive& ar);

private:
	CPoint m_Point;
	bool m_type;

	CLogFile m_objLogFile;
};

#endif // !defined(AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_)
