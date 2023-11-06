#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

#include "Sampler.h"

//-------------------------------------------------------------------------------------- class ViewPlane
// View Plane��洢��ˮƽ����ֱ�����ϵ�ȫ�������Լ����سߴ硣
// ��ƽ���а����˹��ɳ�����Ⱦ���ڵ�ȫ�����ء�
// �ڱ����У���ƽ�潫��ֱ��Zw�ᣬ���е����ؽ���ˮƽ���з�ʽ���С����⣬zw�Ὣ��Խ��������λ�á�
class ViewPlane {

    public:
        int             hres;                        // horizontal image resolution
        int             vres;                        // vertical image resolution
        float           s;                           // pixel size
        // ��ƽ�潫�ṩִ�п���ݲ���ʱ�Ĳ����������������Ҫ��������
        int             num_samples;                 // number of samples per pixel
        // P400�����ߵ�����������
        int             max_depth;                   // max number of reflected bounce rays
        
        float           gamma;                       // gamma correction factor
        float           inv_gamma;                   // the inverse of the gamma correction factor
        bool            show_out_of_gamut;           // display red if RGBColor out of gamut
        // ��ƽ�潫�ṩִ�п���ݲ���ʱ�Ĳ����������������Ҫ�洢ָ������������ָ��
        Sampler*        sampler_ptr;

    public:
        ViewPlane();                                 // default Constructor

        ViewPlane(const ViewPlane& vp);              // copy constructor

        ViewPlane& operator= (const ViewPlane& rhs); // assignment operator

        ~ViewPlane();                                // destructor

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

        // ��ƽ�潫�ṩִ�п���ݲ���ʱ�Ĳ����������������Ҫ��Ӧ�����ú���
        void set_sampler(Sampler* sp);

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

// ������ʹ��һ������������ָ����Ϊ�������ú������ڲ����������Ͳ�û���κ�Ҫ��
inline void ViewPlane::set_sampler(Sampler* sp) {
    if(sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr=NULL;
    }
    num_samples = sp->get_num_samples();
    sampler_ptr = sp;
    sampler_ptr->map_samples_to_hemisphere(50);
}

#endif
