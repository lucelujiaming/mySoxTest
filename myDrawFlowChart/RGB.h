#if !defined(AFX_RGB_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)
#define AFX_RGB_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRGB
{
public:
	CRGB(void);
	CRGB(double red, double green, double blue);
	virtual ~CRGB(void);	
	friend CRGB operator + (const CRGB &clr0, const CRGB &clr1);//���������
	friend CRGB operator - (const CRGB &clr0, const CRGB &clr1);
	friend CRGB operator * (const CRGB &clr0, const CRGB &clr1);
	friend CRGB operator * (const CRGB &clr, double scalar);
	friend CRGB operator * (double scalar, const CRGB &clr);
	friend CRGB operator / (const CRGB &clr, double scalar);
	void Normalize();//�淶����[0,1]����
public:
	double red;//��ɫ����
	double green;//��ɫ����
	double blue;//��ɫ����
};

#endif // !defined(AFX_RGB_RECTANGLE_H__76897764_98F1_4E75_89FD_8F2A42BC6344__INCLUDED_)


