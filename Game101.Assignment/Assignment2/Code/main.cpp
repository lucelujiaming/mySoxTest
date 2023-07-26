// clang-format off
#include <iostream>
#include <opencv2/opencv.hpp>
#include "rasterizer.hpp"
#include "global.hpp"
#include "Triangle.hpp"

constexpr double MY_PI = 3.1415926;

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

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
	rotation_angle = (rotation_angle / 180.0) * MY_PI;//角度转弧度
	Eigen::Matrix4f translate;
	translate << cos(rotation_angle), -sin(rotation_angle), 0, 0,
		 sin(rotation_angle), cos(rotation_angle), 0, 0,
		 0, 0, 1.0f, 0,
		 0, 0, 0, 1.0f;
	//PS：绕X、Z轴旋转的旋转矩阵同理，绕Y轴的旋转矩阵略有不同，sin和-sin的位置会互换
	model = translate * model;


    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, 
					float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

	//透视图，近大远小，是个视锥  此矩阵是一个公式
	Eigen::Matrix4f pto = Eigen::Matrix4f::Identity();//将透视矩阵挤压成正交矩阵
	pto << zNear, 0, 0, 0,
		0, zNear, 0, 0,
		0, 0, zNear + zFar, - zFar * zNear,
		0, 0, 1.0, 0;

	float halfAngle = (eye_fov / 2.0 / 180.0) * MY_PI; //视角的一半
	float top = -1.0f * zNear * tan(halfAngle);//y轴正方向值 = 显示视口的一半高度
	float right= top * aspect_ratio;
	float bottom = -top;
	float left = -right;

	//构造缩放矩阵，使视口大小等同窗口大小
	Eigen::Matrix4f m_s = Eigen::Matrix4f::Identity();
	m_s << 2 / (right - left), 0, 0, 0, //将中心视为原点，则窗口的三维方向值域均为[-1,1]
			0, 2 / (top - bottom), 0, 0,	//缩放的倍数为 期望值/当前值
			0, 0, 2 / (zNear - zFar), 0,	//所以缩放的倍数为 (1+1)/某一维度的当前值
			0, 0, 0, 1;

	//构造平移矩阵，将视口左下角移动到原点
	Eigen::Matrix4f m_t = Eigen::Matrix4f::Identity();
	
	//左下角的点原本为 （x_left,y_down，zNear）
	//注意！此时已经经过了缩放，所以左下角的点的位置已经变化
	//左下角的点现在为 （-1，-1，zNear）
	//即其实可以不用管x和y轴，比较尺寸已经和窗口匹配了
	//左侧+右侧或者上侧+下侧，结果都是0,但这里为了便于理解或者防止参数变动之后会产生的一系列变化还是选用公式的写法
	m_t << 1, 0, 0, -(left+right)/2.0f,					
			0, 1, 0, -(top+bottom)/2.0f,					
			0, 0, 1, -(zNear+zFar)/2.0f,
			0, 0, 0, 1;
	
	projection = m_s * m_t * pto * projection;


    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc == 2)
    {
        command_line = true;
        filename = std::string(argv[1]);
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0,0,5};


    std::vector<Eigen::Vector3f> pos
            {
                    {2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}
            };

    std::vector<Eigen::Vector3i> ind
            {
                    {0, 1, 2},
                    {3, 4, 5}
            };

    std::vector<Eigen::Vector3f> cols
            {
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0}
            };

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);
    auto col_id = r.load_colors(cols);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';
    }

    return 0;
}
// clang-format on