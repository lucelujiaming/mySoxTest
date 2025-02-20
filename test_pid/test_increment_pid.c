#include<stdio.h>
#include<stdlib.h>

// PID控制算法的C语言实现四 增量型PID的C语言实现
struct _pid{
    float SetSpeed;      // 定义设定值
    float ActualSpeed;   // 定义实际值
    float err;           // 定义偏差值
    float err_next;      // 定义上一个偏差值
    float err_last;      // 定义上两个偏差值
    float Kp,Ki,Kd;      // 定义比例、积分、微分系数。
                         // 这里的比例、积分、微分，
                         // 对应于PID的误差，阻力，惯性。
}pid;

void PID_init(){
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.err_next=0.0;
    pid.Kp=0.2;
    pid.Ki=0.015;
    pid.Kd=0.2;
}

float PID_realize(float speed){
    pid.SetSpeed = speed;
    // 偏差err(K) = rin(K) - rout(K);
    pid.err = pid.SetSpeed - pid.ActualSpeed;
    // 所谓的增量型就是计算需要增加的量。
	// 也就是公式中的u(K) - u(K - 1)。
	// 因此上下面的ABC三个部分都是用系数乘以增量。
    float incrementSpeed = 
              // A对应误差。差值error越大，可以给的力气越大。
              pid.Kp * 
			      // 比例部分的增量就是本次误差减去上一次的误差。
			      (pid.err-pid.err_next) 
              // B对应阻力。积分值越大，可以给的力气越大。
		      // 积分部分的增量就是本次误差。
            + pid.Ki * pid.err
              // C对应惯性。对于飞轮来说，就是转速。
            + pid.Kd * 
			      // 微分部分的增量就是：
				  // 本次误差减去上一次的误差得到的误差和
				  // 上一次的误差减去上两次误差得到的误差的差。
				  // 也就是下面的算式。即：
				  //   [err(k) - err(k - 1)] - [err(k - 1) - err(k - 2)] 
				  // = [err(k) - 2 * err(k - 1) + err(k - 2)] 
				  (pid.err - 2 * pid.err_next + pid.err_last);


    // 更新本次的上一次偏差用于下一次计算中的上两次误差。
    pid.err_last=pid.err_next;
    // 更新本次的偏差用于下一次计算中的上一次误差。
    pid.err_next=pid.err;
    // 更新控制结果。
	pid.ActualSpeed += incrementSpeed;
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

