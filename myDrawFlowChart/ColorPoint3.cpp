#include "stdafx.h"
#include "ColorPoint3.h"

CColorPoint3::CColorPoint3(void)
{

}

CColorPoint3::CColorPoint3(int x, int y, double z):CColorPoint2(x, y)
{
	this->z = z;
}

CColorPoint3::~CColorPoint3(void)
{

}
