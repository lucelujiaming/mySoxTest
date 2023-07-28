#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>
constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

//�ڴ˺����У���ֻ��Ҫʵ����ά���� z ����ת�ı任����
//�����ô���ƽ�������š�
Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float ra = rotation_angle/180*MY_PI;   // �Ƕ�ת����
    float cosa = cos(ra);
    float sina = sin(ra);
	//PS����X��Z����ת����ת����ͬ����Y�����ת�������в�ͬ��sin��-sin��λ�ûụ��
    model << 
        cosa, -sina, 0, 0,
        sina, cosa,  0, 0,
        0,    0,     1, 0,
        0,    0,     0, 1;
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,float n, float f)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
    float t = tan((eye_fov/360)*MY_PI)*(abs(n)); //top
    float r = t/aspect_ratio;

    Eigen::Matrix4f Mp; // ͸�Ӿ���

	//͸��ͼ������ԶС���Ǹ���׶  �˾�����һ����ʽ
    Mp << 
        n, 0, 0,   0,
        0, n, 0,   0,
        0, 0, n+f, -n*f,
        0, 0, 1,   0;
    Eigen::Matrix4f Mo_tran;//ƽ�ƾ���
    Mo_tran <<
        1, 0, 0, 0,
        0, 1, 0, 0,  //b=-t;
        0, 0, 1, -(n+f)/2 ,
        0, 0, 0, 1;
    Eigen::Matrix4f Mo_scale;//���ž���
    Mo_scale << 
        1/r,     0,       0,       0,
        0,       1/t,     0,       0,
        0,       0,       2/(n-f), 0,	// �������ŵı���Ϊ (1+1)/ĳһά�ȵĵ�ǰֵ
        0,       0,       0,       1;
    projection = (Mo_scale*Mo_tran)* Mp; // ͶӰ����
    //����һ��Ҫע��˳����͸��������;����������ƽ�������ţ���������������һ��ֱ�ߣ�
    return projection;
}

// �� main.cpp �У�����ģ����ͼ�ι��ߡ�
int main(int argc, const char** argv)
{
	float angle = 0;
	bool command_line = false;
	std::string filename = "output.png";
	// �������������ڡ�
	if (argc >= 3) {
		// ���ñ�־λ��
		command_line = true;
		// �õ���ת�Ƕȡ�
		angle = std::stof(argv[2]); // -r by default
		// ���ָ�������ͼƬ�ļ���������ļ�����
		if (argc == 4) {
			filename = std::string(argv[3]);
		}
		// ���û��ָ�����ͼƬ�ļ�������������
		else {
			return 0;
		}
	}
	// �������ȶ����˹�դ�������ʵ����Ȼ�����������Ҫ�ı�����
	rst::rasterizer objRasterizer(700, 700);

	Eigen::Vector3f eye_pos = { 0, 0, 5 };
	// Ȼ�����ǵõ�һ���������������Ӳ���������� (�벻Ҫ�޸���)��
	// ������������εĶ������ꡣ
	std::vector<Eigen::Vector3f> pos{ { 2, 0, -2 }, { 0, 2, -2 }, { -2, 0, -2 } };
	// ������������εĶ���˳��
	std::vector<Eigen::Vector3i> ind{ { 0, 1, 2 } };

	// ��һ�������εĶ�����������դ��������������������ڹ�դ�����е���š�
	// һ����դ�������Ի��ƶ�������Ρ���������ֻ������һ�������ζ��ѡ�
	rst::pos_buf_id pos_id = objRasterizer.load_positions(pos);
	// ��һ�������εĶ���˳������դ��������������������ڹ�դ�����е���š�
	// һ����դ�������Ի��ƶ�������Ρ���������ֻ������һ�������ζ��ѡ�
	rst::ind_buf_id ind_id = objRasterizer.load_indices(ind);

	int key = 0;
	int frame_count = 0;

	if (command_line) {
		objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);
		// ���������ϣ����Ƕ����������ֱ����ģ�͡���ͼ��ͶӰ����ĺ�����
		// ÿһ���������᷵����Ӧ�ľ���
		// ���ţ������������ķ���ֵ�ᱻ set_model(), set_view() �� set_projection() �������������դ�����С�
		objRasterizer.set_model(get_model_matrix(angle));
		objRasterizer.set_view(get_view_matrix(eye_pos));
		objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		// ��󣬹�դ��������Ļ����ʾ���任�Ľ����
		objRasterizer.draw(pos_id, ind_id, rst::Primitive::Triangle);
		cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);

		cv::imwrite(filename, image);

		return 0;
	}

	while (key != 27) {
		objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);

		objRasterizer.set_model(get_model_matrix(angle));
		objRasterizer.set_view(get_view_matrix(eye_pos));
		objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		objRasterizer.draw(pos_id, ind_id, rst::Primitive::Triangle);

		cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
		image.convertTo(image, CV_8UC3, 1.0f);
		cv::imshow("image", image);
		key = cv::waitKey(10);

		std::cout << "frame count: " << frame_count++ << '\n';

		if (key == 'a') {
			angle += 10;
		}
		else if (key == 'd') {
			angle -= 10;
		}
	}

	return 0;
}
