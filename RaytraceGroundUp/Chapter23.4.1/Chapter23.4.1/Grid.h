#ifndef __GRID__
#define __GRID__


// This contains the declaration of the class Grid.
// The copy constructor, assignment operator, and destructor, are dummy functions.
// The reason is that a grid can contain an arbitrary number of objects, can therefore be of arbitrary size.

// There is no set_bounding_box function because the bounding box is constructed in the setup_cells
// function.
// This Grid class is also way too large. All the triangle and PLY file code should be placed in
// a separate class called TriangleMesh that inherits from Grid.


#include "Compound.h"
#include "ShadeRec.h"
#include "Mesh.h"
#include "BBox.h"

//---------------------------------------------------------------------- class Grid
// 栅格也是一类通用对象，因而可像其他对象一样进行相交测试计算。
// 同时，栅格还包含一项重要的建模特征， 即栅格可实现嵌套构造。
// 另外，栅格对象也继承于Compound类， 原因包括以下两点：
//     首先，栅格对象由多个组件构成，因而也是一类组合对象；
//     其次，当向栅格对象中添加其他对象时，在建立栅格单元之前需要临时存储这些对象，
//     Compound::objects数据结构非常适用于这一情形，并适当地简化了栅格的实现过程。
//     例如，Compound类可将栅格中的各个对象赋予同一材质。
class Grid: public Compound {                                              
    public:

        Grid(void); 

        Grid(Mesh* _mesh_ptr);                                            

        virtual Grid*                                         
        clone(void) const;

        Grid(const Grid& rg);         

        Grid&                                 
        operator= (const Grid& rhs);    

        virtual                                         
        ~Grid(void);               

        virtual BBox 
        get_bounding_box(void);

        void                                                
        read_flat_triangles(const char* file_name);

        void                                                
        read_smooth_triangles(const char* file_name);

        void                                                
        tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps);

        void                                                
        tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps);

        virtual bool 
        hit(const Ray& ray, double& tmin, ShadeRec& sr) const;

        void
        setup_cells(void);

        void
        reverse_mesh_normals(void);

        void
        store_material(Material* material, const int index);                             

    private: 
        // 使用一维数组对象指针数组cells模拟栅格单元。
        vector<GeometricObject*>    cells;             // grid of cells
        // xw、yw、zw方向上的栅格单元数量。
        int                         nx, ny, nz;        // number of cells in the x, y, and z directions
        // 所有对象的包围盒。
        BBox                        bbox;              // bounding box
        Mesh*                       mesh_ptr;          // holds triangle data
        bool                        reverse_normal;    // some PLY files have normals that point inwards

        Point3D
        find_min_bounds(void);

        Point3D
        find_max_bounds(void);

        void
        read_ply_file(const char* file_name, const int triangle_type);

        void
        compute_mesh_normals(void);    
};


// ------------------------------------------------------------------------------ store_material

inline void
Grid::reverse_mesh_normals(void) {   
    reverse_normal = true;
}


// ------------------------------------------------------------------------------ store_material
// stores the material in the specified object
// this is called from the Rosette and Archway classes, which inherit from Grid

inline void
Grid::store_material(Material* material_ptr, const int index) {
    objects[index]->set_material(material_ptr); 
}

#endif









