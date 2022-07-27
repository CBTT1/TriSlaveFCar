#include "headfile.h"
#include "control.h"
#include "device.h"
#include "calculate.h"
#include <math.h>
#include <stdlib.h>

uint8 ControlFlag = 1;
struct PidParam SpeedParam = {9,5,0,0,0,0};
struct PidParam CoeParam = {5,0,10,0,0,0};
struct CarParam LeftMotorSpeed,RightMotorSpeed,Steer;
struct SpeedGoal SlowSpeed = {0,0,0,300,300},BalanceSpeed = {365,365,335,300,300},FastSpeed = {335,335,335,300,300};

int16 coe_pd(struct PidParam* Coe,int16 AmplitudeMax)
{
    float Temp;
    // Coe->Error = ((float)(AD1-AD4))/((AD2+AD3)*sqrt((AD2+AD3)))*10000; 
    Coe->Error = 5*(sqrt(AD1) - sqrt(AD4))/(AD1 + AD4)*10000;
    oled_printf_int32(16,2,Coe->Error,4);
    Temp = Coe->Kp * Coe-> Error  + Coe->Kd * (Coe->Error - Coe->LastError);
    if(Temp >= AmplitudeMax) Coe->Output = AmplitudeMax;
    else if(Temp <= -1*AmplitudeMax)  Coe->Output = -1*AmplitudeMax;
    else Coe->Output = Temp;
    Coe->LastError = Coe->Error;
    return Coe->Output;
}


int16 motor_pi(struct PidParam* Motor,int16 CurrentState,int16 GoalState,int16 AmplitudeMax)
{
    float Temp;
    Motor->Error = GoalState - CurrentState;
    // if(CurrentState < 100)
    //     Temp = Motor->Kp*(Motor->Error - Motor->LastError) + (Motor->Ki/2)*Motor->Error;
    // else
    Temp = Motor->Kp*(Motor->Error - Motor->LastError) + Motor->Ki*Motor->Error;
    Motor->Output +=  Temp;
    if(Motor->Output >= AmplitudeMax) Motor->Output = AmplitudeMax;
    else if(Motor->Output <= -1*AmplitudeMax) Motor->Output = -1*AmplitudeMax;
    Motor->LastError = Motor->Error;
    return Motor->Output;
}

void main_control()
{
    int16 LeftPwm,RightPwm;
    int16 CoeOut;


    // LeftMotorSpeed.Goal = SlowSpeed.TurnSpeed;
    // RightMotorSpeed.Goal = SlowSpeed.TurnSpeed;

    CoeOut = coe_pd(&CoeParam,8000);

    // if(CoeOut> 0) //左边靠前，左减右加！
    // {
    //     LeftMotorSpeed.Goal = LeftMotorSpeed.Goal - CoeOut; 
    //     // RightMotorSpeed.Goal = RightMotorSpeed.Goal + CoeOut/2;
    // }
    // else
    // {
    //     // LeftMotorSpeed.Goal = LeftMotorSpeed.Goal - CoeOut/2; 
    //     RightMotorSpeed.Goal = RightMotorSpeed.Goal + CoeOut;
         
    // }

    // LeftPwm  = motor_pi(&SpeedParam,LeftMotorSpeed.Current,LeftMotorSpeed.Goal,10000) - CoeOut/2;
    // RightPwm = motor_pi(&SpeedParam,RightMotorSpeed.Current,RightMotorSpeed.Goal,10000) + CoeOut/2;

    LeftPwm  = 2000 - CoeOut/2;
    RightPwm = 2000 + CoeOut/2;
    

    pwm_duty(RIGHT_FRONT, RightPwm >0?RightPwm:0);
    pwm_duty(RIGHT_BACK,RightPwm < 0 ? -1*RightPwm:0);//右后
    pwm_duty(LEFT_FRONT,LeftPwm > 0 ? LeftPwm:0);//左前
    pwm_duty(LEFT_BACK,LeftPwm < 0 ? -1*LeftPwm:0); //左后


    if(AD1 < 30 && AD2 < 30 && AD3 < 30 && AD4 < 30 )
    {
        pwm_duty(RIGHT_FRONT,0);
        pwm_duty(RIGHT_BACK,0);//右后
        pwm_duty(LEFT_FRONT,0);//左前
        pwm_duty(LEFT_BACK,0); //左后
    }
}
