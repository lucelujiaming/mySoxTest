//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdexcept>

// 传入一个三角形坐标位置。放在pos_buf中。
rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
	pos_buf_id obj;
	obj.pos_id = get_next_id();
	// map::emplace()是C++ STL中的内置函数，它将键及其元素插入到映射容器中。有效地将容器尺寸增加了一个。
    // 如果多次放置同一个键，则映射仅存储第一个元素，因为映射是不存储多个相同值的键的容器。
    // 用法:
    //     map_name.emplace(key, element)
    // 参数：该函数接受两个强制性参数，如下所述：
    //     key –指定要在多图容器中插入的键。
    //     element –指定要插入Map容器的键元素。
	pos_buf.emplace(obj.pos_id, positions);

	return obj;
}

// 传入一个三角形顶点顺序。放在ind_buf中。
rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
	ind_buf_id obj;
	obj.ind_id = get_next_id();
	// map::emplace()是C++ STL中的内置函数，它将键及其元素插入到映射容器中。有效地将容器尺寸增加了一个。
    // 如果多次放置同一个键，则映射仅存储第一个元素，因为映射是不存储多个相同值的键的容器。
    // 用法:
    //     map_name.emplace(key, element)
    // 参数：该函数接受两个强制性参数，如下所述：
    //     key –指定要在多图容器中插入的键。
    //     element –指定要插入Map容器的键元素。
	ind_buf.emplace(obj.ind_id, indices);

	return obj;
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
	col_buf_id obj;
	obj.col_id = get_next_id();
	col_buf.emplace(obj.col_id, cols);

	return obj;
}

rst::col_buf_id rst::rasterizer::load_normals(const std::vector<Eigen::Vector3f>& normals)
{
	rst::col_buf_id obj;
    obj.col_id = get_next_id();
	nor_buf.emplace(obj.col_id, normals);

	normal_id = obj.col_id;

	return obj;
}


// Bresenham's line drawing algorithm
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
    auto x1 = begin.x();
    auto y1 = begin.y();
    auto x2 = end.x();
    auto y2 = end.y();

    Eigen::Vector3f line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
	dx1 = fabs((float)dx);
	dy1 = fabs((float)dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;

    if(dy1<=dx1)
    {
        if(dx>=0)
        {
            x=x1;
            y=y1;
            xe=x2;
        }
        else
        {
            x=x2;
            y=y2;
            xe=x1;
        }
        Eigen::Vector2i point = Eigen::Vector2i(x, y);
        set_pixel(point,line_color);
        for(i=0;x<xe;i++)
        {
            x=x+1;
            if(px<0)
            {
                px=px+2*dy1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }
                else
                {
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
//            delay(0);
            Eigen::Vector2i point = Eigen::Vector2i(x, y);
            set_pixel(point,line_color);
        }
    }
    else
    {
        if(dy>=0)
        {
            x=x1;
            y=y1;
            ye=y2;
        }
        else
        {
            x=x2;
            y=y2;
            ye=y1;
        }
        Eigen::Vector2i point = Eigen::Vector2i(x, y);
        set_pixel(point,line_color);
        for(i=0;y<ye;i++)
        {
            y=y+1;
            if(py<=0)
            {
                py=py+2*dx1;
            }
            else
            {
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    x=x+1;
                }
                else
                {
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
//            delay(0);
            Eigen::Vector2i point = Eigen::Vector2i(x, y);
            set_pixel(point,line_color);
        }
    }
}

// 把一个Vector3f和一个w组合成一个Vector4f对象。
Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

static bool insideTriangle(int x, int y, const Vector4f* _v){
    Vector3f v[3];
    for(int i=0;i<3;i++)
        v[i] = {_v[i].x(),_v[i].y(), 1.0};
    Vector3f f0,f1,f2;
    f0 = v[1].cross(v[0]);
    f1 = v[2].cross(v[1]);
    f2 = v[0].cross(v[2]);
    Vector3f p(x,y,1.);
    if((p.dot(f0)*f0.dot(v[2])>0) && (p.dot(f1)*f1.dot(v[0])>0) && (p.dot(f2)*f2.dot(v[1])>0))
        return true;
    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector4f* v){
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
	return std::tuple<float, float, float>(c1, c2, c3);
}

void rst::rasterizer::draw(std::vector<Triangle *> &TriangleList) {

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (const auto& t:TriangleList)
    {
        Triangle newtri = *t;

        std::array<Eigen::Vector4f, 3> mm {
                (view * model * t->v[0]),
                (view * model * t->v[1]),
                (view * model * t->v[2])
        };

        std::array<Eigen::Vector3f, 3> viewspace_pos;

		std::transform(mm.begin(), mm.end(), viewspace_pos.begin(), [](Eigen::Vector4f& v) {
            return v.template head<3>();
        });

        Eigen::Vector4f v[] = {
                mvp * t->v[0],
                mvp * t->v[1],
                mvp * t->v[2]
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec.x()/=vec.w();
            vec.y()/=vec.w();
            vec.z()/=vec.w();
        }

        Eigen::Matrix4f inv_trans = (view * model).inverse().transpose();
        Eigen::Vector4f n[] = {
                inv_trans * to_vec4(t->normal[0], 0.0f),
                inv_trans * to_vec4(t->normal[1], 0.0f),
                inv_trans * to_vec4(t->normal[2], 0.0f)
        };

        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }
		// 使用计算好的结果设定三角形的三个顶点。
        for (int i = 0; i < 3; ++i)
        {
            //screen space coordinates
            newtri.setVertex(i, v[i]);
        }

        for (int i = 0; i < 3; ++i)
        {
            //view space normal
            newtri.setNormal(i, n[i].head<3>());
        }

        newtri.setColor(0, 148,121.0,92.0);
        newtri.setColor(1, 148,121.0,92.0);
        newtri.setColor(2, 148,121.0,92.0);

        // Also pass view space vertice position
        rasterize_triangle(newtri, viewspace_pos);
    }
}

static Eigen::Vector3f interpolate(float alpha, float beta, float gamma, const Eigen::Vector3f& vert1, const Eigen::Vector3f& vert2, const Eigen::Vector3f& vert3, float weight)
{
    return (alpha * vert1 + beta * vert2 + gamma * vert3) / weight;
}

static Eigen::Vector2f interpolate(float alpha, float beta, float gamma, const Eigen::Vector2f& vert1, const Eigen::Vector2f& vert2, const Eigen::Vector2f& vert3, float weight)
{
    auto u = (alpha * vert1[0] + beta * vert2[0] + gamma * vert3[0]);
    auto v = (alpha * vert1[1] + beta * vert2[1] + gamma * vert3[1]);

    u /= weight;
    v /= weight;

    return Eigen::Vector2f(u, v);
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t, const std::array<Eigen::Vector3f, 3>& view_pos) 
{
    auto v = t.toVector4(); //v[0],v[1],v[2]分别为三角形的三个顶点，是四维向量
    //比较三个顶点的横纵坐标，确定包围盒的边界并取整
    double min_x = std::min(v[0][0], std::min(v[1][0], v[2][0]));
    double max_x = std::max(v[0][0], std::max(v[1][0], v[2][0]));
    double min_y = std::min(v[0][1], std::min(v[1][1], v[2][1]));
    double max_y = std::max(v[0][1], std::max(v[1][1], v[2][1]));
    min_x = static_cast<int>(std::floor(min_x));
    min_y = static_cast<int>(std::floor(min_y));
    max_x = static_cast<int>(std::ceil(max_x));
    max_y = static_cast<int>(std::ceil(max_y));
    
    //此处实现的是MSAA
    std::vector<Eigen::Vector2f> pos
    {                               //对一个像素分割四份 当然你还可以分成4x4 8x8等等甚至你还可以为了某种特殊情况设计成不规则的图形来分割单元
        {0.25,0.25},                //左下
        {0.75,0.25},                //右下
        {0.25,0.75},                //左上
        {0.75,0.75}                 //右上
    };

    for (int i = min_x; i <= max_x; ++i)
    {
        for (int j = min_y; j <= max_y; ++j)
        {
            int count = 0; //开始遍历四个小格子，获得平均值
            for (int MSAA_4 = 0; MSAA_4 < 4; ++MSAA_4)
            {
			    // 1. 创建三角形的 2 维 bounding box。
				// 2. 遍历此 bounding box 内的所有像素（使用其整数索引）。
				//    然后，使用像素中心的屏幕空间坐标来检查中心点是否在三角形内。
                if (insideTriangle(static_cast<float>(i+pos[MSAA_4][0]), static_cast<float>(j+pos[MSAA_4][1]),t.v))
                    ++count;
            }
			// 3. 如果在内部，
            if(count) //至少有一个小格子在三角形内
            {
                //此处是框架中代码，获得z，见原程序注释：
                // auto [alpha, beta, gamma]
				std::tuple<float, float, float > objTuple = computeBarycentric2D(i + 0.5, j + 0.5, t.v);
				float alpha = std::get<0>(objTuple);
				float beta = std::get<1>(objTuple);
				float gamma = std::get<2>(objTuple);
                float Z = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float zp = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                zp *= Z;
                //end
				// 则将其位置处的插值深度值 (interpolated depth value) 与深度缓冲区 (depth buffer) 中的相应值进行比较。
                if (depth_buf[get_index(i, j)] > zp)
                {
                    // 4. 如果当前点更靠近相机，请设置像素颜色并更新深度缓冲区 (depth buffer)。
                    depth_buf[get_index(i, j)] = zp;//更新深度
                    //这里注意，虽然说明上说"反转了z，保证都是正数，并且越大表示离视点越远"，
                    //但经过我的查看，实际上并没有反转，因此还是按照-z近大远小来做，当然也可以在上面补一个负号不过没必要

                    //利用重心坐标插值各种值
					auto interpolated_color = interpolate(alpha, beta, gamma, t.color[0], t.color[1], t.color[2], 1);
					auto interpolated_normal = interpolate(alpha, beta, gamma, t.normal[0], t.normal[1], t.normal[2], 1).normalized();
					auto interpolated_texcoords = interpolate(alpha, beta, gamma, t.tex_coords[0], t.tex_coords[1], t.tex_coords[2], 1);
					auto interpolated_shadingcoords = interpolate(alpha, beta, gamma, view_pos[0], view_pos[1], view_pos[2], 1);
                    //shadingcoords是由view_pos插值得到，也就是物体表面的点在相机坐标系的位置。他们会在shader中被用到，来计算光照等信息。

                    //此处是框架中代码，获得z，见原程序注释：
					if (texture)
					{
						fragment_shader_payload payload(interpolated_color, interpolated_normal, interpolated_texcoords, texture);
						payload.view_pos = interpolated_shadingcoords;
						auto pixel_color = fragment_shader(payload);
						// 设置颜色
						set_pixel(Eigen::Vector2i(i, j), pixel_color * (count / 4.0));
					}
                    //end

                }
            }
        }
    }
}

// 设定模型矩阵。
void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

// 设定视图矩阵。
void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

// 设定投影矩阵。
void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

// 清除帧缓冲动态数组和深度缓冲动态数组中的内容。
void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
    }
}

// 使用宽和高初始化光栅器。
rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);

    texture = NULL;
}

// 根据屏幕坐标获得数据位置。
int rst::rasterizer::get_index(int x, int y)
{
    return (height-y)*width + x;
}

void rst::rasterizer::set_pixel(const Vector2i &point, const Eigen::Vector3f &color)
{
    //old index: auto ind = point.y() + point.x() * width;
    int ind = (height-point.y())*width + point.x();
    frame_buf[ind] = color;
}

void rst::rasterizer::set_vertex_shader(std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader)
{
    vertex_shader = vert_shader;
}

void rst::rasterizer::set_fragment_shader(std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader)
{
    fragment_shader = frag_shader;
}

