#include "stdafx.h"
#include "ColorP3.h"


CColorP3::CColorP3(void)
{
	z = 0.0;
}

CColorP3::CColorP3(double x, double y, double z):CColorP2(x, y)
{
	this->z = z;
}

CColorP3::~CColorP3(void)
{

}