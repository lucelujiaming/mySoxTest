//
// Created by goksu on 4/6/19.
//

#include <algorithm>
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

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
// 使用Bresenham算法绘制直线。
/************************************************************************/
/* 这个算法的思想是这样的。我们首先假设dx > dy，                        */
/* 那么为了绘制从P0到P1的直线，我们首先肯定是要在X方向进行累加。        */
/* 而计算下一个点的位置。在DDA算法中，我们使用ROUND进行四舍五入。       */
/************************************************************************/
/* 而Bresenham算法的原理与之类似。 我们首先假设dx > dy，                */
/* 首先设定e的初值为-dx。作为不存在的-1轮的残留误差。                   */
/* 这个理论上为-dy也是可以的。但是因为dx > dy，我们需要优先在X方向累加。*/
/* 所以设定e的初值为-dx。                                               */
/* 因为e的初值为-dx。因此上首先在X方向上进行累加。因为是四舍五入，      */
/* 那就意味着误差增加了2 * dy。如果这个时候，我们在Y方向上不累加。      */
/* 而那这个误差就会保留到下一轮。而如果我们在Y方向上累加。              */
/* 那就意味着误差减少了2 * dx。                                         */
/* 说的更形象一点就是，本来绘制斜线就是绘制一个台阶折线的过程。         */
/* 这些算法绕来绕去其实都是一个目的，就是通过累加在直线上下来回跳跃。   */
/************************************************************************/
/* 之后是代码实现中一个巧妙的地方。就是如果X方向的累加值比Y方向的大，   */
/* 就往X方向移动，否则就往Y方向移动。 反之dy > dx，其实也是一样的。     */
/* 只需要修改累加方向即可。且累加方向可以为负。由signX和signY控制。     */
/************************************************************************/
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
	// auto关键字用于声明自动存储类别的变量。
    // 当我们在函数内部定义变量时，如果没有指定该变量的存储类型，则默认为auto类型。
    // 这意味着，该变量将被自动分配存储空间，并在函数执行结束后自动释放。
    // 因此，我们不需要手动管理变量的存储空间，也不需要担心变量泄露的问题。
    float  x1 = begin.x();
	float y1 = begin.y();
	float x2 = end.x();
	float y2 = end.y();

    Eigen::Vector3f line_color = {255, 255, 255};

    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;

    dx=x2-x1;
    dy=y2-y1;
    dx1=fabs((float)dx);
	dy1 = fabs((float)dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;
	// 如果X方向的增量大。
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
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
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
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
	}
	// 如果Y方向的增量大。
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
        Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
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
            Eigen::Vector3f point = Eigen::Vector3f(x, y, 1.0f);
            set_pixel(point,line_color);
        }
    }
}

// 把一个Vector3f和一个w组合成一个Vector4f对象。
Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

// 绘制函数。绘制了一个三角形。
void rst::rasterizer::draw(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer, rst::Primitive type)
{
    // 只能用于绘制三角形
    if (type != rst::Primitive::Triangle)
    {
        throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
    }
    // 
	std::vector<Eigen::Vector3f> vectorPosotionsBuffer = pos_buf[pos_buffer.pos_id];
	std::vector<Eigen::Vector3i> vectorIndices         = ind_buf[ind_buffer.ind_id];

    float f1 = (100 - 0.1) / 2.0;
    float f2 = (100 + 0.1) / 2.0;
	// 生成转换矩阵。
    Eigen::Matrix4f mvp = projection * view * model;
	for (Eigen::Vector3i objIndice : vectorIndices)
    {
		Triangle objTriangle;
		// 使用转换矩阵来转换三角形的三个顶点。
        Eigen::Vector4f v[] = {
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[0]], 1.0f),
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[1]], 1.0f),
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[2]], 1.0f)
        };
		// 看起来好像是做了归一化。
		for (Eigen::Vector4f& vec : v) {
			float wTemp = vec.w();
            vec /= vec.w();
			std::cout << vec << std::endl;

		}
		// 转换为屏幕坐标。
		for (Eigen::Vector4f& vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }
		// 使用计算好的结果设定三角形的三个顶点。
        for (int i = 0; i < 3; ++i)
        {
			objTriangle.setVertex(i, v[i].head<3>());
			objTriangle.setVertex(i, v[i].head<3>());
			objTriangle.setVertex(i, v[i].head<3>());
        }
		// 为三个顶点指定颜色。
		objTriangle.setColor(0, 255.0, 0.0, 0.0);
		objTriangle.setColor(1, 0.0, 255.0, 0.0);
		objTriangle.setColor(2, 0.0, 0.0, 255.0);
		// 绘制三角形的三条边。
		rasterize_wireframe(objTriangle);
    }
}

// 绘制三角形的三条边。
void rst::rasterizer::rasterize_wireframe(const Triangle& t)
{
	// 绘制C点到A点的直线。
	draw_line(t.c(), t.a());
	// 绘制C点到B点的直线。
	draw_line(t.c(), t.b());
	// 绘制B点到A点的直线。
    draw_line(t.b(), t.a());
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
}

// 根据屏幕坐标获得数据位置。
int rst::rasterizer::get_index(int x, int y)
{
    return (height-y)*width + x;
}

// 画点函数。实现为填充frame_buf数组。
void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    int ind = (height-point.y())*width + point.x();
    frame_buf[ind] = color;
}

