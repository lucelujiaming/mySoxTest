//
// Created by goksu on 4/6/19.
//

#include <algorithm>
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

// Bresenham's line drawing algorithm
// Code taken from a stack overflow answer: https://stackoverflow.com/a/16405254
// ʹ��Bresenham�㷨����ֱ�ߡ�
/************************************************************************/
/* ����㷨��˼���������ġ��������ȼ���dx > dy��                        */
/* ��ôΪ�˻��ƴ�P0��P1��ֱ�ߣ��������ȿ϶���Ҫ��X��������ۼӡ�        */
/* ��������һ�����λ�á���DDA�㷨�У�����ʹ��ROUND�����������롣       */
/************************************************************************/
/* ��Bresenham�㷨��ԭ����֮���ơ� �������ȼ���dx > dy��                */
/* �����趨e�ĳ�ֵΪ-dx����Ϊ�����ڵ�-1�ֵĲ�����                   */
/* ���������Ϊ-dyҲ�ǿ��Եġ�������Ϊdx > dy��������Ҫ������X�����ۼӡ�*/
/* �����趨e�ĳ�ֵΪ-dx��                                               */
/* ��Ϊe�ĳ�ֵΪ-dx�������������X�����Ͻ����ۼӡ���Ϊ���������룬      */
/* �Ǿ���ζ�����������2 * dy��������ʱ��������Y�����ϲ��ۼӡ�      */
/* ����������ͻᱣ������һ�֡������������Y�������ۼӡ�              */
/* �Ǿ���ζ����������2 * dx��                                         */
/* ˵�ĸ�����һ����ǣ���������б�߾��ǻ���һ��̨�����ߵĹ��̡�         */
/* ��Щ�㷨������ȥ��ʵ����һ��Ŀ�ģ�����ͨ���ۼ���ֱ������������Ծ��   */
/************************************************************************/
/* ֮���Ǵ���ʵ����һ������ĵط����������X������ۼ�ֵ��Y����Ĵ�   */
/* ����X�����ƶ����������Y�����ƶ��� ��֮dy > dx����ʵҲ��һ���ġ�     */
/* ֻ��Ҫ�޸��ۼӷ��򼴿ɡ����ۼӷ������Ϊ������signX��signY���ơ�     */
/************************************************************************/
void rst::rasterizer::draw_line(Eigen::Vector3f begin, Eigen::Vector3f end)
{
	// auto�ؼ������������Զ��洢���ı�����
    // �������ں����ڲ��������ʱ�����û��ָ���ñ����Ĵ洢���ͣ���Ĭ��Ϊauto���͡�
    // ����ζ�ţ��ñ��������Զ�����洢�ռ䣬���ں���ִ�н������Զ��ͷš�
    // ��ˣ����ǲ���Ҫ�ֶ���������Ĵ洢�ռ䣬Ҳ����Ҫ���ı���й¶�����⡣
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
	// ���X�����������
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
	// ���Y�����������
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

// ��һ��Vector3f��һ��w��ϳ�һ��Vector4f����
Vector4f to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

// ���ƺ�����������һ�������Ρ�
void rst::rasterizer::draw(rst::pos_buf_id pos_buffer, rst::ind_buf_id ind_buffer, rst::Primitive type)
{
    // ֻ�����ڻ���������
    if (type != rst::Primitive::Triangle)
    {
        throw std::runtime_error("Drawing primitives other than triangle is not implemented yet!");
    }
    // 
	std::vector<Eigen::Vector3f> vectorPosotionsBuffer = pos_buf[pos_buffer.pos_id];
	std::vector<Eigen::Vector3i> vectorIndices         = ind_buf[ind_buffer.ind_id];

    float f1 = (100 - 0.1) / 2.0;
    float f2 = (100 + 0.1) / 2.0;
	// ����ת������
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
		// Ϊ��������ָ����ɫ��
		objTriangle.setColor(0, 255.0, 0.0, 0.0);
		objTriangle.setColor(1, 0.0, 255.0, 0.0);
		objTriangle.setColor(2, 0.0, 0.0, 255.0);
		// ���������ε������ߡ�
		rasterize_wireframe(objTriangle);
    }
}

// ���������ε������ߡ�
void rst::rasterizer::rasterize_wireframe(const Triangle& t)
{
	// ����C�㵽A���ֱ�ߡ�
	draw_line(t.c(), t.a());
	// ����C�㵽B���ֱ�ߡ�
	draw_line(t.c(), t.b());
	// ����B�㵽A���ֱ�ߡ�
    draw_line(t.b(), t.a());
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
    return (height-y)*width + x;
}

// ���㺯����ʵ��Ϊ���frame_buf���顣
void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    if (point.x() < 0 || point.x() >= width ||
        point.y() < 0 || point.y() >= height) return;
    int ind = (height-point.y())*width + point.x();
    frame_buf[ind] = color;
}

