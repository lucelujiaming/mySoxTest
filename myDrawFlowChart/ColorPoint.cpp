#include "stdafx.h"
#include "ColorPoint.h"

CColorPoint::CColorPoint(void)
{
	x = 0;
	y = 0;
	c = CRGB(0, 0, 0);
}

CColorPoint::CColorPoint(int x, int y)
{
	this->x = x;
	this->y = y;
	c = CRGB(0, 0, 0);
}

CColorPoint::CColorPoint(int x, int y, CRGB c)
{
	this->x = x;
	this->y = y;
	this->c = c;
}

CColorPoint::~CColorPoint(void)
{

}