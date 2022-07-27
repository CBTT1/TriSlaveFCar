#ifndef __CONTROL_H
#define __CONTROL_H


// #define LEFT_FRONT          PWM6_P75
// #define LEFT_BACK           PWM5_P74
// #define RIGHT_FRONT         PWM8_P77
// #define RIGHT_BACK          PWM7_P76

struct PidParam
{
    float Kp;
    float Ki;
    float Kd;
    int16 Error;
    int16 LastError;
    float Output;
};

struct CarParam{
    int16 Current;
    int16 Goal;
};

struct Variance
{
    int16 Output;
    int16 LastOutput;
    float ParamCoe;

};

struct SpeedGoal
{
    uint16 StraightSpeed;
    uint16 MaxSpeed; //长直道速度
    uint16 TurnSpeed;
    uint16 RampUpSpeed;
    uint16 RampDownSpeed;
};

extern uint8 ControlFlag;
extern struct PidParam SpeedParam;
extern struct PidParam CoeParam;
extern struct CarParam LeftMotorSpeed,RightMotorSpeed,Steer;
extern struct SpeedGoal SlowSpeed,BalanceSpeed,FastSpeed;

extern void main_control();

#endif