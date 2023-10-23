#include "Phong.h"

// ---------------------------------------------------------------- default constructor

Phong::Phong (void)
    :    Material(),
        // �����ⷴ���ʼ��Ϊ��ȫ������Lambertian����
        ambient_brdf(new Lambertian),
        // �������ʼ��Ϊ��ȫ������Lambertian����
        diffuse_brdf(new Lambertian),
        specular_brdf(new GlossySpecular)
{}


// ---------------------------------------------------------------- copy constructor

Phong::Phong(const Phong& m)
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

    if(m.specular_brdf)
    {
        specular_brdf = m.specular_brdf->clone();
    }
    else 
    {
        specular_brdf = NULL;
    }
}


// ---------------------------------------------------------------- clone

Material*
Phong::clone(void) const {
    return (new Phong(*this));
}


// ---------------------------------------------------------------- assignment operator

Phong&
Phong::operator= (const Phong& rhs) {

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
    if (specular_brdf) {
        delete specular_brdf;
        specular_brdf = NULL;
    }

    if (rhs.specular_brdf) {
        specular_brdf = rhs.specular_brdf->clone();
    }
    return (*this);
}


// ---------------------------------------------------------------- destructor

Phong::~Phong(void) {

    if (ambient_brdf) {
        delete ambient_brdf;
        ambient_brdf = NULL;
    }

    if (diffuse_brdf) {
        delete diffuse_brdf;
        diffuse_brdf = NULL;
    }

    if (specular_brdf) {
        delete specular_brdf;
        specular_brdf = NULL;
    }
}


// ---------------------------------------------------------------- shade
// explained on page 285
// ���㻷�����գ�������ȫ����Դ�Լ���ֱ����������պ;���߹ⷴ����ա�
// �Ӷ����ز��ʵ���ɫ��
// �μ���ʽ15.8
RGBColor
Phong::shade(ShadeRec& sr) {

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
            // �������λ����Ӱ�У��ͼ��������Դ��������������պ;���߹ⷴ����ա�
            if (!in_shadow) {
                // ���㹫ʽ�μ�15.7��15.8�ĵڶ����֡�
                L += (    diffuse_brdf->f(sr, wo, wi)    // ���������
                        + specular_brdf->f(sr, wo, wi))  // ����߹ⷴ�����
                            * sr.w.lights[j]->L(sr) * ndotwi;
            }
        }
    }

    return (L);
}


// ---------------------------------------------------------------- get_Le

RGBColor
Phong::get_Le(ShadeRec& sr) const{
    return (black);
}
