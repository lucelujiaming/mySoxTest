#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// PID控制算法的C语言实现三 位置型PID的C语言实现
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
    printf("PID_init begin \n");
    pid.SetSpeed=0.0;
    pid.ActualSpeed=0.0;
    pid.err=0.0;
    pid.err_last=0.0;
    pid.voltage=0.0;
    pid.integral=0.0;
    pid.Kp=0.2;
    pid.Ki=0.015;
    pid.Kd=0.2;
    printf("PID_init end \n");
}

/***************************************************************
 * PID物理解释。
 *    任何一个系统，相比于我们需要它达到的值，
 *    必定有误差，且系统内部有阻力、有惯性。
 *    本质上我们就是在控制这个系统的：
 *      1、误差 2、阻力 3、惯性。
 ***************************************************************
 * 以让飞轮转动到指定位置为例。
 * 现在有个静止的飞轮，飞轮上面有个竖直朝下的箭头。
 * 我们准备把箭头顺时针转动，指到竖直向上12点的位置。
 * 假设有多个时刻：t1 t2 t3 t4 t5 t6 t7。
 * 1. A对应误差。每个时刻都对应一个位置，
 *    t7为最终在12点的位置，
 *    每个时刻和t7之间有一个差值error，
 *    差值error越大，可以给的力气越大，
 *    没有差值的之后，就可以不给力气
 *    （当然在t7的时候，就可以不给力气了，
 *     因为已经达到目的了）。
 * 2. B对应阻力。因为轴有阻力，按照A方法，根本到不了12点，
 *    可能在11点的时候，指针就转不动了。
 *    因此上我们必须给一个力F3，压过这个阻力。
 *    那么这个更大的力F3是多少呢？
 *    我们可以把所有时刻指针的位置和12点的差值加起来，
 *    搞一个积分出来，如果指针因为阻力无法到达12点，
 *    这个差值会一直加加加加加，加到无穷大，
 *    到无穷大这段时间内，总有一个瞬间，
 *    力F3会压过轴的阻力，让飞轮转起来，
 *    最后指向12点的时候，这个差值为0，
 *    任你再累加力F3也不会发生变化了。 
 * 3. C对应惯性。我们应该在飞轮快要到达12点的时候，
 *    比如说11点58分的时候，我们就需要提前降低
 *    乃至撤销这个力F1，让飞轮利用它的惯性，
 *    自己甩到12点的位置。
 *    对于飞轮来说，惯性不就是等效于飞轮的转速嘛~
 *    飞轮的转速不就是第一个时刻和12点的差
 *    减去第二个时刻和12点的差，
 *    两个差值一减（就是飞轮t1到t2转了的角度），
 *    就可以得到速度（位置采样时间需要保持一致）
 ***************************************************************/
float PID_realize(float speed){
    pid.SetSpeed = speed;
    // 偏差err(K) = rin(K) - rout(K);
    pid.err = pid.SetSpeed - pid.ActualSpeed;
    // 积分环节用加和的形式表示，即err(K)+err(K+1)+……;
    pid.integral += pid.err;
    pid.voltage = 
              // A对应误差。差值error越大，可以给的力气越大。
              pid.Kp * pid.err
              // B对应阻力。积分值越大，可以给的力气越大。
            + pid.Ki * pid.integral 
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

int main(int argc, char ** argv) 
{
    printf("System begin \n");
    PID_init();
    int count=0;
    while(count<1000)
    {
        float speed = PID_realize(200.0);
        printf("%f\n",speed);
        count++;
    }
return 0;
}
