// This file contains the definition of the ThinLens class

#include "stdafx.h"
#include "Constants.h"
#include "Point3D.h"
#include "Vector3D.h"
#include "ThinLens.h"

// ----------------------------------------------------------------------------- default constructor

ThinLens::ThinLens(void)
    :    Camera(),
        lens_radius(1.0),
        d(500),
        f(75.0),
        zoom(1.0),
        sampler_ptr(NULL)
{}


// ----------------------------------------------------------------------------- copy constructor

ThinLens::ThinLens(const ThinLens& c)
    :    Camera(c),
        lens_radius(c.lens_radius),
        d(c.d),
        f(c.f),
        zoom(c.zoom)
{
    // need to do a deep copy of the sampler
    if(c.sampler_ptr != NULL) {
        sampler_ptr = c.sampler_ptr->clone();
    }
}


// ----------------------------------------------------------------------------- clone

Camera*
ThinLens::clone(void) const {
    return (new ThinLens(*this));
}


// ----------------------------------------------------------------------------- assignment operator

ThinLens&
ThinLens::operator=(const ThinLens& rhs) {

    if (this == &rhs)
        return (*this);

    Camera::operator=(rhs);

    lens_radius = rhs.lens_radius;
    d            = rhs.d;
    f            = rhs.f;
    zoom        = rhs.zoom;

    // need to do a deep copy of the sampler
    if(rhs.sampler_ptr != NULL) {
        sampler_ptr = rhs.sampler_ptr->clone();
    }

    return (*this);
}


// ----------------------------------------------------------------------------- destructor

ThinLens::~ThinLens(void) {

    delete sampler_ptr;
}


// ----------------------------------------------------------------------------- set_sampler

void
ThinLens::set_sampler(Sampler* sp) {

    if (sampler_ptr) {
        delete sampler_ptr;
        sampler_ptr = NULL;
    }
    // �����ֲ�������ʹ�õĲ�����������
    // �����������������������������������ض�����������ʱ��͸�������ϵĹ���Դ�㽫������ֲ�״̬��
    // ���ս��Ϊ������͸���뾶�����ӣ���Ⱦͼ�񽫰����������㣬��ȴ���Կ��ٵ���ʾ����Ч��
    sampler_ptr = sp;
    // �����Խ�������ӳ������Ӧ�ĵ�λԲ�ϡ�
    sampler_ptr->map_samples_to_unit_disk();
}


// ----------------------------------------------------------------------------- ray direction
// explained on page 173
Vector3D
ThinLens::ray_direction(const Point2D& pixel_point, const Point2D& lens_point) const {
    // ��������������ԭ���μ���ʽ10-1��10-2
    Point2D p(pixel_point.x * f / d, pixel_point.y * f / d);    // hit point on focal plane
    // �μ���ʽ10-3
    Vector3D dir = (p.x - lens_point.x) * u + (p.y - lens_point.y) * v - f * w;
    // ����������������ǵ�λ�����������ִ�й��߸��ټ���֮ǰ���б�Ҫ������й淶��������
    dir.normalize();

    return (dir);
}


// ----------------------------------------------------------------------------- render_stereo

void
ThinLens::render_stereo(const World& w, float x, int pixel_offset) {

        // not implemented yet
}


// ----------------------------------------------------------------------------- render scene

// ����������߼���World::render_scene��һ���ĵط���Ҳ��ÿһ��ͶӰ�㷨��ʵ��ԭ��
// �������Ǹ���������������XY���꣬ʹ�ò�ͬ��ͶӰ��ʽ����ͶӰ���XY���ꡣ
// ���Ҹ��ݴ�����ͷǰ�Ĳ�����ʹ�����ͷ��Ĳ����������߷��򡣲������£�
void
ThinLens::render_scene(const World& w) {

    RGBColor    L;
    Ray            ray;
    ViewPlane    vp(w.vp);
    int         depth         = 0;

    Point2D sp;            // ��ƽ������������еĲ����㣺sample point in [0, 1] X [0, 1]
    Point2D pp;            // ���ݲ����������еĲ����������������ص㣺sample point on a pixel
    Point2D dp;            // ͸�������������еĲ����㣺sample point on unit disk
    Point2D lp;            // ͸���еĲ����㣺sample point on lens

    vp.s /= zoom;

    // ��������Ҫ������������forѭ�����ڣ���ѭ���彫�����������ص���ɫֵ��
    // �ڸú����У��������ڴ��ڵ����½Ǵ���������Ⱦ��
    // ��ǰ�߿ɽ����ص���ɫֵд��һ�����������У����ɻ���������ݡ�
    for (int r = 0; r < vp.vres; r++)            // up
        for (int c = 0; c < vp.hres; c++) {        // across

            L = black;
            // ��β�����
            for (int n = 0; n < vp.num_samples; n++) {
                // ����ViewPlane�д洢�ڲ����������е���һ�������㣬ӳ�䵽��λ���Ρ�
                sp = vp.sampler_ptr->sample_unit_square();
                pp.x = vp.s * (c - vp.hres / 2.0 + sp.x);
                pp.y = vp.s * (r - vp.vres / 2.0 + sp.y);
                
                // ������߼����Ǻ�World::render_scene��һ���ĵط���Ҳ��ÿһ��ͶӰ�㷨��ʵ��ԭ��
                // �������Ǹ���������������XY���꣬ʹ�ò�ͬ��ͶӰ��ʽ����ͶӰ���XY���ꡣ
                // ���Ҹ��ݴ�����ͷǰ�Ĳ�����ʹ�����ͷ��Ĳ����������߷��򡣲������£�
                // 1. ����͸���Լ��洢�ڲ����������е���һ�������㣬ӳ�䵽��λԲ��
                dp = sampler_ptr->sample_unit_disk();
                // 2. ���Ծ�ͷ�뾶���õ���ͷ�еĲ����㡣
                lp = dp * lens_radius;
                // 3. ʹ�þ�ͷ�еĲ��������������
                ray.o = eye + lp.x * u + lp.y * v;
                // 4. ������߷���
                ray.d = ray_direction(pp, lp);
                // L += w.tracer_ptr->trace_ray(ray, depth);
                // �����ߵ�Դ���Ͷ�䷽�������Ϻ� ����trace_ray() �������á�
                // �ú����ǹ��߸������еĺ��ĺ�����������Ե�ǰ����ʵʩ���߸��٣�
                // ͬʱ���������ظ����ص���ɫֵ��
                // ���˴���ȡ��һ�ּ�ӵĵ��÷����� 
                // ���Ըú���ͨ��ָ��Tracer�����tracer_ptrָ����Ե��á�
                L += w.tracer_ptr->trace_ray(ray);
            }
            // ���������ƽ��ֵ��
            L /= vp.num_samples;
            // �����ع�ǿ��ϵ����Ĭ��Ϊ1��
            L *= exposure_time;
            // ѭ���������һ�д��뽫����display_pixel() �������ڴ�������ʾ���ء�
            // �⽫���RGBColorת��Ϊ��ǰ�����֧�ֵ���ʾ��ʽ��
            w.display_pixel(r, c, L);
        }
}
