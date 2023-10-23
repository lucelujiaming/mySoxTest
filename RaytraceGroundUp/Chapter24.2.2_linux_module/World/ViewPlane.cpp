#include "stdafx.h"
// This file contains the definition the ViewPlane class

#include "ViewPlane.h"
#include "MultiJittered.h"
#include "Regular.h"

// ---------------------------------------------------------------- default constructor    

ViewPlane::ViewPlane(void)
    :     hres(400),
        vres(400),
        s(1.0),
        num_samples(1),
        sampler_ptr(NULL),
        gamma(1.0),
        inv_gamma(1.0),
        show_out_of_gamut(false)
{}


// ---------------------------------------------------------------- copy constructor

ViewPlane::ViewPlane(const ViewPlane& vp)
    :    hres(vp.hres),
        vres(vp.vres),
        s(vp.s),
        num_samples(vp.num_samples),
        gamma(vp.gamma),
        inv_gamma(vp.inv_gamma),
        show_out_of_gamut(vp.show_out_of_gamut)
{
     // need to do a deep copy of the sampler
    if(vp.sampler_ptr != NULL) {
        sampler_ptr = vp.sampler_ptr->clone();
    }
    else
    {
        sampler_ptr = NULL;
    }
}


// ---------------------------------------------------------------- assignment operator

ViewPlane&
ViewPlane::operator=(const ViewPlane& rhs) {
    if (this == &rhs)
        return (*this);

    hres                = rhs.hres;
    vres                = rhs.vres;
    s                    = rhs.s;
    num_samples            = rhs.num_samples;
    gamma                = rhs.gamma;
    inv_gamma            = rhs.inv_gamma;
    show_out_of_gamut    = rhs.show_out_of_gamut;

//    // need to do a deep copy of the sampler
//    if(rhs.sampler_ptr != NULL) {
//        sampler_ptr = rhs.sampler_ptr->clone();
//    }

    return (*this);
}


// -------------------------------------------------------------- destructor

ViewPlane::~ViewPlane(void) {

//    delete sampler_ptr;
}


// -------------------------------------------------------------- set_samples
// 与set_sampler()函数相比，set_samples()函数的使用频率稍高，
void
ViewPlane::set_samples(const int n) {

    num_samples = n;

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }

    // 该函数在默认情况下使用多重抖动采样。
    if (num_samples > 1)
        sampler_ptr = new MultiJittered(num_samples);
    // 当采样数量为1时，则转变为均匀采样，并在各像素中心处设置相应的采样点，如程序清单4.1所示，
    // 从而可避免出现如图5.22(a)所示的失真图像。    
    else
        sampler_ptr = new Regular(1);
}




