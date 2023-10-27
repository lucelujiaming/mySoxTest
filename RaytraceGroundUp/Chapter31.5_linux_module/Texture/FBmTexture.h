#ifndef FBMTEXTURE_H
#define FBMTEXTURE_H

#include "Texture.h"
#include "LatticeNoise.h"
// 与fBm相对应的纹理类
class FBmTexture : public Texture
{
	public:

		FBmTexture(void);

		FBmTexture(LatticeNoise* ln_ptr);

		FBmTexture(const FBmTexture& ft);

		virtual FBmTexture*
		clone(void) const;

		virtual
		~FBmTexture(void);

		void
		set_noise(LatticeNoise* ln_ptr);

		void
		set_color(const RGBColor& c);

		void
		set_color(const float r, const float g, const float b);

		void
		set_color(const float c);

		void
		set_min_value(const float mi);

		void
		set_max_value(const float ma);

		virtual RGBColor
		get_color(const ShadeRec& sr) const;

	protected:

		FBmTexture&
		operator= (const FBmTexture& rhs);

    private:

		LatticeNoise*		noise_ptr;
        // 相对于灰度图，向纹理中添加相应的颜色值可进一步丰富图像的内容。		RGBColor 	  		color;
        // 数据成员min_value和max_value使得纹理对噪声函数value_fBm()返回值的缩放操作成为可能。		float		  		min_value, max_value;	// scaling factors};


// ---------------------------------------------------------------- set_color

inline void
FBmTexture::set_noise(LatticeNoise* ln_ptr) {
	noise_ptr = ln_ptr;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTexture::set_color(const RGBColor& c) {
	color = c;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTexture::set_color(const float r, const float g, const float b) {
	color.r = r; color.g = g; color.b = b;
}


// ---------------------------------------------------------------- set_color

inline void
FBmTexture::set_color(const float c) {
	color.r = c; color.g = c; color.b = c;
}


// ---------------------------------------------------------------- set_min_value

inline void
FBmTexture::set_min_value(const float mi) {
	min_value = mi;
}


// ---------------------------------------------------------------- set_max_value

inline void
FBmTexture::set_max_value(const float ma) {
	max_value = ma;
}

#endif // FBMTEXTURE_H
