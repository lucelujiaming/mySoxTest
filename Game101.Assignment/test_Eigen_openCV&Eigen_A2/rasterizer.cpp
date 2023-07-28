//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>
#include <stdexcept>

// ����һ������������λ�á�����pos_buf�С�
rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
	pos_buf_id obj;
	obj.pos_id = get_next_id();
	// map::emplace()��C++ STL�е����ú���������������Ԫ�ز��뵽ӳ�������С���Ч�ؽ������ߴ�������һ����
    // �����η���ͬһ��������ӳ����洢��һ��Ԫ�أ���Ϊӳ���ǲ��洢�����ֵͬ�ļ���������
    // �÷�:
    //     map_name.emplace(key, element)
    // �������ú�����������ǿ���Բ���������������
    //     key �Cָ��Ҫ�ڶ�ͼ�����в���ļ���
    //     element �Cָ��Ҫ����Map�����ļ�Ԫ�ء�
	pos_buf.emplace(obj.pos_id, positions);

	return obj;
}

// ����һ�������ζ���˳�򡣷���ind_buf�С�
rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
	ind_buf_id obj;
	obj.ind_id = get_next_id();
	// map::emplace()��C++ STL�е����ú���������������Ԫ�ز��뵽ӳ�������С���Ч�ؽ������ߴ�������һ����
    // �����η���ͬһ��������ӳ����洢��һ��Ԫ�أ���Ϊӳ���ǲ��洢�����ֵͬ�ļ���������
    // �÷�:
    //     map_name.emplace(key, element)
    // �������ú�����������ǿ���Բ���������������
    //     key �Cָ��Ҫ�ڶ�ͼ�����в���ļ���
    //     element �Cָ��Ҫ����Map�����ļ�Ԫ�ء�
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
    p << x, y;	//.head(2)ָ������ǰ������ֵ����x,y
    a = _v[0].head(2) - _v[1].head(2);          //a = A - B  ��B->A
    b = _v[1].head(2) - _v[2].head(2);          //b = B - C  ��C->B
    c = _v[2].head(2) - _v[0].head(2);          //c = C - A  ��A->C  
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
    // ֻ�����ڻ���������
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
		// ʹ��ת��������ת�������ε��������㡣
        Eigen::Vector4f v[] = {
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[0]], 1.0f),
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[1]], 1.0f),
			mvp * to_vec4(vectorPosotionsBuffer[objIndice[2]], 1.0f)
        };
		// ���������������˹�һ����
		for (Eigen::Vector4f& vec : v) {
			float wTemp = vec.w();
            vec /= vec.w();
			std::cout << vec << std::endl;

		}
		// ת��Ϊ��Ļ���ꡣ
		for (Eigen::Vector4f& vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }
		// ʹ�ü���õĽ���趨�����ε��������㡣
        for (int i = 0; i < 3; ++i)
        {
			objTriangle.setVertex(i, v[i].head<3>());
			objTriangle.setVertex(i, v[i].head<3>());
			objTriangle.setVertex(i, v[i].head<3>());
        }

		auto col_x = col[objIndice[0]];
		auto col_y = col[objIndice[1]];
		auto col_z = col[objIndice[2]];
		// Ϊ��������ָ����ɫ��
        objTriangle.setColor(0, col_x[0], col_x[1], col_x[2]);
        objTriangle.setColor(1, col_y[0], col_y[1], col_y[2]);
        objTriangle.setColor(2, col_z[0], col_z[1], col_z[2]);

		rasterize_triangle(objTriangle);
    }
}

void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4(); //v[0],v[1],v[2]�ֱ�Ϊ�����ε��������㣬����ά����

    //�Ƚ���������ĺ������꣬ȷ����Χ�еı߽粢ȡ��
    double min_x = std::min(v[0][0], std::min(v[1][0], v[2][0]));
    double max_x = std::max(v[0][0], std::max(v[1][0], v[2][0]));
    double min_y = std::min(v[0][1], std::min(v[1][1], v[2][1]));
    double max_y = std::max(v[0][1], std::max(v[1][1], v[2][1]));
    min_x = static_cast<int>(std::floor(min_x));
    min_y = static_cast<int>(std::floor(min_y));
    max_x = static_cast<int>(std::ceil(max_x));
    max_y = static_cast<int>(std::ceil(max_y));
    
    //�˴�ʵ�ֵ���MSAA
    std::vector<Eigen::Vector2f> pos
    {                               //��һ�����طָ��ķ� ��Ȼ�㻹���Էֳ�4x4 8x8�ȵ������㻹����Ϊ��ĳ�����������Ƴɲ������ͼ�����ָԪ
        {0.25,0.25},                //����
        {0.75,0.25},                //����
        {0.25,0.75},                //����
        {0.75,0.75}                 //����
    };

    for (int i = min_x; i <= max_x; ++i)
    {
        for (int j = min_y; j <= max_y; ++j)
        {
            int count = 0; //��ʼ�����ĸ�С���ӣ����ƽ��ֵ
            for (int MSAA_4 = 0; MSAA_4 < 4; ++MSAA_4)
            {
                if (insideTriangle(static_cast<float>(i+pos[MSAA_4][0]), static_cast<float>(j+pos[MSAA_4][1]),t.v))
                    ++count;
            }
            if(count) //������һ��С��������������
            {
                //�˴��ǿ���д��룬���z����ԭ����ע�ͣ�
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
                    depth_buf[get_index(i, j)] = z_interpolated;//�������
                    //����ע�⣬��Ȼ˵����˵"��ת��z����֤��������������Խ���ʾ���ӵ�ԽԶ"��
                    //�������ҵĲ鿴��ʵ���ϲ�û�з�ת����˻��ǰ���-z����ԶС��������ȻҲ���������油һ�����Ų���û��Ҫ
                    Eigen::Vector3f color = t.getColor() * (count / 4.0);//����ɫ����ƽ��
                    Eigen::Vector3f point;
                    point << static_cast<float>(i), static_cast<float>(j), z_interpolated;
                    set_pixel(point, color);//������ɫ
                }
            }
        }
    }
}

// �趨ģ�;���
void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

// �趨��ͼ����
void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

// �趨ͶӰ����
void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

// ���֡���嶯̬�������Ȼ��嶯̬�����е����ݡ�
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

// ʹ�ÿ�͸߳�ʼ����դ����
rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

// ������Ļ����������λ�á�
int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

// ���㺯����ʵ��Ϊ���frame_buf���顣
void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;
}

