#include "stdafx.h"
#include "Jittered.h"

// ---------------------------------------------------------------- default constructor

Jittered::Jittered(void)
    : Sampler()
{}


// ---------------------------------------------------------------- constructor

Jittered::Jittered(const int num_samples)
    : Sampler(num_samples) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- constructor

Jittered::Jittered(const int num_samples, const int m)
    : Sampler(num_samples, m) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- copy constructor

Jittered::Jittered(const Jittered& js)
    : Sampler(js) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}

// ---------------------------------------------------------------- assignment operator

Jittered&
Jittered::operator= (const Jittered& rhs) {

    if (this == &rhs)
        return (*this);

    Sampler::operator= (rhs);

    return (*this);
}

// ---------------------------------------------------------------- clone

Jittered*
Jittered::clone(void) const {
    return (new Jittered(*this));
}

// ---------------------------------------------------------------- destructor

Jittered::~Jittered(void) {}


// ---------------------------------------------------------------- generate_samples
// 函数将在单位正方形内生成采样点，对其进行存储的最为简单的方式是使用一维数组，
// 并利用索引对数据元素进行访问。
void
Jittered::generate_samples(void) {

    int n = (int) sqrt((float)num_samples);

    for (int p = 0; p < num_sets; p++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++) {
                // 计算逻辑参见4.2.4 抖动采样
                Point2D sp((k + rand_float()) / n, (j + rand_float()) / n);
                samples.push_back(sp);
            }
        }
    }
}
