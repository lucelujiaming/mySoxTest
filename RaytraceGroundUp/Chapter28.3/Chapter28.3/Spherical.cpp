// This file contains the definition of the Spherical class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "Spherical.h"

// ----------------------------------------------------------------------------- default constructor

Spherical::Spherical(void)
    :    Camera(),
        psi_max(180.0) // is this a good default value?
{}


// ----------------------------------------------------------------------------- copy constructor

Spherical::Spherical(const Spherical& c)
    :    Camera(c),
        psi_max(c.psi_max)
{}


// ----------------------------------------------------------------------------- clone

Camera*
Spherical::clone(void) const {
    return (new Spherical(*this));
}


// ----------------------------------------------------------------------------- assignment operator

Spherical&
Spherical::operator=(const Spherical& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator=(rhs);

    psi_max = rhs.psi_max;

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

Spherical::~Spherical(void) {}


// ----------------------------------------------------------------------------- ray direction
// explained on page 192
Vector3D
Spherical::ray_direction(    const Point2D&    pp,
                            const int         hres,
                            const int         vres,
                            const float     s    ) const {

    // compute the normalised device coordinates
    // Ϊ�˸��õ�ִ������ ���ȣ�
    // 1. ����������Ӽ�(�۲�) ����ת��Ϊ��׼���豸���ꡣ
    //    �μ���ʽ11.1��ͼ11.5
    Point2D pn( 2.0 / (s * hres) * pp.x, 2.0 / (s * vres) * pp.y);

    // compute the angles lambda and phi in radians
    // �����ص�ı�׼�豸���궨�巽λ�����lambda�Լ�����psi
    // ת��Ϊ�Ƕȡ�
    float lambda     = pn.x * lambda_max * PI_ON_180;
    float psi         = pn.y * psi_max * PI_ON_180;

    // compute the regular azimuth and polar angles
    // ���ص�ı�׼�豸���궨�巽λ�����lambda�Լ�����psi
    // ����������Ƕ�phi��theta֮��������м򵥵ļ����ϵ��
    //    �μ���ʽ11.5
    float phi         = PI - lambda;
    float theta     = 0.5 * PI - psi;

    float sin_phi     = sin(phi);
    float cos_phi     = cos(phi);
    float sin_theta = sin(theta);
    float cos_theta = cos(theta);

    // equation 11.6
    // ͨ����lambda��psi��ʽ(11.5)�еı�׼��������Ƕ��������
    // ������r=1)���ɼ�����߷���Ϊ��
    //    �μ���ʽ11.6
    Vector3D dir     = sin_theta * sin_phi * u + cos_theta * v + sin_theta * cos_phi * w;
    return (dir);
}


// ----------------------------------------------------------------------------- render_stereo

void
Spherical::render_stereo(const World& w, float x, int pixel_offset) {

        // not implemented yet
}


// ----------------------------------------------------------------------------- render scene
// explained on page 191
void
Spherical::render_scene(const World& wr) {

    RGBColor    L;
    ViewPlane    vp(wr.vp);
    int         hres        = vp.hres;
    int         vres         = vp.vres;
    float        s             = vp.s;
    Ray            ray;
    int         depth         = 0;
    Point2D     sp;                     // ��ƽ������������еĲ����㣺sample point in [0, 1] X [0, 1]
    Point2D     pp;                     // ���ݲ����������еĲ����������������ص㣺sample point on the pixel

    ray.o = eye;

    // ��������Ҫ������������forѭ�����ڣ���ѭ���彫�����������ص���ɫֵ��
    // �ڸú����У��������ڴ��ڵ����½Ǵ���������Ⱦ��
    // ��ǰ�߿ɽ����ص���ɫֵд��һ�����������У����ɻ���������ݡ�
    for (int r = 0; r < vres; r++)        // up
        for (int c = 0; c < hres; c++) {    // across
            L = black;
            // ��β�����
            for (int j = 0; j < vp.num_samples; j++) {
                // ����ViewPlane�д洢�ڲ����������е���һ�������㣬ӳ�䵽��λ���Ρ�
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = s * (c - 0.5 * hres + sp.x);
                pp.y = s * (r - 0.5 * vres + sp.y);
                // 4. ������߷���
                ray.d = ray_direction(pp, hres, vres, s);
                // L += wr.tracer_ptr->trace_ray(ray, depth);
                // �����ߵ�Դ���Ͷ�䷽�������Ϻ� ����trace_ray() �������á�
                // �ú����ǹ��߸������еĺ��ĺ�����������Ե�ǰ����ʵʩ���߸��٣�
                // ͬʱ���������ظ����ص���ɫֵ��
                // ���˴���ȡ��һ�ּ�ӵĵ��÷����� 
                // ���Ըú���ͨ��ָ��Tracer�����tracer_ptrָ����Ե��á�
                L += wr.tracer_ptr->trace_ray(ray);
            }
            // ���������ƽ��ֵ��
            L /= vp.num_samples;
            // �����ع�ǿ��ϵ����Ĭ��Ϊ1��
            L *= exposure_time;
            // ѭ���������һ�д��뽫����display_pixel() �������ڴ�������ʾ���ء�
            // �⽫���RGBColorת��Ϊ��ǰ�����֧�ֵ���ʾ��ʽ��
            wr.display_pixel(r, c, L);
        }
}
