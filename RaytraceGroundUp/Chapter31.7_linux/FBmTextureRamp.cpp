#include "FBmTextureRamp.h"

#ifndef NULL
#define NULL (0)
#endif


// ---------------------------------------------------------------- default constructor

FBmTextureRamp::FBmTextureRamp(void)
    :    Texture(),
        noise_ptr(NULL),
        ramp_ptr(NULL),
        perturbation(1.0)
{}

// ----------------------------------------------------------------constructor

FBmTextureRamp::FBmTextureRamp(LatticeNoise* ln_ptr)
    :    Texture(),
        noise_ptr(ln_ptr),
        ramp_ptr(NULL),
        perturbation(1.0)
{}

// ----------------------------------------------------------------constructor

FBmTextureRamp::FBmTextureRamp(Image* im_ptr)
    :    Texture(),
        noise_ptr(NULL),
        ramp_ptr(im_ptr),
        perturbation(1.0)
{}

// ---------------------------------------------------------------- copy constructor

FBmTextureRamp::FBmTextureRamp(const FBmTextureRamp& ft)
    :   Texture(ft),
        noise_ptr(ft.noise_ptr),
        ramp_ptr(ft.ramp_ptr),
        perturbation(ft.perturbation)
{}


// ---------------------------------------------------------------------- clone

FBmTextureRamp*
FBmTextureRamp::clone(void) const {
    return (new FBmTextureRamp(*this));
}


// ---------------------------------------------------------------------- destructor

FBmTextureRamp::~FBmTextureRamp(void) {}


// ---------------------------------------------------------------------- assignment operator

FBmTextureRamp&
FBmTextureRamp::operator= (const FBmTextureRamp& rhs) {
    if (this == &rhs)
        return (*this);

    Texture::operator= (rhs);

    noise_ptr        = rhs.noise_ptr;
    ramp_ptr         = rhs.ramp_ptr;
    perturbation     = rhs.perturbation;

    return (*this);
}


// --------------------------------------------------- get_color
