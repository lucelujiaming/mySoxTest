#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

class perlin {
    public:
        perlin() {
            ranfloat = new double[point_count];
			// 总随机序列由第一种方法生成，序列中的每一个元素均为0~1的随机数
            for (int i = 0; i < point_count; ++i) {
                ranfloat[i] = random_double();
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }

        ~perlin() {
            delete[] ranfloat;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }
		// 产生噪声值的函数
        double noise(const point3& p) const {
            auto i = static_cast<int>(4*p.x()) & 255;
            auto j = static_cast<int>(4*p.y()) & 255;
            auto k = static_cast<int>(4*p.z()) & 255;

            return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]]; 
        }

    private:
        static const int point_count = 256;
        double* ranfloat;
        int* perm_x;
        int* perm_y;
        int* perm_z;
		// 分量的随机序列生成方法，即，
        static int* perlin_generate_perm() {
            auto p = new int[point_count];
			// 初始序列为1-255，
            for (int i = 0; i < perlin::point_count; i++)
                p[i] = i;
			
            permute(p, point_count);

            return p;
        }
		
        static void permute(int* p, int n) {
			// 之后遍历整个序列，当前位置和一个随机生成的位置进行交换，已达到序列随机化
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

};

#endif

