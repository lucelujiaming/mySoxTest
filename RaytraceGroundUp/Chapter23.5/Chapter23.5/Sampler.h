#ifndef __SAMPLER__
#define __SAMPLER__

#include <vector>
#include <math.h>

#include "Point2D.h"
#include "Point3D.h"
#include "Maths.h"

using std::vector;

class Sampler {

    public:

        Sampler(void);

        Sampler(const int num);

        Sampler(const int num, const int num_sets);

        Sampler(const Sampler& s);

        Sampler&
        operator= (const Sampler& rhs);

        virtual Sampler*
        clone(void) const = 0;

        virtual
        ~Sampler(void);

        void
        set_num_sets(const int np);

        virtual void generate_samples(void) = 0;        // generate sample patterns in a unit square

        int get_num_samples(void);
        void shuffle_x_coordinates(void);
        void shuffle_y_coordinates(void);
        void setup_shuffled_indices(void);                // setup the randomly shuffled indices 
        void map_samples_to_unit_disk(void);              // 将位于正方形内的采样点映射至对应的同心圆上。
        void map_samples_to_hemisphere(const float p);    // 将位于正方形内的采样点映射至对应的半球体上。
        void map_samples_to_sphere(void);

        // the following functions are not const because they change count and jump
        Point2D                                            // get next sample on unit square
        sample_unit_square(void);
        Point2D                                            // get next sample on unit disk
        sample_unit_disk(void);
        Point3D                                            // get next sample on unit hemisphere
        sample_hemisphere(void);
        Point3D                                            // get next sample on unit sphere
        sample_sphere(void);
        Point2D                                            // only used to set up a vector noise table
        sample_one_set(void);                              // this is not discussed in the book, but see the
                                                           // file LatticeNoise.cpp in Chapter 31

    protected:
        // Sampler的继承类包含下列特征：相关类将在场景构造时计算并存储全部采样点。
        // 与在光线跟踪计算中获取采样点相比，该方式将更加高效。
        // 同时，这也将是某些特定采样算法所必须的操作方式。
        int                     num_samples;            // the number of sample points in a set
        int                     num_sets;               // the number of sample sets
        vector<Point2D>         samples;                // sample points on a unit square
        vector<int>             shuffled_indices;       // shuffled samples array indices
        // 存储映射到单位圆的采样点。
        vector<Point2D>         disk_samples;           // sample points on a unit disk
        // 存储映射到半球体的采样点。
        vector<Point3D>         hemisphere_samples;     // sample points on a unit hemisphere
        vector<Point3D>         sphere_samples;         // sample points on a unit sphere
        unsigned long           count;                  // the current number of sample points used
        int                     jump;                   // random index jump
};

#endif
