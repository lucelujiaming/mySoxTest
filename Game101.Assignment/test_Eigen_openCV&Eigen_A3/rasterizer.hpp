//
// Created by goksu on 4/6/19.
//

#pragma once

#include <Eigen/Eigen>
// #include <optional>
#include <algorithm>
#include "global.hpp"
#include "Shader.hpp"
#include "Triangle.hpp"

using namespace Eigen;

namespace rst
{
    enum class Buffers
    {
        Color = 1,
        Depth = 2
    };

    inline Buffers operator|(Buffers a, Buffers b)
    {
        return Buffers((int)a | (int)b);
    }

    inline Buffers operator&(Buffers a, Buffers b)
    {
        return Buffers((int)a & (int)b);
    }

    enum class Primitive
    {
        Line,
        Triangle
    };

    /*
     * For the curious : The draw function takes two buffer id's as its arguments. These two structs
     * make sure that if you mix up with their orders, the compiler won't compile it.
     * Aka : Type safety
     * */
    struct pos_buf_id
    {
        int pos_id = 0;
    };

    struct ind_buf_id
    {
        int ind_id = 0;
    };

    struct col_buf_id
    {
        int col_id = 0;
    };

    class rasterizer
    {
    public:
        rasterizer(int w, int h);
        pos_buf_id load_positions(const std::vector<Eigen::Vector3f>& positions);
        ind_buf_id load_indices(const std::vector<Eigen::Vector3i>& indices);
        col_buf_id load_colors(const std::vector<Eigen::Vector3f>& colors);
        col_buf_id load_normals(const std::vector<Eigen::Vector3f>& normals);

    // ���ڲ���ģ�;�����Ϊ�������ݸ���դ������
        void set_model(const Eigen::Matrix4f& m);
    // ����ͼ�任������Ϊ�ڲ���ͼ����
        void set_view(const Eigen::Matrix4f& v);
    // ���ڲ���ͶӰ������Ϊ��������p�������ݸ���դ����
        void set_projection(const Eigen::Matrix4f& p);

        void set_texture(Texture * tex) { texture = tex; }

        void set_vertex_shader(std::function<Eigen::Vector3f(vertex_shader_payload)> vert_shader);
        void set_fragment_shader(std::function<Eigen::Vector3f(fragment_shader_payload)> frag_shader);
        // ����Ļ���ص� (x, y) ��Ϊ (r, g, b) ����ɫ����д����Ӧ��֡������λ�á�
        void set_pixel(const Vector2i &point, const Eigen::Vector3f &color);

        void clear(Buffers buff);

        void draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type);
        void draw(std::vector<Triangle *> &TriangleList);

        std::vector<Eigen::Vector3f>& frame_buffer() { return frame_buf; }

    private:
        void draw_line(Eigen::Vector3f begin, Eigen::Vector3f end);

        void rasterize_triangle(const Triangle& t, const std::array<Eigen::Vector3f, 3>& world_pos);

        // VERTEX SHADER -> MVP -> Clipping -> /.W -> VIEWPORT -> DRAWLINE/DRAWTRI -> FRAGSHADER

    private:
	    // �����任����
	    Eigen::Matrix4f model;            // ģ�ͱ任����
	    Eigen::Matrix4f view;             // �ӵ�任����
	    Eigen::Matrix4f projection;       // ͶӰ�任����

        int normal_id = -1;
		// ���map���������ڻ��Ƶ������εĶ������ꡣ
		// ÿһ��key/value�Զ�������һ�������εĶ������ꡣkey��1��ʼ��
        std::map<int, std::vector<Eigen::Vector3f>> pos_buf;
		// ���map���������ڻ��Ƶ������εĶ���˳��
		// ÿһ��key/value�Զ�������һ�������εĶ���˳��key��1��ʼ��
        std::map<int, std::vector<Eigen::Vector3i>> ind_buf;
        std::map<int, std::vector<Eigen::Vector3f>> col_buf;
        std::map<int, std::vector<Eigen::Vector3f>> nor_buf;

        // std::optional<Texture> texture;
		Texture* texture;

        std::function<Eigen::Vector3f(fragment_shader_payload)> fragment_shader;
        std::function<Eigen::Vector3f(vertex_shader_payload)> vertex_shader;
		// ֡���嶯̬���飬���ڴ洢��Ҫ����Ļ�ϻ��Ƶ���ɫ���ݡ�
        std::vector<Eigen::Vector3f> frame_buf;
		// ��Ȼ��嶯̬���顣Ŀǰû���õ���
        std::vector<float> depth_buf;
        int get_index(int x, int y);

        int width, height;

		// ���Ƶ������μ�ֵ�Ե�KEY�����ۼӺ�����
        int next_id = 0;
        int get_next_id() { return next_id++; }
    };
}
