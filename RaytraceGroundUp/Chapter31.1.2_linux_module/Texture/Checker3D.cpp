#include "Checker3D.h"


// ---------------------------------------------------------------- default constructor

Checker3D::Checker3D(void)
	:	Texture(),
		size(0.2),
	    color1(1.0),
	    color2(0.0)
{}


// ---------------------------------------------------------------- copy constructor

Checker3D::Checker3D(const Checker3D& cc)
	:   Texture(cc),
        size(cc.size),
		color1(cc.color1),
		color2(cc.color2)
{}


// ---------------------------------------------------------------------- clone

Checker3D*
Checker3D::clone(void) const {
	return (new Checker3D(*this));
}


// ---------------------------------------------------------------------- destructor

Checker3D::~Checker3D(void) {}


// ---------------------------------------------------------------------- assignment operator

Checker3D&
Checker3D::operator= (const Checker3D& rhs) {
	if (this == &rhs)
		return (*this);

	Texture::operator= (rhs);

	size   = rhs.size;
	color1 = rhs.color1;
	color2 = rhs.color2;

	return (*this);
}


// ------------------------------------- get_color
// 存在多种方式定义棋盘纹理，一种较为简单的方法将使用到标准Ｃ语言库中的floor() 函数。
// 由于该函数将返回double值， 因而可将其转换为int值并加以计算。
RGBColor
Checker3D::get_color(const ShadeRec& sr) const {
    // 避免将平面棋盘表面置于棋盘边界处。例如，可将棋盘地面平面置于y=±0.001处而非y=0.0处。
	float eps = -0.000187453738;	// small random number
	float x = sr.local_hit_point.x + eps;
	float y = sr.local_hit_point.y + eps;
	float z = sr.local_hit_point.z + eps;

	if (((int)floor(x / size) +
         (int)floor(y / size) +
         (int)floor(z / size)) % 2 == 0)
		return (color1);
	else
		return (color2);
}
