#include<stdio.h>
#include<stdlib.h>

// PID控制算法的C语言实现五 积分分离的PID控制算法C语言实现
struct _pid{
    float SetSpeed;      // 定义设定值
    float ActualSpeed;   // 定义实际值
    float err;           // 定义偏差值
    float err_last;      // 定义上一个偏差值
    float Kp,Ki,Kd;      // 定义比例、积分、微分系数。
                         // 这里的比例、积分、微分，
                         // 对应于PID的误差，阻力，惯性。
    float voltage;       // 定义电压值（控制执行器的变量）
    float integral;      // 定义积分值
}pid;

void PID_init(){
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.voltage=0.0;
    pid.integral=0.0;
    pid.Kp=0.2;
    pid.Ki=0.015;
    pid.Kd=0.2;
}

float PID_realize(float speed){
    int index;
    pid.SetSpeed = speed;
    // 偏差err(K) = rin(K) - rout(K);
    pid.err = pid.SetSpeed - pid.ActualSpeed;
    // 在启动、结束或大幅度增减设定时，短时间内系统输出有很大的偏差，
    // 会造成PID运算的积分积累，
    // 导致控制量超过执行机构可能允许的最大动作范围对应极限控制量，
    // 从而引起较大的超调，甚至是震荡，这是绝对不允许的。
	// 为了克服这一问题，引入了积分分离的概念，其基本思路是：
	// 当被控量与设定值偏差较大时，取消积分作用; 
    if(abs(pid.err) > 200)
    {
        index=0;
	// 当被控量接近给定值时，引入积分控制，以消除静差，提高精度。
    }else{
        index=1;
        pid.integral += pid.err;
    }
    pid.voltage = 
              // A对应误差。差值error越大，可以给的力气越大。
              pid.Kp * pid.err
              // B对应阻力。积分值越大，可以给的力气越大。
            + index * pid.Ki * pid.integral
              // C对应惯性。对于飞轮来说，就是转速。
            + pid.Kd * 
              // 也就是第一个时刻和12点的差
              // 减去第二个时刻和12点的差，
              // 微分环节用斜率的形式表示，即[err(K)-err(K-1)]/T;
              (pid.err-pid.err_last);    // 算法具体实现过程
    // 更新偏差用于下一次。
    pid.err_last = pid.err;
    // 更新控制结果。
    pid.ActualSpeed = pid.voltage*1.0;
    return pid.ActualSpeed;
}

int main(){
    PID_init();
    int count=0;
    while(count<1000)
    {
        float speed=PID_realize(200.0);
        printf("%f\n",speed);
        count++;
    }
    return 0;
}

