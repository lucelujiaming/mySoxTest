#include "stdafx.h"
#include "math.h"
#include "RGB.h"

CRGB::CRGB(void)
{
	red = 1.0;
	green = 1.0;
	blue = 1.0;
}

CRGB::CRGB(double red, double green, double blue)//重载构造函数
{
	this->red = red;
	this->green = green;
	this->blue = blue;	
}

CRGB::~CRGB(void)
{
}

CRGB operator +(const CRGB &clr0, const CRGB &clr1)//+运算符重载
{
	CRGB color;
	color.red = clr0.red + clr1.red;
	color.green = clr0.green + clr1.green;
	color.blue = clr0.blue + clr1.blue;
	return color;
}

CRGB operator -(const CRGB &clr0, const CRGB &clr1)//-运算符重载
{
	CRGB color;
	color.red = clr0.red - clr1.red;
	color.green = clr0.green - clr1.green;
	color.blue = clr0.blue - clr1.blue;
	return color;
}

CRGB operator *(const CRGB &clr0, const CRGB &clr1)//*运算符重载
{
	CRGB color;
	color.red = clr0.red * clr1.red;
	color.green = clr0.green * clr1.green;
	color.blue = clr0.blue * clr1.blue;
	return color;
}

CRGB operator *(const CRGB &clr, double scalar)//*运算符重载
{
	CRGB color;
	color.red = scalar * clr.red;
	color.green = scalar * clr.green;
	color.blue = scalar * clr.blue;
	return color;
}

CRGB operator *(double scalar, const CRGB &clr)//*运算符重载
{
	CRGB color;
	color.red = scalar * clr.red;
	color.green = scalar * clr.green;
	color.blue = scalar * clr.blue;
	return color;
}

CRGB operator /(const CRGB &clr, double scalar)///运算符重载
{
	if(fabs(scalar) < 1e-4)
		scalar = 1.0;
	CRGB color;
	color.red = clr.red / scalar;
	color.green = clr.green / scalar;
	color.blue = clr.blue / scalar;
	return color;
}

void CRGB::Normalize(void)//归一化
{
	red = (red < 0.0) ? 0.0 : ((red > 1.0) ? 1.0 : red);
	green = (green < 0.0) ? 0.0 : ((green > 1.0) ? 1.0 : green);
	blue = (blue < 0.0) ? 0.0 : ((blue > 1.0) ? 1.0 : blue);
}
