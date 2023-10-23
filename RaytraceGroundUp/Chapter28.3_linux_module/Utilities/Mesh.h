#ifndef __MESH__
#define __MESH__

// A mesh object stores the data for mesh triangles:
//
//            vertices
//            normals
//            uv texture coordinates
//            lists of triangles that share each vertex
//
// Mesh triangles are stored in a grid, which has a pointer to the mesh
// Each mesh triangle also has a pointer to the mesh
// The Mesh class does not inherit from GeometricObject

#include <vector>
using std::vector;

#include "Point3D.h"
#include "Normal.h"

// 由于顶点可能会被多个三角形共享，因而存储每一个三角形的顶点和法线数据将会浪费大量的内存空间。
// 实际上，对于包含大量三角形的任意封闭型三角形网格来说，
// 每个顶点平均被6个三角形共享，该值可通过对象表面的Euler特征证明。
// 可在某种辅助数据结构中一次性存储各顶点的坐标值以及法线数据，来达到节省内存空间的目的。
// 在此，只存储各三角形的一条法线数据，以在场景的构建过程中节省计算时间。这一类数据结构称作三角形网格。
// 三角形网格可采用Mesh数据结构以及一系列协同工作的网格三角形加以实现。
// 其中，Mesh类存储了顶点数据信息，且全部数据成员均定义为public类型以方便访问。
class Mesh {
    public:
        vector<Point3D>         vertices;                 // 顶点列表：mesh vertices
        vector<Normal>          normals;                  // 每一个顶点的法线方向列表：average normal at each vertex;
        vector<float>           u;                        // 纹理坐标：u texture coordinate at each vertex
        vector<float>           v;                        // 纹理坐标：v texture coordinate at each vertex
        vector<vector<int> >    vertex_faces;             // 由三个顶点组成的三角形列表：the triangles shared by each vertex
        int                     num_vertices;             // 顶点个数：number of vertices
        int                     num_triangles;            // 三角形个数：number of triangles

        Mesh(void);
        Mesh(const Mesh& m);
        ~Mesh(void);
        Mesh&
        operator= (const Mesh& rhs);
};

#endif


