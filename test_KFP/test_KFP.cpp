// test_KFP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
 
double frand()
{
	return 2 * ((rand() / (double)RAND_MAX) - 0.5);//随机噪声
}


//1. 结构体类型定义
typedef struct 
{ 
    double LastP;  // 上次估算协方差，初始化值为0.02
    double Now_P;  // 当前估算协方差，初始化值为0
    double out;    // 卡尔曼滤波器输出，初始化值为0
    double Kg;     // 卡尔曼增益，初始化值为0
    double Q;      // 过程噪声协方差，初始化值为0.001
    double R;      // 观测噪声协方差，初始化值为0.543
}KFP;              // Kalman Filter parameter

//2. 以高度为例 定义卡尔曼结构体并初始化参数
KFP KFP_height={0.02,0,0,0,0.001,0.543};

/** * 卡尔曼滤波器 
    * @ param KFP 
    *    kfp 卡尔曼结构体参数 
    *    float input 需要滤波的参数的测量值（即传感器的采集值） 
    * @return 滤波后的参数（最优值） 
*/
double kalmanFilter(KFP *kfp,double input)
{ 
	// 1 - 求先验估计。
	// 2 - 求先验误差。
	// 预测协方差方程：k时刻系统估算协方差 = k-1时刻的系统协方差 + 过程噪声协方差
	kfp->Now_P = kfp->LastP + kfp->Q;
	// 3 - 求卡尔曼增益。
	// 卡尔曼增益方程：卡尔曼增益 = k时刻系统估算协方差 / （k时刻系统估算协方差 + 观测噪声协方差）
	kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
	// 4 - 求后验估计。
	// 更新最优值方程：k时刻状态变量的最优值 = 状态变量的预测值 + 卡尔曼增益 * （测量值 - 状态变量的预测值）
	kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//因为这一次的预测值就是上一次的输出值
	// 5 - 更新误差协方差
	// 协方差方程: 本次的系统协方差付给 kfp->LastP 威下一次运算准备。
	kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
	return kfp->out;
}

#define   ACTUAL_VALUE                100
#define   MEASUREMENT_INACCURACY      4

int main(int argc, char* argv[])
{
	/** *调用卡尔曼滤波器 实践 */
	double height;
	double kalman_height = 0.0;
	// 使用首次测量值作为初值。
	KFP_height.out = ACTUAL_VALUE + frand() * MEASUREMENT_INACCURACY ;
	printf("(kalman_height, error) = (%+.6f, %+.6f)\r\n", 
			KFP_height.out, (KFP_height.out - ACTUAL_VALUE));
	for (int i = 0; i < 10000; i++)
	{
		height = ACTUAL_VALUE + frand() * MEASUREMENT_INACCURACY ; // 测量值
		kalman_height = kalmanFilter(&KFP_height, height);
		if(i % 100 == 0)
			printf("(height, error) = (%+.6f, %+.6f) and (kalman_height[%04d], error) = (%+.6f, %+.6f)\r\n", 
				   height, (height - ACTUAL_VALUE), 
				i, kalman_height, (kalman_height - ACTUAL_VALUE));
	}
	
	printf("Hello World!\n");
	return 0;
}

