#include "stdafx.h"
#include "MultiJittered.h"

// ---------------------------------------------------------------- default constructor

MultiJittered::MultiJittered(void)
    : Sampler()
{}


// ---------------------------------------------------------------- constructor

MultiJittered::MultiJittered(const int num_samples)
    :     Sampler(num_samples) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- constructor

MultiJittered::MultiJittered(const int num_samples, const int m)
    :     Sampler(num_samples, m) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}


// ---------------------------------------------------------------- copy constructor

MultiJittered::MultiJittered(const MultiJittered& mjs)
    : Sampler(mjs) {
    // 全部继承类只需实现generate_samples()函数，并在构造函数中加以调用。
    generate_samples();
}

// ---------------------------------------------------------------- assignment operator

MultiJittered&
MultiJittered::operator= (const MultiJittered& rhs) {
    if (this == &rhs)
        return (*this);

    Sampler::operator=(rhs);

    return (*this);
}

// ---------------------------------------------------------------- clone

MultiJittered*
MultiJittered::clone(void) const {
    return (new MultiJittered(*this));
}

// ---------------------------------------------------------------- destructor

MultiJittered::~MultiJittered(void) {}


// ---------------------------------------------------------------- generate_samples

// This is based on code in Chui et al. (1994), cited in the references
// The overloaded functions rand_int and rand_float (called from rand_int),
// which take arguments, are defined here
// explained on page 107
int
MultiJittered::rand_int(int min, int max) {

    if(min == max)
        return max;
    else
        return (rand() % (max - min)) + min;
}

float
rand_float(float min, float max) {

    float r = float(rand()) / (float(RAND_MAX) + 1.0);    // random double in range 0.0 to 1.0 (non inclusive)
    float X = min + r * (max - min);                    // transform to wanted range
    return X;
}

// 多重抖动采样技术由Chiu等开发(1994) ，旨在改进n-rooks采样时的2D分布状态并保留其均匀的1D投影分布特征。
// 从本质上讲， 该方案组合了抖动采样以及n-rooks采样并使用了2级网格。
void
MultiJittered::generate_samples(void) {
    // num_samples needs to be a perfect square

    // 这里，仍将采用尺寸为16×16的网格。
    // 由于在该网格上添加了一个4×4网格，因而可将该16×16网格视为子网格。
    int n = (int)sqrt((float)num_samples);
    float subcell_width = 1.0 / ((float) num_samples);

    // fill the samples array with dummy points to allow us to use the [ ] notation when we set the
    // initial patterns
    // 初始状态下，将在4×4网格中分别生成一个采样点
    Point2D fill_point;
    for (int j = 0; j < num_samples * num_sets; j++)
        samples.push_back(fill_point);

    // distribute points in the initial patterns
    // 在相应的子网格单元中随机生成采样点。相对于4×4网格，这将呈现一类抖动分布；
    // 而在子网格内，还将同时满足n-rooks条件。
    for (int p = 0; p < num_sets; p++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                samples[i * n + j + p * num_samples].x = (i * n + j) * subcell_width + rand_float(0, subcell_width);
                samples[i * n + j + p * num_samples].y = (j * n + i) * subcell_width + rand_float(0, subcell_width);
            }
    // 随后， 在子网格内保持n-rooks条件下对采样点实施混合操作。
    // shuffle x coordinates

    for (int p = 0; p < num_sets; p++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int k = rand_int(j, n - 1);
                float t = samples[i * n + j + p * num_samples].x;
                samples[i * n + j + p * num_samples].x = samples[i * n + k + p * num_samples].x;
                samples[i * n + k + p * num_samples].x = t;
            }

    // shuffle y coordinates

    for (int p = 0; p < num_sets; p++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                int k = rand_int(j, n - 1);
                float t = samples[j * n + i + p * num_samples].y;
                samples[j * n + i + p * num_samples].y = samples[k * n + i + p * num_samples].y;
                samples[k * n + i + p * num_samples].y = t;
        }
}
