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

RGBColor