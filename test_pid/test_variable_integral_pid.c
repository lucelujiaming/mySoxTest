#include<stdio.h>
#include<stdlib.h>

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
    float umax;          // 最大极限值
    float umin;          // 最小极限值
}pid;

void PID_init(){
    printf("PID_init begin \n");
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.voltage=0.0;
    pid.integral=0.0;
    pid.Kp=0.4;
    pid.Ki=0.2;     //增加了积分系数
    pid.Kd=0.2;
    pid.umax=400;
    pid.umin=-200;
    printf("PID_init end \n");
}

float PID_realize(float speed){
    float index;
    pid.SetSpeed=speed;
    // 偏差err(K) = rin(K) - rout(K);
    pid.err=pid.SetSpeed-pid.ActualSpeed;

    // 系统对于积分项的要求是，系统偏差大时，积分作用应该减弱甚至是全无，
    // 而在偏差小时，则应该加强。积分系数取大了会产生超调，甚至积分饱和，
    // 取小了又不能短时间内消除静差。
    // 因此，根据系统的偏差大小改变积分速度是有必要的。
    // 变积分PID的基本思想是设法改变积分项的累加速度，
    // 使其与偏差大小相对应：偏差越大，积分越慢; 偏差越小，积分越快。
    // 
    // 这里给积分系数前加上一个比例值index：
	// 当abs(err)>200时，index=0;
    if(abs(pid.err)>200)           //变积分过程
    {
        index = 0.0;
    }
    // 当abs(err)<180时，index=1;
	else if(abs(pid.err)<180)
	{
        index = 1.0;
        pid.integral += pid.err;
    // 当180<abs(err)<200时，index=（200-abs(err)）/20;
    }else{
        index = (200 - abs(pid.err))/20;
        pid.integral += pid.err;
    }
    pid.voltage = 
              // A对应误差。差值error越大，可以给的力气越大。
              pid.Kp * pid.err
              // B对应阻力。积分值越大，可以给的力气越大。
			  // 此处使用的是矩形积分
            + index * pid.Ki * pid.integral
              // C对应惯性。对于飞轮来说，就是转速。
            + pid.Kd * 
              // 也就是第一个时刻和12点的差
              // 减去第二个时刻和12点的差，
              // 微分环节用斜率的形式表示，即[err(K)-err(K-1)]/T;
              (pid.err-pid.err_last);
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

