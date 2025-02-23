#include "Constants.h"
#include "Instance.h"

// initialise the static transformation matrix

Matrix
Instance::forward_matrix;


// ----------------------------------------------------------------  default constructor

Instance::Instance(void)
    :     GeometricObject(),
        object_ptr(NULL),
        inv_matrix(),
        bbox(),
        transform_the_texture(true)
{
    forward_matrix.set_identity();
}


// ----------------------------------------------------------------  constructor

Instance::Instance(GeometricObject* obj_ptr)
    :    GeometricObject(),
        object_ptr(obj_ptr),
        inv_matrix(),
        bbox(),
        transform_the_texture(true)
{
    forward_matrix.set_identity();
}


// ---------------------------------------------------------------- copy constructor

Instance::Instance (const Instance& instance)
    :     GeometricObject(instance),
        inv_matrix(instance.inv_matrix),
        transform_the_texture(instance.transform_the_texture)
{
    if(instance.object_ptr)
        object_ptr = instance.object_ptr->clone();
    else  object_ptr = NULL;
}


// ---------------------------------------------------------------- clone

Instance*
Instance::clone(void) const {
    return (new Instance(*this));
}


// ---------------------------------------------------------------- destructor

Instance::~Instance(void) {
    if (object_ptr) {
        delete object_ptr;
        object_ptr = NULL;
    }
}


// ---------------------------------------------------------------- assignment operator

Instance&
Instance::operator= (const Instance& rhs) {
    if (this == &rhs)
        return (*this);

    GeometricObject::operator=(rhs);

    if(object_ptr) {
        delete object_ptr;
        object_ptr = NULL;
    }

    if (rhs.object_ptr)
        object_ptr = rhs.object_ptr->clone();
    else
        object_ptr = NULL;

    inv_matrix                = rhs.inv_matrix;
    bbox                    = rhs.bbox;
    transform_the_texture     = rhs.transform_the_texture;

    return (*this);
}


//------------------------------------------------------------------ set_object

void
Instance::set_object(GeometricObject* obj_ptr) {
    object_ptr = obj_ptr;
}


//---------------------------------------------------------------- set_bounding_box
// This function is only called when the instance is to be placed in a grid
// It will always be called from a build function

void
Instance::compute_bounding_box(void) {

    // First get the object's untransformed BBox

    BBox object_bbox = object_ptr->get_bounding_box();


    // Now apply the affine transformations to the box.
    // We must apply the transformations to all 8 vertices of the orginal box
    // and then work out the new minimum and maximum values

    // Construct the eight vertices as 3D points:

    Point3D v[8];

    v[0].x = object_bbox.x0; v[0].y = object_bbox.y0; v[0].z = object_bbox.z0;
    v[1].x = object_bbox.x1; v[1].y = object_bbox.y0; v[1].z = object_bbox.z0;
    v[2].x = object_bbox.x1; v[2].y = object_bbox.y1; v[2].z = object_bbox.z0;
    v[3].x = object_bbox.x0; v[3].y = object_bbox.y1; v[3].z = object_bbox.z0;

    v[4].x = object_bbox.x0; v[4].y = object_bbox.y0; v[4].z = object_bbox.z1;
    v[5].x = object_bbox.x1; v[5].y = object_bbox.y0; v[5].z = object_bbox.z1;
    v[6].x = object_bbox.x1; v[6].y = object_bbox.y1; v[6].z = object_bbox.z1;
    v[7].x = object_bbox.x0; v[7].y = object_bbox.y1; v[7].z = object_bbox.z1;


    // Transform these using the forward matrix

    v[0] = forward_matrix * v[0];
    v[1] = forward_matrix * v[1];
    v[2] = forward_matrix * v[2];
    v[3] = forward_matrix * v[3];
    v[4] = forward_matrix * v[4];
    v[5] = forward_matrix * v[5];
    v[6] = forward_matrix * v[6];
    v[7] = forward_matrix * v[7];


    // Since forward_matrix is a static variable, we must now set it to the unit matrix
    // This sets it up correctly for the next instance object

    forward_matrix.set_identity();


    // Compute the minimum values

    float x0 = kHugeValue;
    float y0 = kHugeValue;
    float z0 = kHugeValue;

    for (int j = 0; j <= 7; j++)  {
        if (v[j].x < x0)
            x0 = v[j].x;
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].y < y0)
            y0 = v[j].y;
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].z < z0)
            z0 = v[j].z;
    }

    // Compute the minimum values

    float x1 = -kHugeValue;
    float y1 = -kHugeValue;
    float z1 = -kHugeValue;

    for (int j = 0; j <= 7; j++) {
        if (v[j].x > x1)
            x1 = v[j].x;
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].y > y1)
            y1 = v[j].y;
    }

    for (int j = 0; j <= 7; j++) {
        if (v[j].z > z1)
            z1 = v[j].z;
    }

    // Assign values to the bounding box

    bbox.x0 = x0;
    bbox.y0 = y0;
    bbox.z0 = z0;
    bbox.x1 = x1;
    bbox.y1 = y1;
    bbox.z1 = z1;
}


//---------------------------------------------------------------- get_bounding_box

BBox
Instance::get_bounding_box(void) {
    return (bbox);
}

//---------------------------------------------------------------- get_material

Material*
Instance::get_material(void) const {
    return (material_ptr);
}


//---------------------------------------------------------------- set_material
// Here, material_ptr is GeometricObject::material_ptr

void
Instance::set_material(Material* m_ptr) {
    material_ptr = m_ptr;
}

//----------------------------------------------------------------------------------------- hit
// explained on page 428
// 如何执行光线与变换对象间的相互运算。
// 1. 对光线采用逆变换序列，生成逆变换光线。
// 2. 在逆变换光线和未转换对象之间执行相交测试。
// 3. 在碰撞点处计算未转换对象的法线。
// 4. 使用未转换对象上的碰撞点计算原光线和转换对象之间的碰撞点。
// 5. 使用未转换对象的法线计算转换对象的法线。
// 也就是所谓：既然世界不愿意向我走过来，我就自己走过去。
// 这个函数可以嵌套使用。
bool
Instance::hit(const Ray& ray, double& t, ShadeRec& sr) const {
    // 声明一个临时变量，表示逆变换光线。
    Ray inv_ray(ray);
    // 1. 对光线采用逆变换序列，生成逆变换光线。
    // 参见公式21.5
    inv_ray.o = inv_matrix * inv_ray.o;
    // 参见公式21.6
    inv_ray.d = inv_matrix * inv_ray.d;

    // 2. 在逆变换光线和未转换对象之间执行相交测试。
    // 3. 在碰撞点处计算未转换对象的法线。
    // 这里返回的是未变换对象上的单位法线。
    if (object_ptr->hit(inv_ray, t, sr)) {
        // 5. 使用未转换对象的法线计算转换对象的法线。
        // 参见公式21.13
        // 也就是在碰撞点上左乘变换矩阵，等于在垂直的法线方向上左乘逆变换矩阵。
        sr.normal = inv_matrix * sr.normal;
        sr.normal.normalize();
        // 这行代码有点奇怪。但是不这么写，有可能程序会崩溃。
        // 因为，图元对象可能没有指定材质。因此上，不能直接取这个指针。
        // 因此上，我们采用了这样一个策略。就是：
        //   先给Instance自己设定一个材质。如果碰撞点所在的对象有材质，
        //   就用这个材质。否则就用Instance自己的。
        if (object_ptr->get_material())
        {
            material_ptr = object_ptr->get_material();
        }
        // 如果转换材质
        if (!transform_the_texture)
        {
            // 4. 使用未转换对象上的碰撞点计算原光线和转换对象之间的碰撞点。
            // 这里有一个技巧。就是：我不需要用逆变换乘上计算出来的碰撞点。
            // 而是直接使用计算出来的t来计算碰撞点。
            sr.local_hit_point = ray.o + t * ray.d;
        }
        return (true);
    }
    return (false);
}


// ----------------------------------------------------------------- shadow_hit

bool
Instance::shadow_hit(const Ray& ray, float& tmin) const {
    // 声明一个临时变量，表示逆变换光线。
    Ray inv_ray(ray);
    // 1. 对光线采用逆变换序列，生成逆变换光线。
    // 参见公式21.5
    inv_ray.o = inv_matrix * inv_ray.o;
    // 参见公式21.6
    inv_ray.d = inv_matrix * inv_ray.d;

    // 2. 在逆变换光线和未转换对象之间执行相交测试。
    // 3. 在碰撞点处计算未转换对象的法线。
    if (object_ptr->shadow_hit(inv_ray, tmin)) {

        if (object_ptr->get_material())
        {
            material_ptr = object_ptr->get_material();
        }

        return (true);
    }

    return (false);
}


//-------------------------------------------------------------------------------- scale

void
Instance::scale(const Vector3D& s) {

    Matrix    inv_scaling_matrix;            // temporary inverse scaling matrix

    inv_scaling_matrix.m[0][0] = 1.0 / s.x;
    inv_scaling_matrix.m[1][1] = 1.0 / s.y;
    inv_scaling_matrix.m[2][2] = 1.0 / s.z;

    inv_matrix = inv_matrix * inv_scaling_matrix;

    Matrix    scaling_matrix;                // temporary scaling matrix

    scaling_matrix.m[0][0] = s.x;
    scaling_matrix.m[1][1] = s.y;
    scaling_matrix.m[2][2] = s.z;

    forward_matrix = scaling_matrix * forward_matrix;
}


//-------------------------------------------------------------------------------- scale

void
Instance::scale(const double a, const double b, const double c) {

    Matrix inv_scaling_matrix;                    // temporary inverse scaling matrix

    inv_scaling_matrix.m[0][0] = 1.0 / a;
    inv_scaling_matrix.m[1][1] = 1.0 / b;
    inv_scaling_matrix.m[2][2] = 1.0 / c;

    inv_matrix = inv_matrix * inv_scaling_matrix;

    Matrix scaling_matrix;                        // temporary scaling matrix

    scaling_matrix.m[0][0] = a;
    scaling_matrix.m[1][1] = b;
    scaling_matrix.m[2][2] = c;

    forward_matrix = scaling_matrix * forward_matrix;
}


//-------------------------------------------------------------------------------- translate

void
Instance::translate(const Vector3D& trans) {

    Matrix inv_translation_matrix;                // temporary inverse translation matrix

    inv_translation_matrix.m[0][3] = -trans.x;
    inv_translation_matrix.m[1][3] = -trans.y;
    inv_translation_matrix.m[2][3] = -trans.z;

    inv_matrix = inv_matrix * inv_translation_matrix;

    Matrix translation_matrix;                    // temporary translation matrix

    translation_matrix.m[0][3] = trans.x;
    translation_matrix.m[1][3] = trans.y;
    translation_matrix.m[2][3] = trans.z;

    forward_matrix = translation_matrix * forward_matrix;
}


//-------------------------------------------------------------------------------- translate

void
Instance::translate(const double dx, const double dy, const double dz) {

    Matrix inv_translation_matrix;                // temporary inverse translation matrix

    inv_translation_matrix.m[0][3] = -dx;
    inv_translation_matrix.m[1][3] = -dy;
    inv_translation_matrix.m[2][3] = -dz;

    inv_matrix = inv_matrix * inv_translation_matrix;

    Matrix translation_matrix;                    // temporary translation matrix

    translation_matrix.m[0][3] = dx;
    translation_matrix.m[1][3] = dy;
    translation_matrix.m[2][3] = dz;

    forward_matrix = translation_matrix * forward_matrix;
}



//-------------------------------------------------------------------------------- rotate_x

void
Instance::rotate_x(const double theta) {

    double sin_theta = sin(theta * PI_ON_180);
    double cos_theta = cos(theta * PI_ON_180);

    Matrix inv_x_rotation_matrix;                    // temporary inverse rotation matrix about x axis

    inv_x_rotation_matrix.m[1][1] = cos_theta;
    inv_x_rotation_matrix.m[1][2] = sin_theta;
    inv_x_rotation_matrix.m[2][1] = -sin_theta;
    inv_x_rotation_matrix.m[2][2] = cos_theta;

    inv_matrix = inv_matrix * inv_x_rotation_matrix;

    Matrix x_rotation_matrix;                        // temporary rotation matrix about x axis

    x_rotation_matrix.m[1][1] = cos_theta;
    x_rotation_matrix.m[1][2] = -sin_theta;
    x_rotation_matrix.m[2][1] = sin_theta;
    x_rotation_matrix.m[2][2] = cos_theta;

    forward_matrix = x_rotation_matrix * forward_matrix;
}


//-------------------------------------------------------------------------------- rotate_y

void
Instance::rotate_y(const double theta) {

    double sin_theta = sin(theta * PI / 180.0);
    double cos_theta = cos(theta * PI / 180.0);

    Matrix inv_y_rotation_matrix;                    // temporary inverse rotation matrix about y axis

    inv_y_rotation_matrix.m[0][0] = cos_theta;
    inv_y_rotation_matrix.m[0][2] = -sin_theta;
    inv_y_rotation_matrix.m[2][0] = sin_theta;
    inv_y_rotation_matrix.m[2][2] = cos_theta;

    inv_matrix = inv_matrix * inv_y_rotation_matrix;

    Matrix y_rotation_matrix;                        // temporary rotation matrix about x axis

    y_rotation_matrix.m[0][0] = cos_theta;
    y_rotation_matrix.m[0][2] = sin_theta;
    y_rotation_matrix.m[2][0] = -sin_theta;
    y_rotation_matrix.m[2][2] = cos_theta;

    forward_matrix = y_rotation_matrix * forward_matrix;
}



//-------------------------------------------------------------------------------- rotate_z

void
Instance::rotate_z(const double theta) {
    double sin_theta = sin(theta * PI / 180.0);
    double cos_theta = cos(theta * PI / 180.0);

    Matrix inv_z_rotation_matrix;                    // temporary inverse rotation matrix about y axis

    inv_z_rotation_matrix.m[0][0] = cos_theta;
    inv_z_rotation_matrix.m[0][1] = sin_theta;
    inv_z_rotation_matrix.m[1][0] = -sin_theta;
    inv_z_rotation_matrix.m[1][1] = cos_theta;

    inv_matrix = inv_matrix * inv_z_rotation_matrix;

    Matrix z_rotation_matrix;                        // temporary rotation matrix about y axis

    z_rotation_matrix.m[0][0] = cos_theta;
    z_rotation_matrix.m[0][1] = -sin_theta;
    z_rotation_matrix.m[1][0] = sin_theta;
    z_rotation_matrix.m[1][1] = cos_theta;

    forward_matrix = z_rotation_matrix * forward_matrix;
}


//-------------------------------------------------------------------------------- shear

void
Instance::shear(const Matrix& s) {

    Matrix inverse_shearing_matrix;    // inverse shear matrix

    // discriminant

    double d = 1.0     - s.m[1][0] * s.m[0][1] - s.m[2][0] * s.m[0][2]  - s.m[2][1] * s.m[1][2]
                    + s.m[1][0] * s.m[2][1] * s.m[0][2] + s.m[2][0] * s.m[0][1] * s.m[2][1];

    // diagonals

    inverse_shearing_matrix.m[0][0] = 1.0 - s.m[2][1] * s.m[1][2];
    inverse_shearing_matrix.m[1][1] = 1.0 - s.m[2][0] * s.m[0][2];
    inverse_shearing_matrix.m[2][2] = 1.0 - s.m[1][0] * s.m[0][1];
    inverse_shearing_matrix.m[3][3] = d;

    // first row

    inverse_shearing_matrix.m[0][1] = -s.m[1][0] + s.m[2][0] * s.m[1][2];
    inverse_shearing_matrix.m[0][2] = -s.m[2][0] + s.m[1][0] * s.m[2][1];

    // second row

    inverse_shearing_matrix.m[1][0] = -s.m[0][1] + s.m[2][1] * s.m[0][2];
    inverse_shearing_matrix.m[1][2] = -s.m[2][1] + s.m[2][0] * s.m[0][1];

    // third row

    inverse_shearing_matrix.m[2][0] = -s.m[0][2] + s.m[0][1] * s.m[1][2];
    inverse_shearing_matrix.m[2][1] = -s.m[1][2] + s.m[1][0] * s.m[0][2] ;

    // divide by discriminant

    inverse_shearing_matrix = inverse_shearing_matrix / d;

    inv_matrix = inv_matrix * inverse_shearing_matrix;

    forward_matrix = s * forward_matrix;
}


