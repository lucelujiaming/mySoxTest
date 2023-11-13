#ifndef __WORLD__
#define __WORLD__

// This file contains the declaration of the class World
// The World class does not have a copy constructor or an assignment operator, for the followign reasons:

// 1     There's no need to copy construct or assign the World
// 2     We wouldn't want to do this anyway, because the world can contain an arbitray amount of data
// 3     These operations wouldn't work because the world is self-referencing:
//         the Tracer base class contains a pointer to the world. If we wrote a correct copy constructor for the
//           Tracer class, the World copy construtor would call itself recursively until we ran out of memory.

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
        // 环境光照直接存储在World类中。
        // 注意：环境光照指针采用了Light指针，而并非Ambient指针，以适应各种环境光照。
        Light*                      ambient_ptr;
        // 使用一个特定的数据结构存储几何对象
        vector<GeometricObject*>    objects;
        // 使用一个特定的数据结构存储World中的光照。
        vector<Light*>              lights;

        World(void);
            ~World();
        // 向当前场景添加对象的函数
        void
        add_object(GeometricObject* object_ptr);

        void
            add_light(Light* light_ptr);

        void set_ambient_light(Light* light_ptr);

        void set_camera(Camera* c_ptr);

        void
        build(char * file_name);

        void
        render_scene(void) const;

        void render_perspective(void) const;

        // void open_window(const int vRes, const int hRes) const;

        RGBColor
        max_to_one(const RGBColor& c) const;

        RGBColor
        clamp_to_color(const RGBColor& c) const;
        void
        display_pixel(const int row, const int column, const RGBColor& pixel_color) const;

        ShadeRec hit_bare_bones_objects(const Ray& ray) const;
        // 在光线和全部对象之间进行相交测试的函数。
        ShadeRec
        hit_objects(const Ray& ray);
	private:
		void add_Phong_axis(bool bUseScale = false);
		void set_back_plane();
		void add_bunnies();
		void create_cornell_box(int iWidth, int iHeight);
};

// 若光线跟踪器只能渲染单一球体，则该光线跟踪器不具备太多的实用价值。
// 因此，需要添加相关内容以对任意数量且不同类型的对象进行光线跟踪计算。
// 这里，将使用vector存储几何对象，并增加一个向当前场景添加对象的函数。
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
        // 好像存在Memory Leak
        // 应该是： if(ambient_ptr) delete ambient_ptr
        ambient_ptr = light_ptr;
}

#endif
