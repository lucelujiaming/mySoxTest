#ifndef __WORLD__
#define __WORLD__

// This file contains the declaration of the class World
// The World class does not have a copy constructor or an assignment operator, for the followign reasons:

// 1 	There's no need to copy construct or assign the World
// 2 	We wouldn't want to do this anyway, because the world can contain an arbitray amount of data
// 3 	These operations wouldn't work because the world is self-referencing:
//	 	the Tracer base class contains a pointer to the world. If we wrote a correct copy constructor for the
// 	  	Tracer class, the World copy construtor would call itself recursively until we ran out of memory.

#include "ViewPlane.h"
#include "RGBColor.h"
#include "GeometricObject.h"
#include "Sphere.h"
#include "Tracer.h"
#include "Plane.h"

#include "Camera.h"
#include "Light.h"
#include "Ambient.h"

class World {
	public:
        ViewPlane                   vp;
        Camera*                     camera_ptr;
        Tracer*                     tracer_ptr;
        RGBColor                    background_color;
        Light*                      ambient_ptr;
        vector<GeometricObject*>    objects;
        vector<Light*>              lights;

		World(void);
        ~World();

        void
        add_object(GeometricObject* object_ptr);

		void
			add_light(Light* light_ptr);

        void set_ambient_light(Light* light_ptr);

        void set_camera(Camera* c_ptr);

		void
		build(void);

		void
		render_scene(void) const;

        void render_perspective(void) const;

//        void open_window(const int vRes, const int hRes) const;

        RGBColor
        max_to_one(const RGBColor& c) const;

        RGBColor
        clamp_to_color(const RGBColor& c) const;
		void
		display_pixel(const int row, const int column, const RGBColor& pixel_color) const;

        ShadeRec
                hit_objects(const Ray& ray);
};

inline void
World::add_object(GeometricObject* object_ptr) {
        objects.push_back(object_ptr);
}

// ------------------------------------------------------------------ add_light

inline void
World::add_light(Light* light_ptr) {
	lights.push_back(light_ptr);
}

inline void
World::set_camera(Camera* c_ptr) {
        camera_ptr = c_ptr;
}

inline void
World::set_ambient_light(Light* light_ptr) {
        ambient_ptr = light_ptr;
}

#endif