#if !defined(AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorStack//链栈
{
public:
	CColorStack(void);
	virtual ~CColorStack(void);
	void Push(CPoint point);
	CPoint Pop(void);
public:
	CColorStack* pTop;//结点指针
	CPoint Pixel;//数据域
	CColorStack* pNext;//指针域
};

#endif // !defined(AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

