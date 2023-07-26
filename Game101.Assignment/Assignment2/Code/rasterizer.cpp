// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}


//x,y�ǹ۲���x��yֵ
static bool insideTriangle(double x, double y, const Vector3f* _v)
{   
    // TODO : Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
	Eigen::Vector2f p;	//���Ŀ��
	p << x, y;
	
	//.head(2)��ȡ������ǰ������ֵ
	Eigen::Vector2f AB = _v[1].head(2) - _v[0].head(2);//AB = B - A  ��A->B�������Դ�����
	Eigen::Vector2f BC = _v[2].head(2) - _v[1].head(2);
	Eigen::Vector2f CA = _v[0].head(2) - _v[2].head(2);

	Eigen::Vector2f AP = p - _v[0].head(2);	//�������㵽�۲�������
	Eigen::Vector2f BP = p - _v[1].head(2);
	Eigen::Vector2f CP = p - _v[2].head(2);
	
	// ���������������ֵΪ�жϽ���Ƿ�Ϊ��ͬ���ţ��ж�ÿ��z�����Ƿ�ͳһ��
	return AB[0] * AP[1] - AB[1] * AP[0] > 0 
		&& BC[0] * BP[1] - BC[1] * BP[0] > 0
		&& CA[0] * CP[1] - CA[1] * CP[0] > 0;
}



static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);

        rasterize_triangle(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();//�����ǰ������νṹ�廻�����������磨x1,y1,z1,1��,����1��ʾ����һ����,�����������
			   
    //bounding box
    float min_x = std::min(v[0][0], std::min(v[1][0], v[2][0]));
    float max_x = std::max(v[0][0], std::max(v[1][0], v[2][0]));
    float min_y = std::min(v[0][1], std::min(v[1][1], v[2][1]));
    float max_y = std::max(v[0][1], std::max(v[1][1], v[2][1]));

    min_x = static_cast<int>(std::floor(min_x));
    min_y = static_cast<int>(std::floor(min_y));
    max_x = static_cast<int>(std::ceil(max_x));
    max_y = static_cast<int>(std::ceil(max_y));

	//��߽�С������ȫ��ֱ���ᣬ�ұ߽�С������ֱ���룬ȷ����Ԫ�߽����궼��������������һ����bounding box�ڡ�

    bool MSAA = true;//MSAA�Ƿ�����

    if (MSAA)
    {
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
                int count = 0;
                float minDepth = FLT_MAX;
                for (int MSAA_4 = 0; MSAA_4 < 4; ++MSAA_4)
                {
                    if (insideTriangle(static_cast<float>(i+pos[MSAA_4][0]), static_cast<float>(j+pos[MSAA_4][1]),t.v))
                    {
			    		auto[alpha, beta, gamma] = computeBarycentric2D(static_cast<float>(i + pos[MSAA_4][0]), static_cast<float>(j + pos[MSAA_4][1]), t.v);
	                    float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
	                    float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
	                    z_interpolated *= w_reciprocal;

                        minDepth = std::min(minDepth, z_interpolated);
                        ++count;
                    }
                }
                if (count)
                {
		    if(get_index(i, j) >= 0)
		    {
		            if (depth_buf[get_index(i, j)] > minDepth)
		            {
		                depth_buf[get_index(i, j)] = minDepth;//�������

		                Eigen::Vector3f color = t.getColor() * (count / 4.0);//����ɫ����ƽ����ʹ�ñ߽��ƽ����Ҳ��һ��ģ�����ֶ�
		                Eigen::Vector3f point;
		                point << static_cast<float>(i), static_cast<float>(j), minDepth;
		                set_pixel(point, color);//������ɫ
		            }
                    }
                }
            }
        }
    }
    else
    {
	for (int i = min_x; i <= max_x; ++i)
	{
		for (int j = min_y; j <= max_y; ++j)
		{
			if (insideTriangle(static_cast<float>(i+0.5), static_cast<float>(j+0.5),t.v))
			{
				auto [alpha, beta, gamma] = computeBarycentric2D(static_cast<float>(i), static_cast<float>(j), t.v);
				float w_reciprocal = 1.0 / (alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
				float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
				z_interpolated *= w_reciprocal;
				if(get_index(i, j) >= 0)
				{
					if (depth_buf[get_index(i, j)] > z_interpolated)
					{
						depth_buf[get_index(i, j)] = z_interpolated;//�������
						Eigen::Vector3f color = t.getColor();
						Eigen::Vector3f point;
						point << static_cast<float>(i), static_cast<float>(j), z_interpolated;
						set_pixel(point, color);//������ɫ
					}
				}
				else
				{
					printf("get_index(%d, %d) return %d", i, j, get_index(i, j));
				}
			}
		}
	}
   }
}




void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

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

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    depth_buf.resize(w * h);
}

int rst::rasterizer::get_index(int x, int y)
{
    return (height-1-y)*width + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on