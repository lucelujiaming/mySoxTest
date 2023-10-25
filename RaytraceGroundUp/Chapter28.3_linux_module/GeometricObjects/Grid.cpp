// This file contains the definition of the Grid class

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <math.h>
using std::vector;
using namespace std;

#include "Constants.h"

#include "Vector3D.h"
#include "Point3D.h"
#include "Grid.h"
#include "Maths.h"

#include "MeshTriangle.h"
#include "FlatMeshTriangle.h"
#include "SmoothMeshTriangle.h"
//#include "FlatUVMeshTriangle.h"
//#include "SmoothUVMeshTriangle.h"

#include "Triangle.h"
#include "SmoothTriangle.h"

#include "ply.h"

typedef enum {
    flat,
    smooth
} TriangleType;


// ----------------------------------------------------------------  default constructor

Grid::Grid(void)
    :     Compound(),
        nx(0),
        ny(0),
        nz(0),
        mesh_ptr(new Mesh),
        reverse_normal(false)
{
    // The cells array will be empty
}


// ----------------------------------------------------------------  constructor
// for rendering triangle meshes

Grid::Grid(Mesh* _mesh_ptr)
    :     Compound(),
        nx(0),
        ny(0),
        nz(0),
        mesh_ptr(_mesh_ptr),
        reverse_normal(false)
{
    // The cells array will be empty
}

// ---------------------------------------------------------------- clone

Grid*
Grid::clone(void) const {
    return (new Grid (*this));
}


// ---------------------------------------------------------------- copy constructor
// not implemented

Grid::Grid(const Grid& grid) {}


// ---------------------------------------------------------------- assignment operator
// not implemented

Grid&
Grid::operator= (const Grid& rhs)    {
    return (*this);
}



// ---------------------------------------------------------------- destructor
// not implemented

Grid::~Grid(void) {}


BBox
Grid::get_bounding_box(void) {
    return (bbox);
}

//------------------------------------------------------------------ setup_cells

void
Grid::setup_cells(void) {
    // find the minimum and maximum coordinates of the grid
    // 计算顶点p0和p1
    Point3D p0 = find_min_bounds();
    Point3D p1 = find_max_bounds();
    // 第一步：设定所有对象的包围盒。
    bbox.x0 = p0.x;
    bbox.y0 = p0.y;
    bbox.z0 = p0.z;
    bbox.x1 = p1.x;
    bbox.y1 = p1.y;
    bbox.z1 = p1.z;
    // 第二步：将栅格划分为栅格单元。
    // compute the number of grid cells in the x, y, and z directions
    // n为栅格中的置入对象数量。
    int num_objects = objects.size();

    // dimensions of the grid in the x, y, and z directions
    // 下面Wx、wy、Wz和nx、ny、nz的计算方法参见公式22.1
    double wx = p1.x - p0.x;
    double wy = p1.y - p0.y;
    double wz = p1.z - p0.z;
    // m表示一个放大系数，从而保证栅格单元的数量可实现正确的变化。
    // 通常情况下，栅格单元的数量可以满足该条件——则全部栅格单元的数量nx ny nz近似为m * n。
    // 当m=1时，栅格单元的数量近似等于对象数量，但该值并非最佳值。
    // 需要注意的是，若栅格单元的数量较多，光线跟踪器将会消耗过多的时间用于在空栅格单元中执行相交测试；
    // 但如果栅格单元的数量过少，又将会导致过量的对象相交测试。
    // 从本质上讲，在栅格单元的极限条件下(即nx=ny=nz=1)，栅格已无存在的必要。
    // 当然，最终也无法获取相应的加速方案。
    // 针对于此，当栅格单元的数量8~10倍于对象数量时，栅格对象将具有较好的结构，其计算性能也为最优。
    // 默认情况下，将该倍数值设置为8，而其对应的m值为2。
    // 此处，读者可尝试使用不同的m值并比较最终的渲染速度(参见练习22.5和22.6)。
    double multiplier = 2.0;      // multiplyer scales the number of grid cells relative to the number of objects
    double s = pow(wx * wy * wz / num_objects, 0.3333333);
    // 令nx、ny、nz为xw、yw、zw方向上的栅格单元数量。
    // 等式中的“+1”项确保栅格单元在任意方向上的数量不为0值。
    nx = multiplier * wx / s + 1;
    ny = multiplier * wy / s + 1;
    nz = multiplier * wz / s + 1;

    // set up the array of grid cells with null pointers
    // 第三步：讨论索引数组问题。
    // cells表示一个长度为(nx, ny, nz)的一维数组。
    // (ix, iy, iz)处的栅格单元的索引可采用下式进行计算
    //    index = ix + nx * iy + nx * ny * iz
    // 同时，这也是C/C++语言中在1D数组中存储3D数据结构的标准方法。
    int num_cells = nx * ny * nz;
    cells.reserve(num_objects);

    for (int j = 0; j < num_cells; j++)
        cells.push_back(NULL);

    // set up a temporary array to hold the number of objects stored in each cell
    // 声明一个临时变量，记录各个栅格单元中的对象数量。
    vector<int> counts;
    counts.reserve(num_cells);
    for (int j = 0; j < num_cells; j++)
        counts.push_back(0);

    // put the objects into the cells
    BBox obj_bBox;     // object's bounding box
    int index;      // cell's array index

    // 最后一个步骤：将各个对象置入栅格单元中，首先针对每一个几何对象。
    for (int j = 0; j < num_objects; j++) {
        // 获得每一个几何对象的包围盒。
        obj_bBox =  objects[j]->get_bounding_box();

        // compute the cell indices at the corners of the bounding box of the object
        // 为了确定栅格单元的对象添加位置，将首先计算包含对象包围盒最大、最小坐标值的栅格单元。
        // 参见图22.4和公式22.4。根据公式22.4可知，这是一个生成基于整数值的步进函数。
        int ixmin = clamp((obj_bBox.x0 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
        int iymin = clamp((obj_bBox.y0 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
        int izmin = clamp((obj_bBox.z0 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
        int ixmax = clamp((obj_bBox.x1 - p0.x) * nx / (p1.x - p0.x), 0, nx - 1);
        int iymax = clamp((obj_bBox.y1 - p0.y) * ny / (p1.y - p0.y), 0, ny - 1);
        int izmax = clamp((obj_bBox.z1 - p0.z) * nz / (p1.z - p0.z), 0, nz - 1);
        // 遍历当前这个几何对象的包围盒跨越的每一个栅格，
        // add the object to the cells
        for (int iz = izmin; iz <= izmax; iz++)                     // cells in z direction
        {
            for (int iy = iymin; iy <= iymax; iy++)                    // cells in y direction
            {
                for (int ix = ixmin; ix <= ixmax; ix++) {            // cells in x direction
                    // 根据公式22.3计算出(ix, iy, iz)处对应的栅格单元cells和counts索引。
                    index = ix + nx * iy + nx * ny * iz;
                    // 如果栅格中没有包含对象，
                    if (counts[index] == 0) {
                        // 把对象直接放入cells中。
                        cells[index] = objects[j];
                        // 增加栅格单元中的对象计数值。
                        counts[index] += 1;                          // now = 1
                    }
                    // 如果栅格中已经有对象，
                    else {
                        // 如果栅格中有一个对象，
                        if (counts[index] == 1) {
                            // 创建一个Compound对象，
                            Compound* compound_ptr = new Compound;    // construct a compound object
                            // 把栅格中之前保存的对象放进去，
                            compound_ptr->add_object(cells[index]); // add object already in cell
                            // 把新发现的对象放进去。
                            compound_ptr->add_object(objects[j]);      // add the new object
                            // 把新创建的Compound对象放入cells中。
                            cells[index] = compound_ptr;            // store compound in current cell
                            // 增加栅格单元中的对象计数值。
                            counts[index] += 1;                      // now = 2
                        }
                        // 如果栅格中有多个对象，说明现在栅格中包含的是Compound对象，
                        else {                                        // counts[index] > 1
                            // 把新发现的对象放进Compound对象。
                            cells[index]->add_object(objects[j]);    // just add current object
                            // 增加栅格单元中的对象计数值。
                            counts[index] += 1;                        // for statistics only
                        }
                    }
                }
            }
        }
    }  // end of for (int j = 0; j < num_objects; j++)


    // erase the Compound::vector that stores the object pointers, but don't delete the objects
    // 我们已经成功把每一个子对象放入了栅格中，我们已经不在需要这个子对象列表了。
    // 我们选择清空子对象列表。
    objects.erase (objects.begin(), objects.end());


    // display some statistics on counts
    // this is useful for finding out how many cells have no objects, one object, etc
    // comment this out if you don't want to use it
    // 打印统计信息。看看有哪些栅格没有对象，那些栅格只有一个对象或多个对象。
    int num_zeroes     = 0;
    int num_ones     = 0;
    int num_twos     = 0;
    int num_threes     = 0;
    int num_greater = 0;

    for (int j = 0; j < num_cells; j++) {
        if (counts[j] == 0)
            num_zeroes += 1;
        if (counts[j] == 1)
            num_ones += 1;
        if (counts[j] == 2)
            num_twos += 1;
        if (counts[j] == 3)
            num_threes += 1;
        if (counts[j] > 3)
            num_greater += 1;
    }
    cout << "num_cells =" << num_cells << endl;
    cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << endl;
    cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << endl;

    // erase the temporary counts vector
    // 清空栅格对象计数列表。
    counts.erase (counts.begin(), counts.end());
}


//------------------------------------------------------------------ find_min_bounds

// find the minimum grid coordinates, based on the bounding boxes of all the objects
// 计算顶点p0和p1较简单。
// 函数由setup_cells() 函数负责调用。需要注意的是，顶点p0和p1并不存储于栅格对象中。
Point3D
Grid::find_min_bounds(void) {
    BBox     object_box;
    Point3D p0(kHugeValue);

    int num_objects = objects.size();
    // 计算各对象包围盒的并集。
    for (int j = 0; j < num_objects; j++) {
        object_box = objects[j]->get_bounding_box();
        // 得到所有对象的XYZ坐标的最小值。
        if (object_box.x0 < p0.x)
            p0.x = object_box.x0;
        if (object_box.y0 < p0.y)
            p0.y = object_box.y0;
        if (object_box.z0 < p0.z)
            p0.z = object_box.z0;
    }
    // 顶点p0将减去一个kEpsilon值，以使栅格稍大于各对象包围盒的并集。
    p0.x -= kEpsilon; p0.y -= kEpsilon; p0.z -= kEpsilon;

    return (p0);
}


//------------------------------------------------------------------ find_max_bounds

// find the maximum grid coordinates, based on the bounding boxes of the objects
// 计算顶点p0和p1较简单
// 函数由setup_cells() 函数负责调用。需要注意的是，顶点p0和p1并不存储于栅格对象中。
Point3D
Grid::find_max_bounds(void) {
    BBox object_box;
    Point3D p1(-kHugeValue);

    int num_objects = objects.size();
    // 计算各对象包围盒的并集。
    for (int j = 0; j < num_objects; j++) {
        object_box = objects[j]->get_bounding_box();
        // 得到所有对象的XYZ坐标的最大值。
        if (object_box.x1 > p1.x)
            p1.x = object_box.x1;
        if (object_box.y1 > p1.y)
            p1.y = object_box.y1;
        if (object_box.z1 > p1.z)
            p1.z = object_box.z1;
    }
    // 顶点p0将加上一个kEpsilon值，以使栅格稍大于各对象包围盒的并集。
    p1.x += kEpsilon; p1.y += kEpsilon; p1.z += kEpsilon;

    return (p1);
}


// The following functions read a file in PLY format, and construct mesh triangles where the data is stored
// in the mesh object
// They are just small wrapper functions that call the functions read_ply_file or read_uv_ply_file that
// do the actual reading
// These use the PLY code by Greg Turk to read the PLY file


// ----------------------------------------------------------------------------- read_flat_triangles
// 读取PLY文件，进行固定着色。
void
Grid::read_flat_triangles(const char* file_name) {
      read_ply_file(file_name, flat);       // 读取PLY文件。
 }


// ----------------------------------------------------------------------------- read_smooth_triangles
// 读取PLY文件，进行平滑着色。
void
Grid::read_smooth_triangles(const char* file_name) {
      read_ply_file(file_name, smooth);     // 读取PLY文件。
      compute_mesh_normals();               // 计算法线方向。
}

// ----------------------------------------------------------------------------- read_ply_file

// Most of this function was written by Greg Turk and is released under the licence agreement
// at the start of the PLY.h and PLY.c files
// The PLY.h file is #included at the start of this file
// It still has some of his printf statements for debugging
// I've made changes to construct mesh triangles and store them in the grid
// mesh_ptr is a data member of Grid
// objects is a data member of Compound
// triangle_type is either flat or smooth
// Using the one function construct to flat and smooth triangles saves a lot of repeated code
// The ply file is the same for flat and smooth triangles
// ply文件下载地址：
//    https://www.cc.gatech.edu/projects/large_models/
/***************************************************************************************************/
/*  注意：                                                                                         */
/*    PLY文件作为一种文件格式。除了基本的数据类型和一个list类型以外，没有内置任何其他数据结构。    */
/*    下面PLY文件中的元素类型vertex，元素类型face，还有vertex_indices都不是PLY文件的内置类型。     */
/*    而是由使用者自行定义的数据结构。而是由使用者自行定义的数据结构。                             */
/*    由read_ply_file函数中的结构体和局部变量定义。每一个字段的含义参见代码中的注释。              */
/*    例如：                                                                                       */
/*        元素类型vertex由read_ply_file函数中的局部变量vert_props定义。                            */
/*        在局部变量face_props中定义了三个名字叫x, y, z的属性。                                    */
/*        元素类型face由read_ply_file函数中的局部变量face_props定义。                              */
/*        在局部变量face_props中定义一个名字叫vertex_indices的属性。                               */
/*    也就是说，下面这个PLY文件和read_ply_file函数是对应的。                                       */
/*    其他的PLY文件，read_ply_file函数不一定可以正确读取。或者是说，几乎不可能正确读取。           */
/*    虽然PLY文件中会存在各种各样的自定义数据。但是通常来说，必然会存在两种数据，就是顶点和面。    */
/***************************************************************************************************/
/*  下面是一个包含了两个三角形的ply文件例子。                                                      */
/*  这个例子里面的数据的含义如下：                                                                 */
/*        ply                                       // 第一行为ply。                               */
/*        format ascii 1.0                          // 第二行定义文件类型和版本号。                */
/*        comment author: Kevin Suffern             // 注释行以comment开头。                       */
/*        comment this file contains two triangles  //                                             */
/*        element vertex 4                          // 使用关键字element定义一个元素类型vertex     */
/*                                                  // 包含4个元素。每一个元素有三个属性。         */
/*        property float x                          // 第一个属性的名字叫X，类型为float。          */
/*        property float y                          // 第二个属性的名字叫Y，类型为float。          */
/*        property float z                          // 第三个属性的名字叫Z，类型为float。          */
/*        element face 2                            // 使用关键字element定义一个元素类型face       */
/*                                                  // 包含2个元素。每一个元素有三个属性。         */
/*        property list int int vertex_indices      // 元素类型face只有一个属性值。                */
/*                                                  // 但是这个属性值被定义为一个表list。          */
/*                                                  // list关键字后面跟了两个类型参数。            */
/*                                                  // 第一个int表明索引的数量为整数类型           */
/*                                                  // 第二个int表明索引的内容为整数类型           */
/*                                                  // 属性的名字叫vertex_indices                  */
/*                                                  // 的数据后面跟着多个整数。                    */
/*                                                  // 内容是vertex的下标。                        */
/*        end_header                                // 文件头结束。                                */
/*        -1.0 0.0 -1.0                             // 4个元素类型vertex的数据。                   */
/*        -1.0 -1.0 1.0                             //                                             */
/*        1.0 0.0 1.0                               //                                             */
/*        1.0 -1.0 -1.0                             //                                             */
/*        3 0 1 2                                   // 两个元素类型face的数据。                    */
/*        3 0 2 3                                   // 表明一个三角形由0 1 2号vertex组成。         */
/*                                                  // 另一个三角形由0 2 3号vertex组成。           */
/***************************************************************************************************/
/*  函数功能：                                                                                     */
/*    针对某一特定的三角形数据类型，着色类型（固定着色或平滑着色）                                 */
/*    并根据所提供的函数参数进行相应的读取操作。                                                   */
/***************************************************************************************************/
void
Grid::read_ply_file(const char* file_name, const int triangle_type) {
    // Vertex definition
    // 定义Vertex类型，包括有三个属性。分别为XYZ坐标。
    typedef struct Vertex {
      float x,y,z;      // space coordinates
    } Vertex;

    // Face definition. This is the same for all files but is placed here to keep all the definitions together
    // 定义Face类型，包括有三个属性。第一个属性是一个字符，表明索引的数量，之后是一个整数列表。
    typedef struct Face {
          unsigned char nverts;    // number of vertex indices in list
          int* verts;              // vertex index list
    } Face;

    // list of property information for a vertex
    // this varies depending on what you are reading from the file
    // 定义Vertex类型的属性信息。
    // Vertex类型包括三个属性，名字为"xyz"，
    // 每一个内部和外部的数据类型为FLOAT，数据对应Vertex的XYZ成员变量上。
    // 后面的四个0中，第一个0表示scalar，就是说属性是标量，不是列表。
    // 因此上，再后面的三个0就是没有用了。
    PlyProperty vert_props[] = {
      {"x", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,x), 0, 0, 0, 0},
      {"y", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,y), 0, 0, 0, 0},
      {"z", PLY_FLOAT, PLY_FLOAT, offsetof(Vertex,z), 0, 0, 0, 0}
    };

    // list of property information for a face.
    // there is a single property, which is a list
    // this is the same for all files
    // 定义Face类型的属性信息。
    // Vertex类型包括一个属性，名字为"vertex_indices"，
    // 每一个内部和外部的数据类型为INT，数据对应Face的verts成员变量上。
    // 后面的1表示list，就是说属性是列表。
    // 后面的第一个PLY_UCHAR表示这个列表个数的数据类型为UCHAR。
    // 后面的第二个PLY_UCHAR表示这个列表元素的数据类型为UCHAR。
    // 列表数据对应Face的nverts成员变量上。
    PlyProperty face_props[] = {
          {"vertex_indices", PLY_INT, PLY_INT, offsetof(Face,verts),
               1, PLY_UCHAR, PLY_UCHAR, offsetof(Face,nverts)}
    };

    // local variables

    int             i,j;
    PlyFile*        ply;
    int             nelems;        // number of element types: 2 in our case - vertices and faces
    char**          elist;
    int             file_type;
    float           version;
    int             nprops;        // number of properties each element has
    int             num_elems;     // number of each type of element: number of vertices or number of faces
    PlyProperty**   plist;
    Vertex**        vlist;
    Face**          flist;
    char*           elem_name;
    int             num_comments;
    char**          comments;
    int             num_obj_info;
    char**          obj_info;


    // open a ply file for reading
    // 打开PLY文件。输入一个文件名。返回四个变量。
    //   nelems     - element个数。这里应该是2。
    //   elem_names - 一个字符串指针数组。每一个元素都是一个字符串指针。给出了元素的名字。
    //                当然在这个函数中，名字只有两种情况。就是："vertex"和"face"。
    //                这里应该是：{"vertex", "face"}。
    //   file_type  - 文件头中的文件类型信息。
    //   version    - 文件头中的版本信息。
    ply = ply_open_for_reading(const_cast<char*>(file_name), &nelems, &elist, &file_type, &version);
    if (ply == NULL)
    {
        printf("read_ply_file::ply == NULL\n");
        return;
    }
    // print what we found out about the file
    // 打印PLY文件头信息
    printf ("version %f\n", version);
    printf ("type %d\n", file_type);

    // go through each kind of element that we learned is in the file and read them
    // there are only two elements in our files: vertices and face
    // 之后循环每一个元素。
    for (i = 0; i < nelems; i++) {  
        // get the description of the first element
        elem_name = elist[i];
        // 根据元素的名字得到在文件中这个元素有几个，放在num_elems中，
        // 和每一个元素的属性个数，放在nprops中。
        // 因为，我们其实知道每一个元素的属性个数，因此上nprops根本没有使用。
        plist = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

        // print the name of the element, for debugging
        cout << "element name  " << elem_name 
             << "  num elements = " << num_elems 
             << "  num properties =  " << nprops << endl;

        // if we're on vertex elements, read in the properties
        // 如果是"vertex"类型的元素。
        if (equal_strings ("vertex", elem_name)) {
            // set up for getting vertex elements
            // the three properties are the vertex coordinates
            // 获取"vertex"类型的三个属性。
            ply_get_property (ply, elem_name, &vert_props[0]);
            ply_get_property (ply, elem_name, &vert_props[1]);
            ply_get_property (ply, elem_name, &vert_props[2]);

            // reserve mesh elements
            // 在Mesh中为顶点列表分配空间。
            mesh_ptr->num_vertices = num_elems;
            mesh_ptr->vertices.reserve(num_elems);

            // grab all the vertex elements
            // 读取所有的vertex数据。
            for (j = 0; j < num_elems; j++) {
                Vertex* vertex_ptr = new Vertex;
                // grab an element from the file
                // 获得一个元素。
                ply_get_element (ply, (void *) vertex_ptr);
                // 放入顶点列表。
                mesh_ptr->vertices.push_back(Point3D(vertex_ptr->x, vertex_ptr->y, vertex_ptr->z));
                delete vertex_ptr;
            }
        }

        // if we're on face elements, read them in
        // 如果是"face"类型的元素。
        if (equal_strings ("face", elem_name)) {
            // set up for getting face elements
            // 获取"face"类型的一个属性。
            ply_get_property (ply, elem_name, &face_props[0]);   // only one property - a list
            // 在Mesh中指定面表个数，也就是三角形个数。
            mesh_ptr->num_triangles = num_elems;
            objects.reserve(num_elems);  // triangles will be stored in Compound::objects

            // the following code stores the face numbers that are shared by each vertex
            // 在Mesh中为面表分配空间。当然这里的面表是由三个顶点组成的三角形列表。
            mesh_ptr->vertex_faces.reserve(mesh_ptr->num_vertices);
            // 声明一个临时变量，作为一个面。也就是三个顶点组成的三角形。
            vector<int> faceList;
            for (j = 0; j < mesh_ptr->num_vertices; j++)
            {
                // store empty lists so that we can use the [] notation below
                mesh_ptr->vertex_faces.push_back(faceList); 
            }

            // grab all the face elements
            // 读取所有的face数据。
            int count = 0; // the number of faces read
            for (j = 0; j < num_elems; j++) {
                // grab an element from the file
                Face* face_ptr = new Face;
                // 获得一个元素。
                ply_get_element (ply, (void *) face_ptr);

                // construct a mesh triangle of the specified type
                if (triangle_type == flat) {
                    // 使用mesh_ptr和face_ptr中保存的mesh_ptr索引构建固定着色三角形。
                    // 就是利用face_ptr中保存的三个mesh_ptr索引，从mesh_ptr取出对应下标的点创建三角形。
                    // 参见MeshTriangle::shadow_hit函数。
                    FlatMeshTriangle* triangle_ptr = new FlatMeshTriangle(
                            mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
                    // 计算三角形的法线方向。
                    triangle_ptr->compute_normal(reverse_normal);
                    objects.push_back(triangle_ptr);
                }

                if (triangle_type == smooth) {
                    // 使用mesh_ptr和face_ptr中保存的mesh_ptr索引构建平滑着色三角形。
                    // 就是利用face_ptr中保存的三个mesh_ptr索引，从mesh_ptr取出对应下标的点创建三角形。
                    // 参见MeshTriangle::shadow_hit函数。
                    SmoothMeshTriangle* triangle_ptr = new SmoothMeshTriangle(
                            mesh_ptr, face_ptr->verts[0], face_ptr->verts[1], face_ptr->verts[2]);
                    // the "flat triangle" normal is used to compute the average normal at each mesh vertex
                    // 计算三角形的法线方向。
                    triangle_ptr->compute_normal(reverse_normal);    
                    // it's quicker to do it once here, than have to do it on average 6 times in compute_mesh_normals                    
                    objects.push_back(triangle_ptr);                 

                    // the following code stores a list of all faces that share a vertex
                    // it's used for computing the average normal at each vertex in order(num_vertices) time
                    mesh_ptr->vertex_faces[face_ptr->verts[0]].push_back(count);
                    mesh_ptr->vertex_faces[face_ptr->verts[1]].push_back(count);
                    mesh_ptr->vertex_faces[face_ptr->verts[2]].push_back(count);
                    count++;
                }
            }
            // 如果是固定着色三角形。因为后边不需要计算顶点法线方向，因此上可以直接清除面表。
            // 因为三角形的光线追踪计算使用的是顶点而不是面表。
            if (triangle_type == flat)
            {
                mesh_ptr->vertex_faces.erase(mesh_ptr->vertex_faces.begin(), 
                                             mesh_ptr->vertex_faces.end());
            }
        }

        // print out the properties we got, for debugging
        // 打印属性信息。用于调试。
        for (j = 0; j < nprops; j++)
        {
            printf ("property %s\n", plist[j]->name);
        }
    }  // end of for (i = 0; i < nelems; i++)


    // grab and print out the comments in the file
    // 打印comments信息。用于调试。
    comments = ply_get_comments (ply, &num_comments);
    for (i = 0; i < num_comments; i++)
    {
        printf ("comment = '%s'\n", comments[i]);
    }

    // grab and print out the object information
    // 打印PLY对象信息。用于调试。
    obj_info = ply_get_obj_info (ply, &num_obj_info);
    for (i = 0; i < num_obj_info; i++)
    {
        printf ("obj_info = '%s'\n", obj_info[i]);
    }

    // close the ply file
    ply_close (ply);
}



// ----------------------------------------------------------------------------- compute_mesh_normals
// this computes the average normal at each vertex
// the calculation is of order(num_vertices)
// some triangles in ply files are not defined properly
// 计算法线方向。
void
Grid::compute_mesh_normals(void) {
    mesh_ptr->normals.reserve(mesh_ptr->num_vertices);
    // 对于每一个顶点：for each vertex
    for (int index = 0; index < mesh_ptr->num_vertices; index++) {   
        Normal normal;    // 首先假设法线方向为零：is zero at this point
        // 对包含这个顶点的三角形的法线进行求和。
        for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
        {
            normal += objects[mesh_ptr->vertex_faces[index][j]]->get_normal();
        }

        // The following code attempts to avoid (nan, nan, nan) normalised normals when all components = 0
        // 之后把求出来的和做归一化。
        if (normal.x == 0.0 && normal.y == 0.0 && normal.z == 0.0)
            normal.y = 1.0;
        else
            normal.normalize();
        // 结果作为这个顶点的法线方向。
        mesh_ptr->normals.push_back(normal);
    }

    // erase the vertex_faces arrays because we have now finished with them
    // 清除面表。也就是顶点组成的三角形列表。
    // 因为三角形的光线追踪计算使用的是顶点而不是面表。
    for (int index = 0; index < mesh_ptr->num_vertices; index++)
    {
        for (int j = 0; j < mesh_ptr->vertex_faces[index].size(); j++)
        {
            mesh_ptr->vertex_faces[index].erase (mesh_ptr->vertex_faces[index].begin(), mesh_ptr->vertex_faces[index].end());
        }
    }
    mesh_ptr->vertex_faces.erase (mesh_ptr->vertex_faces.begin(), mesh_ptr->vertex_faces.end());

    cout << "finished constructing normals" << endl;
}



// ------------------------------------------------------------------------------------------------  tesselate_flat_sphere
// tesselate a unit sphere into flat triangles that are stored directly in the grid
// 下面，首先介绍通用球体的嵌格操作，该操作体现了基于三角形的曲面的近似处理过程。
// 根据式(2.3)，三角形的生成代码采用了球体的参数表达形式，其中r=1。
// 嵌格参数定义为方位角和极角方向上的三角形数量，分别用m和n表示。
void
Grid::tessellate_flat_sphere(const int horizontal_steps, const int vertical_steps) {
    double pi = 3.1415926535897932384;

    // define the top triangles which all touch the north pole
    // 定义北极点周围的三角形。这些三角形共用一个北极点。
    int k = 1;
    for (int j = 0; j <= horizontal_steps - 1; j++) {
        // define vertices
        Point3D v0(    0, 1, 0);                                                                        // top (north pole)

        Point3D v1(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),             // bottom left
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

        Point3D v2(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),         // bottom  right
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)    );

        Triangle* triangle_ptr = new Triangle(v0, v1, v2);
        objects.push_back(triangle_ptr);
    }

    // define the bottom triangles which all touch the south
    // 定义南极点周围的三角形。这些三角形共用一个南极点。
    k = vertical_steps - 1;
    for (int j = 0; j <= horizontal_steps - 1; j++) {
        // define vertices

        Point3D v0(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),             // top left
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

        Point3D v1(    0, -1, 0);                                                                        // bottom (south pole)

        Point3D v2(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),         // top right
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)    );

        Triangle* triangle_ptr = new Triangle(v0, v1, v2);
        objects.push_back(triangle_ptr);
    }

    //  define the other triangles
    // 定义其他三角形。这些三角形两两组成一个四角面片。
    for (int k = 1; k <= vertical_steps - 2; k++) {
        for (int j = 0; j <= horizontal_steps - 1; j++) {
            // define the first triangle
            // vertices
            Point3D v0(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),                 // bottom left, use k + 1, j
                        cos(pi * (k + 1) / vertical_steps),
                        cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            Point3D v1(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),         // bottom  right, use k + 1, j + 1
                        cos(pi * (k + 1) / vertical_steps),
                        cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            Point3D v2(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),                     // top left,     use k, j
                        cos(pi * k / vertical_steps),
                        cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

            Triangle* triangle_ptr1 = new Triangle(v0, v1, v2);
            objects.push_back(triangle_ptr1);


            // define the second triangle
            // vertices
            v0 = Point3D(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),             // top right, use k, j + 1
                            cos(pi * k / vertical_steps),
                            cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );

            v1 = Point3D (    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),                 // top left,     use k, j
                            cos(pi * k / vertical_steps),
                            cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

            v2 = Point3D (    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),     // bottom  right, use k + 1, j + 1
                            cos(pi * (k + 1) / vertical_steps),
                            cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            Triangle* triangle_ptr2 = new Triangle(v0, v1, v2);
            objects.push_back(triangle_ptr2);
        }
    }
}


// ------------------------------------------------------------------------------------------------  tesselate_smooth_sphere
// tesselate a unit sphere into smooth triangles that are stored directly in the grid
// 通过对三角形内部碰撞点处的法线进行插值计算，进而可实现不同表面处的三角形着色渲染。最简单的法线差值计算是采用双线性组合方案
// 如果采用此方式渲染三角形，可使球体看上去更加光滑，但这里不再使用Triangle类， 而是执行Smooth Triangle类。
// 该类存储了3条法线以及顶点数据，并根据公式(23.1)计算法线，因此，这将占据更多的内存空间。但出于演示目的，这不会产生太大的问题。
void
Grid::tessellate_smooth_sphere(const int horizontal_steps, const int vertical_steps) {
    double pi = 3.1415926535897932384;

    // define the top triangles
    // 定义北极点周围的三角形。这些三角形共用一个北极点。
    int k = 1;
    for (int j = 0; j <= horizontal_steps - 1; j++) {
        // define vertices
        Point3D v0(    0, 1, 0);
        Point3D v1(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),             // bottom left
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );
        Point3D v2(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),         // bottom  right
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)    );

        SmoothTriangle* triangle_ptr = new SmoothTriangle(v0, v1, v2);
        triangle_ptr->n0 = v0;
        triangle_ptr->n1 = v1;
        triangle_ptr->n2 = v2;
        objects.push_back(triangle_ptr);
    }


    // define the bottom triangles
    // 定义南极点周围的三角形。这些三角形共用一个南极点。
    k = vertical_steps - 1;
    for (int j = 0; j <= horizontal_steps - 1; j++) {
        // define vertices
        Point3D v0(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),             // top left
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );
        Point3D v1(    0, -1, 0);                                                                        // bottom

        Point3D v2(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),         // top right
                    cos(pi * k / vertical_steps),
                    cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps)    );

        SmoothTriangle* triangle_ptr = new SmoothTriangle(v0, v1, v2);
        triangle_ptr->n0 = v0;
        triangle_ptr->n1 = v1;
        triangle_ptr->n2 = v2;
        objects.push_back(triangle_ptr);
    }


    //  define the other triangles
    // 定义其他三角形。这些三角形两两组成一个四角面片。
    for (int k = 1; k <= vertical_steps - 2; k++) {
        for (int j = 0; j <= horizontal_steps - 1; j++) {
            // define the first triangle
            // vertices
            Point3D v0(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),                 // bottom left, use k + 1, j
                        cos(pi * (k + 1) / vertical_steps),
                        cos(2.0 * pi * j / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            Point3D v1(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),         // bottom  right, use k + 1, j + 1
                        cos(pi * (k + 1) / vertical_steps),
                        cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            Point3D v2(    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),                     // top left,     use k, j
                        cos(pi * k / vertical_steps),
                        cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

            SmoothTriangle* triangle_ptr1 = new SmoothTriangle(v0, v1, v2);
            triangle_ptr1->n0 = v0;
            triangle_ptr1->n1 = v1;
            triangle_ptr1->n2 = v2;
            objects.push_back(triangle_ptr1);


            // define the second triangle
            // vertices
            v0 = Point3D(    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps),             // top right, use k, j + 1
                            cos(pi * k / vertical_steps),
                            cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * k / vertical_steps) );

            v1 = Point3D (    sin(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps),                 // top left,     use k, j
                            cos(pi * k / vertical_steps),
                            cos(2.0 * pi * j / horizontal_steps) * sin(pi * k / vertical_steps)    );

            v2 = Point3D (    sin(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps),     // bottom  right, use k + 1, j + 1
                            cos(pi * (k + 1) / vertical_steps),
                            cos(2.0 * pi * (j + 1) / horizontal_steps) * sin(pi * (k + 1) / vertical_steps)    );

            SmoothTriangle* triangle_ptr2 = new SmoothTriangle(v0, v1, v2);
            triangle_ptr2->n0 = v0;
            triangle_ptr2->n1 = v1;
            triangle_ptr2->n2 = v2;
            objects.push_back(triangle_ptr2);
        }
    }
}


// ---------------------------------------------------------------- hit

// The following grid traversal code is based on the pseudo-code in Shirley (2000)
// The first part is the same as the code in BBox::hit
// hit() 函数代码较复杂， 因而有必要详细考察其代码。
// 这个地方书上写的有点问题，下面是原文：
// if the ray misses the grid's bounding box
//    return fa1se
// if the ray starts inside the grid
//    find the ce11 that contains the ray origin
// e1se
//    find the ce11 where the ray
//    hits the grid from the outside
// traverse the grid
// 根据原文，其高层伪代码如下所示：
// Line1. if(光线未与栅格对象的包围盒碰撞)
// Line2.   return false
// Line3. if(如果光线始于栅格对象内部)
// Line4.    计算光线源点所处的栅格单元
// Line5. else
// Line6.   计算光线从外部与栅格对象发生碰撞的栅格单元
// Line7. 遍历栅格对象
bool
Grid::hit(const Ray& ray, double& t, ShadeRec& sr) const {
    // ox、oy、oz表示光线源点坐标。
    double ox = ray.o.x;
    double oy = ray.o.y;
    double oz = ray.o.z;
    double dx = ray.d.x;
    double dy = ray.d.y;
    double dz = ray.d.z;
    // x0，y0，z0，x1，y1，z1表示所有对象组成的包围盒的顶点。
    // 注意：是所有对象组成的包围盒。
    double x0 = bbox.x0;
    double y0 = bbox.y0;
    double z0 = bbox.z0;
    double x1 = bbox.x1;
    double y1 = bbox.y1;
    double z1 = bbox.z1;
    // 光线与包围盒x0，y0，z0，x1，y1，z1碰撞时的t值的最大值和最小值。
    double tx_min, ty_min, tz_min;
    double tx_max, ty_max, tz_max;

    // the following code includes modifications from Shirley and Morley (2003)
    // 因为光线公式d * t + o = p，则：t = (p - o) / d。
    // 计算光线与包围盒碰撞时的tx值的最大值和最小值。
    double a = 1.0 / dx;
    if (a >= 0) {
        tx_min = (x0 - ox) * a;
        tx_max = (x1 - ox) * a;
    }
    else {
        tx_min = (x1 - ox) * a;
        tx_max = (x0 - ox) * a;
    }
    // 计算光线与包围盒碰撞时的ty值的最大值和最小值。
    double b = 1.0 / dy;
    if (b >= 0) {
        ty_min = (y0 - oy) * b;
        ty_max = (y1 - oy) * b;
    }
    else {
        ty_min = (y1 - oy) * b;
        ty_max = (y0 - oy) * b;
    }
    // 计算光线与包围盒碰撞时的tz值的最大值和最小值。
    double c = 1.0 / dz;
    if (c >= 0) {
        tz_min = (z0 - oz) * c;
        tz_max = (z1 - oz) * c;
    }
    else {
        tz_min = (z1 - oz) * c;
        tz_max = (z0 - oz) * c;
    }

    double t0, t1;
    // t0为光线与包围盒碰撞时的t值。
    // tx_min, ty_min, tz_min表明在和包围盒xyz方向的三个平面上碰撞时的t值。
    // 因为和包围盒平面碰撞不等于和包围盒碰撞。
    // 因此上，我们需要取这三个值中的最大值作为实际碰撞值。
    if (tx_min > ty_min)
        t0 = tx_min;
    else
        t0 = ty_min;
    if (tz_min > t0)
        t0 = tz_min;

    // t0为光线离开包围盒时的t值。
    // tx_max, ty_max, tz_max表明在离开包围盒xyz方向的三个平面时的t值。
    // 因为在包围盒平面的时候，我们可能早就离开包围盒了。
    // 因此上，我们需要取这三个值中的最小值作为实际离开值。
    if (tx_max < ty_max)
        t1 = tx_max;
    else
        t1 = ty_max;

    if (tz_max < t1)
        t1 = tz_max;
    // Line1. 如果先离开后碰撞，说明光线未与栅格对象的包围盒碰撞。直接返回false。
    if (t0 > t1)
    {
        // Line2.
        return(false);
    }

    // initial cell coordinates
    // 对于首个遍历栅格单元，其计算过程如下：
    int ix, iy, iz;
    // Line3. 如果是始于栅格内部的光线。
    // 考虑到光线投射的诸多方式， 可以将相机置入栅格对象中，则全部主光线将始于栅格内部。
    // 另外，当对栅格内的对象进行着色计算时，包含任意光线跟踪计算类型的阴影光线也将始于栅格内部。
    // 其中nx, ny, nz是xw、yw、zw方向上的栅格单元数量。
    //     ox、oy、oz表示光线源点坐标。
    //     x0，y0，z0，x1，y1，z1表示栅格包围盒顶点。
    if (bbox.inside(ray.o)) {              // does the ray start inside the grid?
        // Line4. 计算光线起点所在的栅格单元。
        ix = clamp((ox - x0) * nx / (x1 - x0), 0, nx - 1);
        iy = clamp((oy - y0) * ny / (y1 - y0), 0, ny - 1);
        iz = clamp((oz - z0) * nz / (z1 - z0), 0, nz - 1);
    }
    // Line5. 如果是始于栅格外部的光线。
    else {
        // t0为光线与包围盒碰撞时的t值。
        // Line6. 计算光线碰撞点所在的栅格单元。
        Point3D p = ray.o + t0 * ray.d;  // initial hit point with grid's bounding box
        ix = clamp((p.x - x0) * nx / (x1 - x0), 0, nx - 1);
        iy = clamp((p.y - y0) * ny / (y1 - y0), 0, ny - 1);
        iz = clamp((p.z - z0) * nz / (z1 - z0), 0, nz - 1);
    }

    // ray parameter increments per cell in the x, y, and z directions
    // P366. 为了正确实现光线穿越栅格时的步进计算，下面将考察一类预测法，
    // 详细内容可参考Amana tides-Woo(1987)。
    // 因为光线与栅格单元表面之间的交点呈非均等间隔状态， 而x、y、z方向上的则呈均等相交间隔。
    // 通过计算x、y、z方向上栅格单元间的参数增量，上述结论可适当地简化代码的复杂度。
    // 针对某一栅格单元，有如下等式：
    double dtx = (tx_max - tx_min) / nx;
    double dty = (ty_max - ty_min) / ny;
    double dtz = (tz_max - tz_min) / nz;
    // 光线碰撞邻接x，y，z表面时的t值tx_next，ty_next，tz_next。
    double  tx_next, ty_next, tz_next;
    // XYZ方向的步进次数。可以为负。表示反方向步进。
    int     ix_step, iy_step, iz_step;
    // 告知算法，光线何时在x，y，z方向上离开栅格对象。
    int     ix_stop, iy_stop, iz_stop;
    // 如果X方向的步进值为正向，
    if (dx > 0) {
        // 根据碰撞点（如果光线始于栅格内部就是起点，后面不再重复）得到碰撞点所在的栅格。
        tx_next = tx_min + (ix + 1) * dtx;
        ix_step = +1;
        ix_stop = nx;
    }
    // 如果X方向的步进值为负向，
    else {
        tx_next = tx_min + (nx - ix) * dtx;
        ix_step = -1;
        ix_stop = -1;
    }

    if (dx == 0.0) {
        tx_next = kHugeValue;
        ix_step = -1;
        ix_stop = -1;
    }

    // 如果Y方向的步进值为正向，
    if (dy > 0) {
        // 根据碰撞点得到碰撞点所在的栅格。
        ty_next = ty_min + (iy + 1) * dty;
        iy_step = +1;
        iy_stop = ny;
    }
    else {
        ty_next = ty_min + (ny - iy) * dty;
        iy_step = -1;
        iy_stop = -1;
    }

    if (dy == 0.0) {
        ty_next = kHugeValue;
        iy_step = -1;
        iy_stop = -1;
    }

    // 如果Z方向的步进值为正向，
    if (dz > 0) {
        tz_next = tz_min + (iz + 1) * dtz;
        iz_step = +1;
        iz_stop = nz;
    }
    else {
        tz_next = tz_min + (nz - iz) * dtz;
        iz_step = -1;
        iz_stop = -1;
    }

    if (dz == 0.0) {
        tz_next = kHugeValue;
        iz_step = -1;
        iz_stop = -1;
    }


    // Line7. 遍历栅格对象。traverse the grid
    while (true) {
        GeometricObject* object_ptr = cells[ix + nx * iy + nx * ny * iz];
        // 需要注意的是，当碰撞发生于t<tx_next或t<ty_next范围内时，
        // 才有必要记录碰撞点的信息，以将其限制在当前栅格单元内。
        if (tx_next < ty_next && tx_next < tz_next) {
            if (object_ptr && object_ptr->hit(ray, t, sr) && t < tx_next) {
                material_ptr = object_ptr->get_material();
                return (true);
            }

            tx_next += dtx;
            ix += ix_step;

            if (ix == ix_stop)
                return (false);
        }
        else {
            if (ty_next < tz_next) {
                if (object_ptr && object_ptr->hit(ray, t, sr) && t < ty_next) {
                    material_ptr = object_ptr->get_material();
                    return (true);
                }

                ty_next += dty;
                iy += iy_step;

                if (iy == iy_stop)
                    return (false);
             }
             else {
                if (object_ptr && object_ptr->hit(ray, t, sr) && t < tz_next) {
                    material_ptr = object_ptr->get_material();
                    return (true);
                }

                tz_next += dtz;
                iz += iz_step;

                if (iz == iz_stop)
                    return (false);
             }
        }
    }
}    // end of hit

