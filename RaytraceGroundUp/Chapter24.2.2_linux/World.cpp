#include "stdafx.h"
// this file contains the definition of the World class

// #include "wxraytracer.h"

#include <stdlib.h>
#include "Constants.h"

// utilities

#include "Vector3D.h"
#include "Point2D.h"
#include "Point3D.h"
#include "Normal.h"
#include "Ray.h"

#include "World.h"

// build functions
#include "MultipleObjects.h"
#include "RayCast.h"
#include "RGBColor.h"
#include "Sphere.h"

#include <fstream>
#include <iostream>
using namespace std;
extern ofstream out;

#include "NRooks.h"
#include "Pinhole.h"
#include "MultiJittered.h"
// #include "RayCast.h"
#include "Ambient.h"
#include "ThinLens.h"
#include "FishEye.h"
#include "Spherical.h"
#include "StereoCamera.h"
#include "PointLight.h"
#include "Matte.h"
#include "Phong.h"

#include "AmbientOccluder.h"

#include "MyRectangle.h"
#include "Emissive.h"
#include "AreaLight.h"

#include "SphereConcave.h"
#include "EnvironmentLight.h"

#include "Triangle.h"
#include "Instance.h"

#include "Grid.h"
#include "SmoothMeshTriangle.h"

#include "Whitted.h"
#include "Reflective.h"

// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
    :      background_color(black),
        tracer_ptr(NULL),
        ambient_ptr(new Ambient),
        camera_ptr(NULL)
{}

World::~World(void) {

    if(tracer_ptr) {
        delete tracer_ptr;
        tracer_ptr = NULL;
    }

    if (camera_ptr) {
        delete camera_ptr;
        camera_ptr = NULL;
    }    
}

//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis
// 
void World::render_scene() const
{
    RGBColor pixel_color;
    Ray ray;

    double zw = 200.0;
    // double x, y;
    Point3D sp;                                 // sample point in [0, 1] x [0, 1]
    Point2D pp;                                 // sample point on a pixel

    ray.d = Vector3D(0, 0, -1);
    for (int r = 0; r < vp.vres; r++) {
        for (int c = 0; c < vp.hres; c++) {
            pixel_color = background_color;
            
            for(int j=0; j < vp.num_samples; j++) {
                sp = vp.sampler_ptr->sample_hemisphere();
                pp.x = vp.s*(c-0.5*vp.hres + sp.x);
                pp.y = vp.s*(r-0.5*vp.vres + sp.y);
                ray.o = Point3D(pp.x, pp.y, zw);
                pixel_color += tracer_ptr->trace_ray(ray);
            }

            pixel_color /= vp.num_samples; // average the colors

            display_pixel(r, c, pixel_color);
        }
    }
}

void World::render_perspective(void) const {
    RGBColor pixel_color;
    Ray ray;

    int eye = 500;  
    int d = 400;    
    int s = 1;      
    
    // open_window(hres, vres);
    ray.o = Point3D(0.0, 0.0, eye);
    
    for(int r=0; r < vp.vres; r++) { // up
        for(int c=0; c<=vp.hres; c++) { //across
            ray.d = Vector3D(s*(c-0.5*(vp.hres-1.0) ),
                        s*(r-0.5*(vp.vres-1.0)), -d);
            ray.d.normalize();
            pixel_color=tracer_ptr->trace_ray(ray) ;
            display_pixel(r, c, pixel_color);
        }
    }
}

// ------------------------------------------------------------------ clamp
// explained on page 274
RGBColor
World::max_to_one(const RGBColor& c) const {

    float max_value = max(c.r, max(c.g, c.b));

    if (max_value > 1.0)
        return (c / max_value);
    else
        return (c);
}


// ------------------------------------------------------------------ clamp_to_color
// Set color to red if any component is greater than one
// explained on page 275
RGBColor
World::clamp_to_color(const RGBColor& raw_color) const {

    RGBColor c(raw_color);

    if (raw_color.r > 1.0 || raw_color.g > 1.0 || raw_color.b > 1.0) {
        c.r = 1.0; c.g = 0.0; c.b = 0.0;
    }

    return (c);
}


// ---------------------------------------------------------------------------display_pixel
// raw_color is the pixel color computed by the ray tracer
// its RGB floating point components can be arbitrarily large
// mapped_color has all components in the range [0, 1], but still floating point
// display color has integer components for computer display
// the Mac's components are in the range [0, 65535]
// a PC's components will probably be in the range [0, 255]
// the system-dependent code is in the function convert_to_display_color
// the function SetCPixel is a Mac OS function
// explained on page 72
// display_pixel() 函数将各个像素的颜色值转换为显示器可支持的颜色。
// 这一过程涉及到3个步骤：色调映射、gamma值修正以及整数映射。
//   
void
World::display_pixel(const int row, const int column, const RGBColor& raw_color) const {

    RGBColor mapped_color;

    if (vp.show_out_of_gamut)
        mapped_color = clamp_to_color(raw_color);
    else
        mapped_color = max_to_one(raw_color);
    if (vp.gamma != 1.0)
        mapped_color = mapped_color.powc(vp.inv_gamma);

    //have to start from max y coordinate to convert to screen coordinates
    int x = column;
    int y = vp.vres - row - 1;

    // paintArea->setPixel(x, y, (int)(mapped_color.r * 255),  // 
    //                          (int)(mapped_color.g * 255),   // 
    //                          (int)(mapped_color.b * 255));  // 
    // cout << (int)(mapped_color.r * 255) << " " 
    //      << (int)(mapped_color.g * 255) << " " << (int)(mapped_color.b * 255) << endl; // 
    out << (int)(mapped_color.r * 255) << " " << (int)(mapped_color.g * 255) << " " << (int)(mapped_color.b * 255) << endl;

}


ShadeRec
World::hit_objects(const Ray& ray) {

    ShadeRec    sr(*this);
    double      t;
    Normal normal;
    Point3D local_hit_point;
    double      tmin            = kHugeValue;
    int         num_objects     = objects.size();

    for (int j = 0; j < num_objects; j++)
    {
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            sr.hit_an_object = true;
            tmin = t;
            //  We can use material now
            // Lujiaming uncomment at 230913
            sr.material_ptr = objects[j]->get_material();
            sr.hit_point = ray.o + t * ray.d;
            normal = sr.normal;
            local_hit_point = sr.local_hit_point;
            sr.color = objects[j]->get_color();
        }
    }

    if(sr.hit_an_object) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return(sr);
}

double randf()
{
    return (double)(rand()/(double)RAND_MAX);
}

// #include "BuildRedSphere.cpp"
void World::build()
{
    //construct view plane， integrator， camera， and lights
    // int num_spheres=100000;
    int num_samples = 1;
    vp.set_hres(600) ;
    vp.set_vres(400) ;
    vp.set_samples(num_samples) ;
    vp.set_max_depth(10) ;
/*    
    tracer_ptr=new Whitted(this) ;
    Reflective* reflective_ptr1=new Reflective;
    reflective_ptr1->set_ka(0.25) ;
    reflective_ptr1->set_kd(0.5) ;
    reflective_ptr1->set_cd(0.75, 0.75, 0) ;
    reflective_ptr1->set_ks(0.15) ;
    reflective_ptr1->set_exp(100) ;
    reflective_ptr1->set_kr(0.75) ;
    reflective_ptr1->set_cr(white) ;
  */  
    tracer_ptr=new Whitted(this) ;
    Reflective* reflective_ptr1=new Reflective;
    reflective_ptr1->set_ka(0.0) ;
    reflective_ptr1->set_kd(0.0) ;
    reflective_ptr1->set_cd(0.75, 0.75, 0) ;
    reflective_ptr1->set_ks(0.0) ;
    reflective_ptr1->set_exp(100) ;
    reflective_ptr1->set_kr(0.75) ;
    reflective_ptr1->set_cr(blue) ;

    Phong * phong_ptr = new Phong;
    phong_ptr->set_ka(0.25);
    phong_ptr->set_kd(0.65);
    phong_ptr->set_cd(1, 1, 0);

    Sphere *sphere_ptr = new Sphere;
    sphere_ptr->set_center(-10, -40, 0);
    sphere_ptr->set_radius(100.0);
    sphere_ptr->set_color(1.0, 0.0, 0.0);
    // sphere_ptr->set_material(phong_ptr);
    sphere_ptr->set_material(reflective_ptr1);
    add_object(sphere_ptr);

    sphere_ptr = new Sphere;
    sphere_ptr->set_center(0, 60, 0);
    sphere_ptr->set_radius(80.0);
    sphere_ptr->set_color(1.0, 1.0, 0.0);
    // sphere_ptr->set_material(phong_ptr);
    sphere_ptr->set_material(reflective_ptr1);
    add_object(sphere_ptr);

    Phong * phong_plane_ptr = new Phong;
    phong_plane_ptr->set_ka(0.5);
    phong_plane_ptr->set_kd(0.35);
    phong_plane_ptr->set_cd(0.5, 0.5, 0);


    Plane *plane_ptr = new Plane;
    plane_ptr->a = Vector3D(0.0);
    plane_ptr->n = Vector3D(0.6, 0.3, 0.7);
    plane_ptr->set_color(0.0, 1.0, 0.0);
    plane_ptr->set_material(phong_ptr);
    add_object(plane_ptr);

    PointLight *light_ptr=new PointLight();
    light_ptr->set_location(50, 50, 1);
    light_ptr->scale_radiance(3.0);
    add_light(light_ptr);

    Emissive*emissive_ptr=new Emissive;
    emissive_ptr->set_ce(1.0, 1.0, 0.5);
    // emissive_ptr->set_brightness(1.0) ;

    EnvironmentLight * environment_light_ptr=new EnvironmentLight;
    environment_light_ptr->set_material(emissive_ptr);
    environment_light_ptr->set_sampler(new MultiJittered(num_samples) ) ;
    environment_light_ptr->set_shadows(true);
    add_light(environment_light_ptr) ;




    // 设定相机
    Pinhole* pinhole_ptr = new Pinhole;
    pinhole_ptr->set_eye(0, 0, 500);
    pinhole_ptr->set_lookat(0, 0, -50);
    pinhole_ptr->set_view_distance(400);
    pinhole_ptr->compute_uvw();
    set_camera(pinhole_ptr);

}

