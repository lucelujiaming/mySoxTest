#if !defined(AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)
#define AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColorStack//��ջ
{
public:
	CColorStack(void);
	virtual ~CColorStack(void);
	void Push(CPoint point);
	CPoint Pop(void);
public:
	CColorStack* pTop;//���ָ��
	CPoint Pixel;//������
	CColorStack* pNext;//ָ����
};

#endif // !defined(AFX_COLORSTACK_H__6BBCFA39_2B2E_45D8_B2B4_6C9464FB23C9__INCLUDED_)

