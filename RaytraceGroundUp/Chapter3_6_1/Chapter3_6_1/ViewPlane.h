﻿#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

#include "Sampler.h"

//-------------------------------------------------------------------------------------- class ViewPlane
// View Plane类存储了水平、垂直方向上的全部像素以及像素尺寸。
// 视平面中包含了构成场景渲染窗口的全部像素。
// 在本章中，视平面将垂直于Zw轴，其中的像素将以水平多行方式排列。另外，zw轴将穿越窗口中心位置。
class ViewPlane {

	public:
		int 			hres;						// horizontal image resolution
		int 			vres;						// vertical image resolution
		float			s;							// pixel size
		int				num_samples;				// number of samples per pixel
		int				max_depth;					// max number of reflected bounce rays
		
		float			gamma;						// gamma correction factor
		float			inv_gamma;					// the inverse of the gamma correction factor
		bool			show_out_of_gamut;			// display red if RGBColor out of gamut
		Sampler*		sampler_ptr;

	public:
		ViewPlane();								// default Constructor

		ViewPlane(const ViewPlane& vp);				// copy constructor

		ViewPlane& operator= (const ViewPlane& rhs);		// assignment operator

		~ViewPlane();							// destructor

		void
		set_hres(const int h_res);

		void
		set_vres(const int v_res);

		void
		set_pixel_size(const float size);

		void
		set_gamma(const float g);

		void
		set_gamut_display(const bool show);

		void
		set_samples(const int n);

		// void
		// set_sampler(Sampler* sp);

		void
		set_max_depth(int depth);
};


// ------------------------------------------------------------------------------ set_hres

inline void
ViewPlane::set_hres(const int h_res) {
	hres = h_res;
}


// ------------------------------------------------------------------------------ set_vres

inline void
ViewPlane::set_vres(const int v_res) {
	vres = v_res;
}


// ------------------------------------------------------------------------------ set_pixel_size

inline void
ViewPlane::set_pixel_size(const float size) {
	s = size;
}


// ------------------------------------------------------------------------------ set_gamma

inline void
ViewPlane::set_gamma(const float g) {
	gamma = g;
	inv_gamma = 1.0 / gamma;
}


// ------------------------------------------------------------------------------ set_gamut_display

inline void
ViewPlane::set_gamut_display(const bool show) {
	show_out_of_gamut = show;
}


// ------------------------------------------------------------------------------ set_max_depth

inline void
ViewPlane::set_max_depth(int depth) {
	max_depth = depth;
}


#endif
