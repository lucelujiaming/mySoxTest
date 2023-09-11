#include "stdafx.h"
// this file contains the definition of the World class

// #include "wxraytracer.h"

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
#include "RGBColor.h"
#include "Sphere.h"

#include <fstream>
#include <iostream>
using namespace std;
extern ofstream out;


// -------------------------------------------------------------------- default constructor

// tracer_ptr is set to NULL because the build functions will always construct the appropriate tracer
// ambient_ptr is set to a default ambient light because this will do for most scenes
// camera_ptr is set to NULL because the build functions will always have to construct a camera
// and set its parameters

World::World(void)
	:  	background_color(black),
		tracer_ptr(NULL)
{}

World::~World(void) {

    if(tracer_ptr) {
        delete tracer_ptr;
        tracer_ptr = NULL;
    }
    
}

//------------------------------------------------------------------ render_scene

// This uses orthographic viewing along the zw axis

void World::render_scene() const
{
    RGBColor pixel_color;
    Ray ray;
    double zw = 200.0;
    double x, y;

    ray.d = Vector3D(0, 0, -1);
    for (int r = 0; r < vp.vres; r++) {
        for (int c = 0; c < vp.hres; c++) {
            pixel_color = background_color;
            x = vp.s * (c - 0.5 * (vp.hres - 1.0));
            y = vp.s * (r - 0.5 * (vp.vres - 1.0));
            ray.o = Point3D(x, y, zw);
            pixel_color = tracer_ptr->trace_ray(ray);
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

    printf("%d %d %d\n", (int)(mapped_color.r * 255), (int)(mapped_color.g * 255), (int)(mapped_color.b * 255));
	// paintArea->setPixel(x, y, (int)(mapped_color.r * 255),
    //                          (int)(mapped_color.g * 255),
    //                          (int)(mapped_color.b * 255));
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
        if (objects[j]->hit(ray, t, sr) && (t < tmin)) {
            sr.hit_an_object    = true;
            tmin                = t;
            // sr.material_ptr     = objects[j]->get_material();
            sr.hit_point        = ray.o + t * ray.d;
            normal              = sr.normal;
			local_hit_point = sr.local_hit_point;
			sr.color = objects[j]->get_color();
        }

    if(sr.hit_an_object) {
        sr.t = tmin;
        sr.normal = normal;
        sr.local_hit_point = local_hit_point;
    }

    return(sr);
}

// #include "BuildRedSphere.cpp"
void World::build()
{
   //vp.set_hres(200);
   //vp.set_vres(200);
   //vp.set_pixel_size(1.0);
   //vp.set_gamma(1.0);
   //background_color = black;
   //tracer_ptr = new MultipleObjects(this);
   //Sphere * sphere = new Sphere;
   //sphere->set_center(100,100,30);
   //sphere->set_radius(85.0);
   //sphere->set_color(RGBColor(1,0,0));
   //add_object(sphere);

    vp.set_hres(400);
    vp.set_vres(400);
    vp.set_pixel_size(1);
    vp.set_gamma(1.0);
    background_color = blue;

    //
    //tracer_ptr = new SingleSphere(this);
    //sphere.set_center(0.0);
    //sphere.set_radius(200);

    tracer_ptr = new MultipleObjects(this);
    Sphere *sphere_ptr = new Sphere;
    sphere_ptr->set_center(-10, -40, 0);
    sphere_ptr->set_radius(100.0);
    sphere_ptr->set_color(1.0, 0.0, 0.0);
    add_object(sphere_ptr);

//  sphere_ptr = new Sphere(Vector3D(0, 60, 0), 80.0);

    sphere_ptr = new Sphere;
    sphere_ptr->set_center(0, 60, 0);
    sphere_ptr->set_radius(80.0);
    sphere_ptr->set_color(1.0, 1.0, 0.0);
    add_object(sphere_ptr);
}

