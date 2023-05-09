// ConnectPoint.h: interface for the CConnectPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_)
#define AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
};

#endif // !defined(AFX_CONNECTPOINT_H__A77E1ADE_A0C8_495F_B106_8563A42EF899__INCLUDED_)
