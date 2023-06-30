#include "stdafx.h"
#include "ColorPoint2.h"

CColorPoint2::CColorPoint2(void)
{
	x = 0;
	y = 0;
	c = CRGB(0, 0, 0);
}

CColorPoint2::CColorPoint2(int x, int y)
{
	this->x = x;
	this->y = y;
	c = CRGB(0, 0, 0);
}

CColorPoint2::CColorPoint2(int x, int y, CRGB c)
{
	this->x = x;
	this->y = y;
	this->c = c;
}

CColorPoint2::~CColorPoint2(void)
{

}