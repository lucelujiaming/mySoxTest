#include "stdafx.h"
#include "ColorStack.h"


CColorStack::CColorStack(void)
{

}

CColorStack::~CColorStack(void)
{

}

void CColorStack::Push(CPoint point)//��ջ����
{
	pTop = new CColorStack;
	pTop->Pixel = point;
	pTop->pNext = this->pNext;
	this->pNext = pTop;
}

CPoint CColorStack::Pop(void)//��ջ����
{
	CPoint point;
	if (this->pNext != NULL)
	{
		pTop = this->pNext;
		this->pNext = pTop->pNext;
		point = pTop->Pixel;
		delete pTop;
	}
	return point;
}