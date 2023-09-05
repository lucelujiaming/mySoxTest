#include "rtweekend.h"


#include "camera.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>
#include "material.h"
#include "moving_sphere.h"
#include "aarect.h"
// 添加了一个const color& background参数，作为背景色。
// color ray_color(const ray& r, const hittable& world, int depth) {
color ray_color(const ray& r, const color& background, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);
    // 如果没有撞击到物体。
    // If the ray hits nothing, return the background color.
    if (!world.hit(r, 0.001, infinity, rec))
        return background;
	// 我们既然添加了光照，那么计算光照的时候也要将它加进去。
	// 1. 计算光照
    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
	// 如果发光材质不散射，那就直接返回这个发光材质的颜色。
	// 也就是说，你用手里发出的一束光照在一盏灯上，
	// 你手里发出的一束光将在后续的光线跟踪过程中，不再传递。
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;
	// 2. 否则，如果发光材质会散射，这需要把光照加进去。
    return emitted + attenuation * ray_color(scattered, background, world, depth-1);
}

hittable_list random_scene() {
    hittable_list world;

   //  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
   //  world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));
   auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
					// 将random_scene()产生的材质为漫反射的球体改为moving_sphere对象，并设定相应参数
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;
	
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
	// 生成两个棋盘纹理的随机球体
    objects.add(make_shared<sphere>(point3(0,-10, 0), 10, make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    // auto pertext = make_shared<noise_texture>();
	// 缩放比例设置为4
    auto pertext = make_shared<noise_texture>(4);
	// 生成两个perlin纹理的随机球体
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return objects;
}

// 绘制纹理贴图的球。
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

hittable_list simple_light() {
    hittable_list objects;
	// 创建一个柏林噪声纹理。
    auto pertext = make_shared<noise_texture>(4);
	// 添加两个球。
    objects.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0,2,0), 2, make_shared<lambertian>(pertext)));
	// 创建一个发光材质。
    auto difflight = make_shared<diffuse_light>(color(4,4,4));
	// 创建一个位于 z = k 平面的平板物体，并且采用发光材质。
	// 相当于一面窗户。
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

    return objects;
}

// 制造Cornell box场景
hittable_list cornell_box() {
    hittable_list objects;
	// 创建红色漫反射材质
    auto red   = make_shared<lambertian>(color(.65, .05, .05));
	// 创建白色漫反射材质
    auto white = make_shared<lambertian>(color(.73, .73, .73));
	// 创建绿色漫反射材质
    auto green = make_shared<lambertian>(color(.12, .45, .15));
	// 创建自发光材质
    auto light = make_shared<diffuse_light>(color(15, 15, 15));
	// 制造Cornell box
	// 左侧面板为绿色
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	// 右侧面板为红色
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	// 上方一盏灯。
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	// 上方面板为白色
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	// 下方面板为白色
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	// 背景面板为白色
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    return objects;
}

// 整个主函数包含四个部分。逻辑如下：
//   1. Image：这个部分定义了一些基本参数。包括生成图片的大小。
//             每一个像素采样了几根光线。还有就是光线反射次数。
//   2. World：这个部分是一个对象列表。也就是一个std::vector<hittable>。
// 			   每一个对象是一个hittable对象。包含一个hit函数用于判断是否和光线相交。
//   3. Camera：顾名思义，这个部分定义了视口。
//   4. Render：有了前面的设定，我们就可以进行绘制了。
//              步骤就是
//              1. 计算每一个像素。
//              2. 通过像素内多次采样的的方式抗锯齿。
//              2. 对于每一次采样，需要根据光线反射次数，把每一个像素的光照采样累加起来。
//                 计算函数就是ray_color函数。
//              4. 之后输出第二步的计算结果。因为前面的计算结果是采样累加值。
//                 因此上，结果需要除以光线反射次数。
//   经过上面的分析可以知道，这个的计算量是非常大的。
//   首先就是一帧图像包含很多的像素点。而每一个像素点都需要做多次采样。
//   而每一次采样都需要使用当前采样光线判断是否和物体相交。
//   并且每一次相交计算都需要模拟计算迭代光线跳跃过程。
int main() {

    // Image
	// 最终成像（图像）的宽高比
    auto aspect_ratio = 16.0 / 9.0;
	// 图像宽度（像素个数）
    int image_width = 400;
	// 图像高度（像素个数）
    // const int image_height = static_cast<int>(image_width / aspect_ratio);
    int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;

	point3 lookfrom;
	point3 lookat;
	auto vfov = 40.0;
	auto aperture = 0.0;
	// 默认背景色为黑色
    color background(0,0,0);
	
	switch (0) {
	    case 1:
	        world = random_scene();
            background = color(0.70, 0.80, 1.00);
	        lookfrom = point3(13,2,3);
	        lookat = point3(0,0,0);
	        vfov = 20.0;
	        aperture = 0.1;
	        break;
	
		// 显示两个球体
	    case 2:
	        world = two_spheres();
            background = color(0.70, 0.80, 1.00);
	        lookfrom = point3(13,2,3);
	        lookat = point3(0,0,0);
	        vfov = 20.0;
	        break;
		// 显示两个柏林噪声纹理纹理贴图的球体
        case 3:
            world = two_perlin_spheres();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
		// 显示一个纹理贴图的球体
        case 4:
            world = earth();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            break;
		// 显示两个被窗户透进来的光照亮的柏林噪声纹理纹理贴图的球体
        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0,0,0);
            lookfrom = point3(26,3,6);
            lookat = point3(0,2,0);
            vfov = 20.0;
            break; 
	    default:
		// // 制造Cornell box场景
        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0,0,0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;
	}
	
    // Camera 使用Camera类声明摄像机
	
	vec3 vup(0,1,0);
	auto dist_to_focus = 10.0;
	int image_height = static_cast<int>(image_width / aspect_ratio);
	
	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    // 从左上角遍历整个屏幕
	// j：竖向移动的横向扫描线，j的值从大到小。（输出像素是从上到下，与j的值相反）
    for (int j = image_height-1; j >= 0; --j) {
		// 剩余扫描线数
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		// i：横向移动的竖向扫描线，i的值从小到大。（输出像素是从左到右，与i的值一致）
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
			// 之前生成的图在球的边缘有很明显的锯齿，因为球和背景颜色很不一样，所以看起来很“走样”。
			// 所谓的反走样，其本质就是“调整像素明暗”，边缘像素的RGB调整为前景与背景的混合值，
			// 这样的话边缘就会产生过渡效果。
            // 因此上，可以通过像素内多次采样的的方式抗锯齿，采样点坐标用前面的随机数获得。
            // 在main函数中只是简单的将采样结果相加。
            for (int s = 0; s < samples_per_pixel; ++s) {
				// 以下两行相当于在像素左边一点点和上边一点点进行随机采样
                auto u = (i + random_double()) / (image_width-1);   // u：成像上像素的 x 坐标（小数）
                auto v = (j + random_double()) / (image_height-1);  // v：成像上像素的 y 坐标（小数）
				// 从相机位置打到采样处的光线（原点一样：光线.origin = 相机.origin）
                ray r = cam.get_ray(u, v);
				// 对成像上的每一个像素发出光线，打在场景上，并计算像素颜色.
				// 原本的像素颜色 += 光的颜色，最终累加，合成该像素的颜色
                pixel_color += ray_color(r, background, world, max_depth);
            }
            // 在最后write_color时再根据采样点个数进行平均，这样能够减少工作量。
            write_color(std::cout, pixel_color, samples_per_pixel);
       }
    }

    std::cerr << "\nDone.\n";
}
