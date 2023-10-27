#include "FBmTexture.h"

#ifndef NULL
#define NULL (0)
#endif

// ---------------------------------------------------------------- default constructor

FBmTexture::FBmTexture(void)
	:	Texture(),
        noise_ptr(NULL),
	    color(0.0),
	    min_value(-0.1),
	    max_value(1.1)
{}

// ----------------------------------------------------------------constructor

FBmTexture::FBmTexture(LatticeNoise* ln_ptr)
	:	Texture(),
        noise_ptr(ln_ptr),
	    color(0.0),
	    min_value(-0.1),
	    max_value(1.1)
{}

// ---------------------------------------------------------------- copy constructor

FBmTexture::FBmTexture(const FBmTexture& ft)
	:   Texture(ft),
        noise_ptr(ft.noise_ptr),
	    color(ft.color),
	    min_value(ft.min_value),
	    max_value(ft.max_value)
{}


// ---------------------------------------------------------------------- clone

FBmTexture*
FBmTexture::clone(void) const {
	return (new FBmTexture(*this));
}


// ---------------------------------------------------------------------- destructor

FBmTexture::~FBmTexture(void) {}


// ---------------------------------------------------------------------- assignment operator

FBmTexture&
FBmTexture::operator= (const FBmTexture& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator= (rhs);

	noise_ptr     = rhs.noise_ptr;
	color         = rhs.color;
	min_value     = rhs.min_value;
	max_value     = rhs.max_value;

	return (*this);
}


// --------------------------------------------------- get_color
// 适当地调整value_fBm的返回值范围可增加或降低纹理颜色值的动态范围， 这也是采用min_value 和max_value的原因。此处， get_color() 函数将value=0映射至min_value， 将value=1映射至max_value。value_fBm的返回值通常位于[0， 1] 范围内， 因而缩放值将位于[min_value， max_value] 范围内。这里， 可使用min_value<0和max_value>1增加数值范围， 或使用min_value>0和max_value<1降低数值范围。

RGBColorFBmTexture::get_color(const ShadeRec& sr) const {	float value = noise_ptr->value_fbm(sr.local_hit_point); // in the range (0, 1)	value = min_value + (max_value - min_value) * value;	// in the range (min_value, max_value)	return (value * color);}