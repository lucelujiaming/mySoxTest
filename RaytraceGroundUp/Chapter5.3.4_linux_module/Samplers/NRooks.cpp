
#include "NRooks.h"

// ---------------------------------------------------------------- default constructor

NRooks::NRooks(void)
    : Sampler()
{}


// ---------------------------------------------------------------- constructor

NRooks::NRooks(const int num_samples)
    :     Sampler(num_samples) {
	// 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- constructor

NRooks::NRooks(const int num_samples, const int m)
    :     Sampler(num_samples, m) {
	// 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- copy constructor

NRooks::NRooks(const NRooks& nr)
    : Sampler(nr) {
	// 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}

// ---------------------------------------------------------------- assignment operator

NRooks& 
NRooks::operator= (const NRooks& rhs) {
    if (this == &rhs)
        return (*this);

    Sampler::operator=(rhs);

    return (*this);
}

// ---------------------------------------------------------------- clone

NRooks*
NRooks::clone(void) const {
    return (new NRooks(*this));
}

// ---------------------------------------------------------------- destructor

NRooks::~NRooks(void) {}


// ---------------------------------------------------------------- generate_samples

// n-Rooks采样(Shirley，1991)可进一步实现1D环境下采样点的均匀分布。
// 其中，可将n个采样点置入一个n * n网格中，从而实现各行各列中只存在一个采样点。
// 如果在n * n网格中置入n个点，且保证各行各列中只包含一个点，各采样点间将彼此分离。
void
NRooks::generate_samples(void) {
    // 各采样点随机置入相应的网格单元中， 这也是采样点的初始化生成方式。
    for (int p = 0; p < num_sets; p++) {
        for (int j = 0; j < num_samples; j++) {
            Point2D sp((j + rand_float()) / num_samples, (j + rand_float()) / num_samples);
            samples.push_back(sp);
        }
    }
    // 在保持n-rooks状态下随机混合x， y坐标，从而生成2D分布状态
    // shuffle the x coordinates of the points within each set
    shuffle_x_coordinates();
	// shuffle the y coordinates of the points within set
    shuffle_y_coordinates();
}
