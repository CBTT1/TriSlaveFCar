#include <math.h>
#include <stdlib.h>
#include "headfile.h"
#include "control.h"
#include "device.h"
#include "calculate.h"
#include "element_process.h"



uint8 ControlFlag = 1;
int16 RateKd = 11;//舵机pid
struct PidParam SpeedParam = {15,15,0,0,0,0};
struct PidParam SteerPid = {4.8,0,44,0,0,0},SteerStraight = {0.8,0,1.6,0,0,0},SteerTurn = {1.2,0,2.4,0,0,0};
struct PidParam CoeParam = {5,0,10,0,0,0};
struct CarParam LeftMotorSpeed,RightMotorSpeed,Steer;
struct SpeedGoal SlowSpeed = {320,320,320,300,300},BalanceSpeed = {365,365,335,300,300},FastSpeed = {335,335,335,300,300};
float ParamCoeBalance = 1.5;

int16 error_culculate()
{
    int16 A = AD1,B = AD4;
    if(RoundaboutFlag >0)
    {
        switch (RoundaboutFlag)
        {
            case 1:break;
            case 2:A = AD5;B = AD6;break;
            case 3:A = AD5;B = AD6;break;
            case 4:A = AD5;B = AD6;break;
            case 5:break;           
            default:break;
        }
    }
    else
    {

    } 
    return (int16)(((float)(A-B))/((AD2+AD3)*sqrt((AD2+AD3)))*1000);
}

int8 steer_pd(struct PidParam* Steer,int16 CurrentState,int16 GoalState,int16 AmplitudeMax)
{

    Steer->Error = GoalState - CurrentState;
    Steer->Output = Steer->Kp*Steer->Error + Steer->Kd*(Steer->Error - Steer->LastError);
    Steer->LastError = Steer->Error;

    if(RoundaboutFlag == 2)
    {
        if(RoundaboutDir == LEFT)
            Steer->Output = AmplitudeMax;
        else if(RoundaboutDir == RIGHT)
            Steer->Output = -1*AmplitudeMax;
    }
    if(ForkFlag == 1 || ForkFlag == 3)
    {
        if(ForkDir == LEFT)
            Steer->Output = -1*AmplitudeMax;
        else if(RoundaboutDir == RIGHT)
            Steer->Output = AmplitudeMax;
    }
    if(Steer->Output >= AmplitudeMax) Steer->Output = AmplitudeMax;
    else if(Steer->Output <= -1*AmplitudeMax) Steer->Output = -1*AmplitudeMax;
    return (int8)Steer->Output;
}

// int16 coe_pd(struct PidParam* Coe,int16 AmplitudeMax)
// {
//     float Temp;
//     // Coe->Error = ((float)(AD1-AD4))/((AD2+AD3)*sqrt((AD2+AD3)))*10000; 
//     Coe->Error = 5*(sqrt(AD1) - sqrt(AD4))/(AD1 + AD4)*10000;
//     oled_printf_int32(16,2,Coe->Error,4);
//     Temp = Coe->Kp * Coe-> Error  + Coe->Kd * (Coe->Error - Coe->LastError);
//     if(Temp >= AmplitudeMax) Coe->Output = AmplitudeMax;
//     else if(Temp <= -1*AmplitudeMax)  Coe->Output = -1*AmplitudeMax;
//     else Coe->Output = Temp;
//     Coe->LastError = Coe->Error;
//     return Coe->Output;
// }


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
    int8  SteerOutTemp;
    float  ScaleCoe;
    int16 Error;
    static uint8 NumControlTime = 0;

    // SteerPid.Kp = SteerTurn.Kp;
    SteerPid.Kd = SteerTurn.Kp*RateKd;
    ScaleCoe = ParamCoeBalance;
    
    if(ForkFlag == 1 || ForkFlag == 3)
    {
        LeftMotorSpeed.Goal = SlowSpeed.TurnSpeed - 150;
        RightMotorSpeed.Goal = SlowSpeed.TurnSpeed - 150;
        ScaleCoe = ParamCoeBalance*1.2;
    }
    else 
    {
        LeftMotorSpeed.Goal = SlowSpeed.TurnSpeed;
        RightMotorSpeed.Goal = SlowSpeed.TurnSpeed;
    }

    /***舵机输出***/
    Error = error_culculate(); 
    SteerOutTemp = steer_pd(&SteerPid,Error,0,85);
    pwm_duty(STEER,Middle - SteerOutTemp);

    

    /***差速计算***/
    /**左转舵机值变大*/
    if(SteerOutTemp > 0)
    {
        RightMotorSpeed.Goal = RightMotorSpeed.Goal - SteerOutTemp* ScaleCoe;
    }
    else
    {
        LeftMotorSpeed.Goal = LeftMotorSpeed.Goal + SteerOutTemp* ScaleCoe;
    }


    if(NumControlTime == 0)//5Ms控制一次舵机 20Ms控制一次电机
    {
        LeftPwm  = motor_pi(&SpeedParam,LeftMotorSpeed.Current,LeftMotorSpeed.Goal,10000);
        RightPwm = motor_pi(&SpeedParam,RightMotorSpeed.Current,RightMotorSpeed.Goal,10000);
        
        pwm_duty(RIGHT_FRONT, RightPwm >0?RightPwm:0);
        pwm_duty(RIGHT_BACK,RightPwm < 0 ? -1*RightPwm:0);//右后
        pwm_duty(LEFT_FRONT,LeftPwm > 0 ? LeftPwm:0);//左前
        pwm_duty(LEFT_BACK,LeftPwm < 0 ? -1*LeftPwm:0); //左后

    }

    NumControlTime++;
    if(NumControlTime == 4) NumControlTime = 0;
    
    /***保护***/
    if(AD1 < 30 && AD2 < 30 && AD3 < 30 && AD4 < 30 )
    {
        pwm_duty(RIGHT_FRONT,0);
        pwm_duty(RIGHT_BACK,0);//右后
        pwm_duty(LEFT_FRONT,0);//左前
        pwm_duty(LEFT_BACK,0); //左后
    }

    

}

