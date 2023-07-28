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

Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


static bool insideTriangle(int x, int y, const Vector3f* _v)
{     
    Eigen::Vector2f p, a, b, c, AP, BP, CP;  
    p << x, y;	//.head(2)指这个点的前两个数值，即x,y
    a = _v[0].head(2) - _v[1].head(2);          //a = A - B  即B->A
    b = _v[1].head(2) - _v[2].head(2);          //b = B - C  即C->B
    c = _v[2].head(2) - _v[0].head(2);          //c = C - A  即A->C  
    AP = p - _v[0].head(2);
    BP = p - _v[1].head(2);
    CP = p - _v[2].head(2);
    return ((AP[0] * c[1] - AP[1] * c[0] > 0 && BP[0] * a[1] - BP[1] * a[0] > 0 && CP[0] * b[1] - CP[1] * b[0] > 0)||
    (AP[0] * c[1] - AP[1] * c[0] < 0 && BP[0] * a[1] - BP[1] * a[0] < 0 && CP[0] * b[1] - CP[1] * b[0] < 0));
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
	return std::tuple<float, float, float>(c1, c2, c3);
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    // 只能用于绘制三角形
    if (type != rst::Primitive::Triangle)
    {
        throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
    }
    // 
    std::vector<Eigen::Vector3f> vectorPosotionsBuffer = pos_buf[pos_buffer.pos_id];
    std::vector<Eigen::Vector3i> vectorIndices = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

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

		auto col_x = col[objIndice[0]];
		auto col_y = col[objIndice[1]];
		auto col_z = col[objIndice[2]];
		// 为三个顶点指定颜色。
        objTriangle.setColor(0, col_x[0], col_x[1], col_x[2]);
        objTriangle.setColor(1, col_y[0], col_y[1], col_y[2]);
        objTriangle.setColor(2, col_z[0], col_z[1], col_z[2]);

		rasterize_triangle(objTriangle);
    }
}

void rst::rasterizer::rasterize_triangle(const Triangle& t) {
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
                if (insideTriangle(static_cast<float>(i+pos[MSAA_4][0]), static_cast<float>(j+pos[MSAA_4][1]),t.v))
                    ++count;
            }
            if(count) //至少有一个小格子在三角形内
            {
                //此处是框架中代码，获得z，见原程序注释：
                // auto [alpha, beta, gamma]
				std::tuple<float, float, float > objTuple = computeBarycentric2D(static_cast<float>(i + 0.5), static_cast<float>(j + 0.5), t.v);
				float alpha = std::get<0>(objTuple);
				float beta = std::get<1>(objTuple);
				float gamma = std::get<2>(objTuple);
                float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
                float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
                z_interpolated *= w_reciprocal;
                //end
                if (depth_buf[get_index(i, j)] > z_interpolated)
                {
                    depth_buf[get_index(i, j)] = z_interpolated;//更新深度
                    //这里注意，虽然说明上说"反转了z，保证都是正数，并且越大表示离视点越远"，
                    //但经过我的查看，实际上并没有反转，因此还是按照-z近大远小来做，当然也可以在上面补一个负号不过没必要
                    Eigen::Vector3f color = t.getColor() * (count / 4.0);//对颜色进行平均
                    Eigen::Vector3f point;
                    point << static_cast<float>(i), static_cast<float>(j), z_interpolated;
                    set_pixel(point, color);//设置颜色
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
}

// 根据屏幕坐标获得数据位置。
int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

// 画点函数。实现为填充frame_buf数组。
void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;
}

