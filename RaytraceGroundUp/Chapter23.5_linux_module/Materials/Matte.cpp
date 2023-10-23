#include "stdafx.h"
#include "Matte.h"

// ---------------------------------------------------------------- default constructor

Matte::Matte (void)
    :    Material(),
        // �����ⷴ���ʼ��Ϊ��ȫ������Lambertian����
        ambient_brdf(new Lambertian),
        // �������ʼ��Ϊ��ȫ������Lambertian����
        diffuse_brdf(new Lambertian)
{}


// ---------------------------------------------------------------- copy constructor

Matte::Matte(const Matte& m)
    :     Material(m)
{
    if(m.ambient_brdf)
    {
        ambient_brdf = m.ambient_brdf->clone();
    }
    else  
    {
        ambient_brdf = NULL;
    }

    if(m.diffuse_brdf)
    {
        diffuse_brdf = m.diffuse_brdf->clone();
    }
    else  
    {
        diffuse_brdf = NULL;
    }
}


// ---------------------------------------------------------------- clone

Material*
Matte::clone(void) const {
    return (new Matte(*this));
}


// ---------------------------------------------------------------- assignment operator

Matte&
Matte::operator= (const Matte& rhs) {

    if (this == &rhs)
        return (*this);

    Material::operator=(rhs);

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (rhs.ambient_brdf)
    {
        ambient_brdf = rhs.ambient_brdf->clone();
    }
        
    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }

    if (rhs.diffuse_brdf) {
        diffuse_brdf = rhs.diffuse_brdf->clone();
    }

    return (*this);
}


// ---------------------------------------------------------------- destructor

Matte::~Matte(void) {

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }
}


// ---------------------------------------------------------------- shade
// explained on page 271
// ���㻷�����գ�������ȫ����Դ�Լ���ֱ����������ա�
// �Ӷ����ز��ʵ���ɫ��
// �μ���ʽ14.10
RGBColor
Matte::shade(ShadeRec& sr) {

    Vector3D     wo           = -sr.ray.d;
    // ���㻷�����ա��μ���ʽ14.10
    // ���㷽��Ϊ�ɹ�ʽ13.20������˫������ϵ��(ka * cd)����
    // �ɹ�ʽ14.1���������价��������(ls * cl)
    RGBColor     L            = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int         num_lights    = sr.w.lights.size();
    // ����ȫ����Դ�Լ���ֱ����������ա�
    for (int j = 0; j < num_lights; j++) {
        Vector3D wi = sr.w.lights[j]->get_direction(sr);
        // �ж��Ƿ�ɼ���
        float ndotwi = sr.normal * wi;
        // ����ɼ���
        if (ndotwi > 0.0) {
            bool in_shadow = false;
            // �ж��Ƿ������Ӱ����
            if (sr.w.lights[j]->casts_shadows()) {
                // ���������Ӱ���㣬�͹���һ������ײ��Ϊ��㣬ָ�������Դ�Ĺ��ߡ�
                Ray shadowRay(sr.hit_point, wi);
                // �ж����������Ƿ�λ����Ӱ�С�����ǻ�����Դ�����Ϊ�档
                in_shadow = sr.w.lights[j]->in_shadow(shadowRay, sr);
            }
            // �������λ����Ӱ�У��ͼ��������Դ��������������ա�
            if (!in_shadow) {
                // ���㹫ʽ�μ�14.6��14.10�ĵڶ����֡�
                L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * ndotwi;
            }
        }
    }
    return (L);
}


// ---------------------------------------------------------------- area_light_shade
// explained on page 332
// �����������
RGBColor
Matte::area_light_shade(ShadeRec& sr) {

    Vector3D     wo             = -sr.ray.d;
    // ���㻷�����ա��μ���ʽ14.10
    // ���㷽��Ϊ�ɹ�ʽ13.20������˫������ϵ��(ka * cd)����
    // �ɹ�ʽ14.1���������价��������(ls * cl)
    RGBColor     L             = ambient_brdf->rho(sr, wo) * sr.w.ambient_ptr->L(sr);
    int         num_lights    = sr.w.lights.size();
    // ����ȫ����Դ�Լ���ֱ����������ա�
    for (int j = 0; j < num_lights; j++) {
        Vector3D     wi         = sr.w.lights[j]->get_direction(sr);
        // �ж��Ƿ�ɼ���
        float         ndotwi     = sr.normal * wi;
        // ����ɼ���
        if (ndotwi > 0.0) {
            bool in_shadow = false;
            // �ж��Ƿ������Ӱ����
            if (sr.w.lights[j]->casts_shadows()) {
                // ���������Ӱ���㣬�͹���һ������ײ��Ϊ��㣬ָ�������Դ�Ĺ��ߡ�
                Ray shadow_ray(sr.hit_point, wi);
                // �ж����������Ƿ�λ����Ӱ�С�
                in_shadow = sr.w.lights[j]->in_shadow(shadow_ray, sr); 
            }
            // �������λ����Ӱ�У��ͼ��������Դ������������ա�
            // �������ʽ(18.3)
            if (!in_shadow)
            {
				// L += diffuse_brdf->f(sr, wo, wi) * sr.w.lights[j]->L(sr) * sr.w.lights[j]->G(sr) * ndotwi / sr.w.lights[j]->pdf(sr);
				RGBColor diffColor = diffuse_brdf->f(sr, wo, wi);
				RGBColor radianceColor = sr.w.lights[j]->L(sr);
				float    gFactor = sr.w.lights[j]->G(sr);
				float    pdf = sr.w.lights[j]->pdf(sr);
				RGBColor calcResult = diffColor * radianceColor * gFactor * gFactor / pdf;
				L += calcResult;
            }
        }
    }

    return (L);
}


// ---------------------------------------------------------------- get_Le

RGBColor
Matte::get_Le(ShadeRec& sr) const {
    return (black);
}
