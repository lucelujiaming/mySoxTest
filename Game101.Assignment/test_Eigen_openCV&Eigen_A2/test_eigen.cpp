// clang-format off
#include <iostream>
#include <opencv2/opencv.hpp>
#include "rasterizer.hpp"
#include "global.hpp"
#include "Triangle.hpp"

// sconstexpr 
double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1,0,0,-eye_pos[0],
                 0,1,0,-eye_pos[1],
                 0,0,1,-eye_pos[2],
                 0,0,0,1;

    view = translate*view;

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

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();
	float t = tan((eye_fov / 360)*MY_PI)*(abs(zNear)); //top
    float r = t/aspect_ratio;

	// ͸�Ӿ���
	Eigen::Matrix4f matrixPerspective;
	//͸��ͼ������ԶС���Ǹ���׶  �˾�����һ����ʽ
	matrixPerspective <<
		zNear,     0,            0,             0,
		    0, zNear,            0,             0,
		    0,     0, zNear + zFar, -zNear * zFar,
            0,     0,            1,             0;

	//ƽ�ƾ���
	Eigen::Matrix4f matrixTranslate;
	matrixTranslate <<
        1, 0, 0,                   0,
        0, 1, 0,                   0,  //b=-t;
		0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0,                   1;

	//���ž���
	Eigen::Matrix4f matrixScale;
	matrixScale <<
        1/r,   0,                  0,    0,
          0, 1/t,                  0,    0,
		  0,   0, 2 / (zNear - zFar),    0,	// �������ŵı���Ϊ (1+1)/ĳһά�ȵĵ�ǰֵ
          0,   0,                  0,    1;
	projection = (matrixScale * matrixTranslate)* matrixPerspective; // ͶӰ����
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
    if (argc == 2)
    {
		// ���ñ�־λ��
        command_line = true;
		// ���ָ�������ͼƬ�ļ���������ļ�����
        filename = std::string(argv[1]);
    }

	// �������ȶ����˹�դ�������ʵ����Ȼ�����������Ҫ�ı�����
    rst::rasterizer objRasterizer(700, 700);

    Eigen::Vector3f eye_pos = {0,0,5};

	// Ȼ�����ǵõ������������������Ӳ���������� (�벻Ҫ�޸���)��
	// ���������������εĶ������ꡣ

    std::vector<Eigen::Vector3f> pos
            {
                    {2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}
            };
	// ���������������εĶ���˳��
    std::vector<Eigen::Vector3i> ind
            {
                    {0, 1, 2},
                    {3, 4, 5}
            };
	// ���������������ε���ɫ��
    std::vector<Eigen::Vector3f> cols
            {
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0}
            };

 	// ��һ�������εĶ�����������դ��������������������ڹ�դ�����е���š�
	// һ����դ�������Ի��ƶ�������Ρ�
    rst::pos_buf_id pos_id = objRasterizer.load_positions(pos);
	// ��һ�������εĶ���˳������դ��������������������ڹ�դ�����е���š�
	// һ����դ�������Ի��ƶ�������Ρ�
	rst::ind_buf_id ind_id = objRasterizer.load_indices(ind);
	// ��һ�������εĶ�����ɫ�����դ��������������������ڹ�դ�����е���š�
	// һ����դ�������Ի��ƶ�������Ρ�
	rst::col_buf_id col_id = objRasterizer.load_colors(cols);

    int key = 0;
    int frame_count = 0;

	if (command_line) {
		// ���֡���嶯̬�������Ȼ��嶯̬�����е����ݡ�
        objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);
		// ���������ϣ����Ƕ����������ֱ����ģ�͡���ͼ��ͶӰ����ĺ�����
		// ÿһ���������᷵����Ӧ�ľ���
		// ���ţ������������ķ���ֵ�ᱻ set_model(), set_view() �� set_projection() �������������դ�����С�
        objRasterizer.set_model(get_model_matrix(angle));
        objRasterizer.set_view(get_view_matrix(eye_pos));
        objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		// ��󣬹�դ��������Ļ����ʾ���任�Ľ����
        objRasterizer.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

		// �ѱ任���ת����ͼƬ��
        cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		// �ѱ任��������ͼƬ��
        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
		// ���֡���嶯̬�������Ȼ��嶯̬�����е����ݡ�
        objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);

		// ���������ϣ����Ƕ����������ֱ����ģ�͡���ͼ��ͶӰ����ĺ�����
		// ÿһ���������᷵����Ӧ�ľ���
		// ���ţ������������ķ���ֵ�ᱻ set_model(), set_view() �� set_projection() �������������դ�����С�
        objRasterizer.set_model(get_model_matrix(angle));
        objRasterizer.set_view(get_view_matrix(eye_pos));
        objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		// ��󣬹�դ��������Ļ����ʾ���任�Ľ����
        objRasterizer.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

		// �ѱ任���ת����ͼƬ��
        cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		// ��ʾ�任�����
        cv::imshow("image", image);
		// �ȴ���������
        key = cv::waitKey(10);
        std::cout << "frame count: " << frame_count++ << '\n';
		// ʹ��A��D��������������ת�Ƕȡ�
		if (key == 'a') {
			angle += 10;
			std::cout << "angle: " << angle << '\n';
		}
		else if (key == 'd') {
			angle -= 10;
			std::cout << "angle: " << angle << '\n';
		}
    }

    return 0;
}
// clang-format on