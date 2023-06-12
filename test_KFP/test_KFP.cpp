// test_KFP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
 
double frand()
{
	return 2 * ((rand() / (double)RAND_MAX) - 0.5);//�������
}


//1. �ṹ�����Ͷ���
typedef struct 
{ 
    double LastP;  // �ϴι���Э�����ʼ��ֵΪ0.02
    double Now_P;  // ��ǰ����Э�����ʼ��ֵΪ0
    double out;    // �������˲����������ʼ��ֵΪ0
    double Kg;     // ���������棬��ʼ��ֵΪ0
    double Q;      // ��������Э�����ʼ��ֵΪ0.001
    double R;      // �۲�����Э�����ʼ��ֵΪ0.543
}KFP;              // Kalman Filter parameter

//2. �Ը߶�Ϊ�� ���忨�����ṹ�岢��ʼ������
KFP KFP_height={0.02,0,0,0,0.001,0.543};

/** * �������˲��� 
    * @ param KFP 
    *    kfp �������ṹ����� 
    *    float input ��Ҫ�˲��Ĳ����Ĳ���ֵ�����������Ĳɼ�ֵ�� 
    * @return �˲���Ĳ���������ֵ�� 
*/
double kalmanFilter(KFP *kfp,double input)
{ 
	// 1 - ��������ơ�
	// 2 - ��������
	// Ԥ��Э����̣�kʱ��ϵͳ����Э���� = k-1ʱ�̵�ϵͳЭ���� + ��������Э����
	kfp->Now_P = kfp->LastP + kfp->Q;
	// 3 - �󿨶������档
	// ���������淽�̣����������� = kʱ��ϵͳ����Э���� / ��kʱ��ϵͳ����Э���� + �۲�����Э���
	kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);
	// 4 - �������ơ�
	// ��������ֵ���̣�kʱ��״̬����������ֵ = ״̬������Ԥ��ֵ + ���������� * ������ֵ - ״̬������Ԥ��ֵ��
	kfp->out = kfp->out + kfp->Kg * (input -kfp->out);//��Ϊ��һ�ε�Ԥ��ֵ������һ�ε����ֵ
	// 5 - �������Э����
	// Э�����: ���ε�ϵͳЭ����� kfp->LastP ����һ������׼����
	kfp->LastP = (1-kfp->Kg) * kfp->Now_P;
	return kfp->out;
}

#define   ACTUAL_VALUE                100
#define   MEASUREMENT_INACCURACY      4

int main(int argc, char* argv[])
{
	/** *���ÿ������˲��� ʵ�� */
	double height;
	double kalman_height = 0.0;
	// ʹ���״β���ֵ��Ϊ��ֵ��
	KFP_height.out = ACTUAL_VALUE + frand() * MEASUREMENT_INACCURACY ;
	printf("(kalman_height, error) = (%+.6f, %+.6f)\r\n", 
			KFP_height.out, (KFP_height.out - ACTUAL_VALUE));
	for (int i = 0; i < 10000; i++)
	{
		height = ACTUAL_VALUE + frand() * MEASUREMENT_INACCURACY ; // ����ֵ
		kalman_height = kalmanFilter(&KFP_height, height);
		if(i % 100 == 0)
			printf("(height, error) = (%+.6f, %+.6f) and (kalman_height[%04d], error) = (%+.6f, %+.6f)\r\n", 
				   height, (height - ACTUAL_VALUE), 
				i, kalman_height, (kalman_height - ACTUAL_VALUE));
	}
	
	printf("Hello World!\n");
	return 0;
}

