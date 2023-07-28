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

//在此函数中，你只需要实现三维中绕 z 轴旋转的变换矩阵，
//而不用处理平移与缩放。
Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    float ra = rotation_angle/180*MY_PI;   // 角度转弧度
    float cosa = cos(ra);
    float sina = sin(ra);
	//PS：绕X、Z轴旋转的旋转矩阵同理，绕Y轴的旋转矩阵略有不同，sin和-sin的位置会互换
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

	// 透视矩阵
	Eigen::Matrix4f matrixPerspective;
	//透视图，近大远小，是个视锥  此矩阵是一个公式
	matrixPerspective <<
		zNear,     0,            0,             0,
		    0, zNear,            0,             0,
		    0,     0, zNear + zFar, -zNear * zFar,
            0,     0,            1,             0;

	//平移矩阵
	Eigen::Matrix4f matrixTranslate;
	matrixTranslate <<
        1, 0, 0,                   0,
        0, 1, 0,                   0,  //b=-t;
		0, 0, 1, -(zNear + zFar) / 2,
        0, 0, 0,                   1;

	//缩放矩阵
	Eigen::Matrix4f matrixScale;
	matrixScale <<
        1/r,   0,                  0,    0,
          0, 1/t,                  0,    0,
		  0,   0, 2 / (zNear - zFar),    0,	// 所以缩放的倍数为 (1+1)/某一维度的当前值
          0,   0,                  0,    1;
	projection = (matrixScale * matrixTranslate)* matrixPerspective; // 投影矩阵
    //这里一定要注意顺序，先透视再正交;正交里面先平移再缩放；否则做出来会是一条直线！
    return projection;
}

// 在 main.cpp 中，我们模拟了图形管线。
int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";
	// 如果输入参数存在。
    if (argc == 2)
    {
		// 设置标志位。
        command_line = true;
		// 如果指定了输出图片文件名。获得文件名。
        filename = std::string(argv[1]);
    }

	// 我们首先定义了光栅化器类的实例，然后设置了其必要的变量。
    rst::rasterizer objRasterizer(700, 700);

    Eigen::Vector3f eye_pos = {0,0,5};

	// 然后我们得到两个带有三个顶点的硬编码三角形 (请不要修改它)。
	// 给出这两个三角形的顶点坐标。

    std::vector<Eigen::Vector3f> pos
            {
                    {2, 0, -2},
                    {0, 2, -2},
                    {-2, 0, -2},
                    {3.5, -1, -5},
                    {2.5, 1.5, -5},
                    {-1, 0.5, -5}
            };
	// 给出这两个三角形的顶点顺序。
    std::vector<Eigen::Vector3i> ind
            {
                    {0, 1, 2},
                    {3, 4, 5}
            };
	// 给出这两个三角形的颜色。
    std::vector<Eigen::Vector3f> cols
            {
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {217.0, 238.0, 185.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0},
                    {185.0, 217.0, 238.0}
            };

 	// 将一个三角形的顶点坐标放入光栅化器。返回这个三角形在光栅化器中的序号。
	// 一个光栅化器可以绘制多个三角形。
    rst::pos_buf_id pos_id = objRasterizer.load_positions(pos);
	// 将一个三角形的顶点顺序放入光栅化器。返回这个三角形在光栅化器中的序号。
	// 一个光栅化器可以绘制多个三角形。
	rst::ind_buf_id ind_id = objRasterizer.load_indices(ind);
	// 将一个三角形的顶点颜色放入光栅化器。返回这个三角形在光栅化器中的序号。
	// 一个光栅化器可以绘制多个三角形。
	rst::col_buf_id col_id = objRasterizer.load_colors(cols);

    int key = 0;
    int frame_count = 0;

	if (command_line) {
		// 清除帧缓冲动态数组和深度缓冲动态数组中的内容。
        objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);
		// 在主函数上，我们定义了三个分别计算模型、视图和投影矩阵的函数，
		// 每一个函数都会返回相应的矩阵。
		// 接着，这三个函数的返回值会被 set_model(), set_view() 和 set_projection() 三个函数传入光栅化器中。
        objRasterizer.set_model(get_model_matrix(angle));
        objRasterizer.set_view(get_view_matrix(eye_pos));
        objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		// 最后，光栅化器在屏幕上显示出变换的结果。
        objRasterizer.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

		// 把变换结果转换成图片。
        cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		// 把变换结果保存成图片。
        cv::imwrite(filename, image);

        return 0;
    }

    while(key != 27)
    {
		// 清除帧缓冲动态数组和深度缓冲动态数组中的内容。
        objRasterizer.clear(rst::Buffers::Color | rst::Buffers::Depth);

		// 在主函数上，我们定义了三个分别计算模型、视图和投影矩阵的函数，
		// 每一个函数都会返回相应的矩阵。
		// 接着，这三个函数的返回值会被 set_model(), set_view() 和 set_projection() 三个函数传入光栅化器中。
        objRasterizer.set_model(get_model_matrix(angle));
        objRasterizer.set_view(get_view_matrix(eye_pos));
        objRasterizer.set_projection(get_projection_matrix(45, 1, 0.1, 50));

		// 最后，光栅化器在屏幕上显示出变换的结果。
        objRasterizer.draw(pos_id, ind_id, col_id, rst::Primitive::Triangle);

		// 把变换结果转换成图片。
        cv::Mat image(700, 700, CV_32FC3, objRasterizer.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR);
		// 显示变换结果。
        cv::imshow("image", image);
		// 等待按键输入
        key = cv::waitKey(10);
        std::cout << "frame count: " << frame_count++ << '\n';
		// 使用A和D两个按键调整旋转角度。
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