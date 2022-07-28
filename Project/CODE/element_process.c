#include "headfile.h"
#include "control.h"
#include "device.h"
#include "element_process.h"

int16 RoundaboutSwitch = 0,ForkSwitch = 1;
uint8 RoundaboutNum = 0,ForkNum = 0;
uint8 RoundaboutSize[4] = {0};
uint8 RoundaboutFlag = 0,ForkFlag = 0;
uint8 RoundaboutDir = 0,ForkDir = LEFT;
struct ElementIntegral RdIntegral ={0,0,0};
struct RoundaboutSetIntegral RdSetIntegral[3] ={{45,45,45},{50,50,50},{50,50,50}};

float abs_float(float x);
int16 abs_int16(int16 x);
void element_process()
{
    if(RoundaboutSwitch == 1)
    {
        if(RoundaboutFlag == 0 && ForkFlag == 0)
        {
            if((AD2>700&&AD3>700||AD1>1000||AD4>1000)
                &&AD5+AD6>500
                &&AD1>300
                &&AD2>300
                &&AD3>300
                &&AD4>300)
                {
                    RoundaboutFlag = 1;
                    RdIntegral.Start = PathIntegral;
                    if(AD5 > AD6)
                    {
                        RoundaboutDir = 1;
                    }
                    else 
                    {
                        RoundaboutDir = 2;
                    }
                }
        }
        else if(RoundaboutDir == LEFT )
        {
            if(RoundaboutFlag == 1)//向前走积分一段
            {
                RdIntegral.Now = PathIntegral;
                RdIntegral.Error = RdIntegral.Now - RdIntegral.Start;
                if(RdIntegral.Error > RdSetIntegral[RoundaboutNum].In)
                {
                    RoundaboutFlag = 2;
                    RdIntegral.Now = RdIntegral.Error = RdIntegral.Start = 0;
                }
            }
            else if(RoundaboutFlag == 2)//正在转弯，舵机打死
            {
                if(AD2+AD3<1300)
                {
                    RoundaboutFlag = 3;
                }
            }
            else if(RoundaboutFlag == 3)//转圈圈
            {
                if(AD7>1500&&AD8>1500)
                {
                    RoundaboutFlag = 4;
                    RdIntegral.Start = PathIntegral;  
                }

            }
            else if(RoundaboutFlag == 4)
            {
                RdIntegral.Now = PathIntegral;
                RdIntegral.Error = RdIntegral.Now  - RdIntegral.Start;
                if(AD5>250 && AD6>250 && RdIntegral.Error > RdSetIntegral[RoundaboutNum].Out2)
                {
                    RoundaboutFlag = 5;
                    RdIntegral.Now = RdIntegral.Error = RdIntegral.Start = 0;
                }
                    
            }
            else if(RoundaboutFlag == 5)
            {
                if(AD2+AD3<1500)
                {
                    RoundaboutFlag = 0;
                    RoundaboutDir = 0;
                    RoundaboutNum++;
                }
            }
        }
        else if(RoundaboutDir == RIGHT)
        {
            if(RoundaboutFlag == 1)//向前走积分一段
            {
                RdIntegral.Now = PathIntegral;
                RdIntegral.Error = RdIntegral.Now - RdIntegral.Start;
                if(RdIntegral.Error > RdSetIntegral[RoundaboutNum].In)
                {
                    RoundaboutFlag = 2;
                    RdIntegral.Now = RdIntegral.Error = RdIntegral.Start = 0;
                }
            }
            else if(RoundaboutFlag == 2)//正在转弯，舵机打死
            {
                if(AD2+AD3<1300)
                {
                    RoundaboutFlag = 3;
                }
            }
            else if(RoundaboutFlag == 3)//转圈圈
            {
                if(AD7>1500&&AD8>1500)
                {
                    RoundaboutFlag = 4;
                    RdIntegral.Start = PathIntegral;  
                }

            }
            else if(RoundaboutFlag == 4)
            {
                RdIntegral.Now = PathIntegral;
                RdIntegral.Error = RdIntegral.Now  - RdIntegral.Start;
                if(AD5>250 && AD6>250 && RdIntegral.Error > RdSetIntegral[RoundaboutNum].Out2)
                    RoundaboutFlag = 5;
            }
            else if(RoundaboutFlag == 5)
            {
                if(AD5+AD6<600&&AD5+AD6>300&&AD2+AD3>400&&AD1+AD4<1200)
                {
                    RoundaboutFlag = 0;
                    RoundaboutDir = 0;
                    RoundaboutNum++;
                }
            }
        }

    }

    if(ForkSwitch == 1)
    {
        if(ForkFlag == 0 && RoundaboutFlag == 0  )
        {
            if(error_culculate() < 10
            && AD7+AD8 > 1000
            && AD1>200 && AD1<600
            && AD4>200 && AD4<600 
            && AD2<500 && AD3<600
            && abs_int16(AD1 - AD2)< 100
            && abs_int16(AD3 - AD4)< 100
            && abs_int16(AD2 - AD3)< 150 
            && (AD5>200||AD6>200)) //              
            {
                ForkFlag = 1;
            }
        }
        else if(ForkFlag == 1)
        {
            if(ForkDir == LEFT && abs_int16(AD1 - AD4) > 150 && AD1 + AD4 < 400 && error_culculate() > 15)
            {
                ForkFlag = 2;
            }
            else if(ForkDir == RIGHT && abs_int16(AD4 - AD1) > 150 && error_culculate() < -15)
            {
                ForkFlag = 2;
            }
        }
        else if(ForkFlag == 2)
        {
            //正常巡线
            if(AD7+AD8 > 1200 
                && AD1>200 && AD1<400
                && AD4>200 && AD4<400 
                && AD2<500 && AD3<500 
                && abs_float(AD2 - AD3)< 150 
                && (AD5>200||AD6>200))
                {
                ForkFlag = 3;
                }
        }
        else if(ForkFlag == 3)
        {
            if(ForkDir == LEFT && abs_int16(AD1 - AD4) > 150)
            {
                ForkFlag = 4;
            }
            else if(ForkDir == RIGHT && abs_int16(AD4 - AD1) > 150)
            {
                ForkFlag = 4;
            }
        }
        else if(ForkFlag == 4)
        {
            if(ForkDir == LEFT) ForkDir = RIGHT;
            else if(ForkDir == RIGHT) ForkDir = LEFT;
            ForkNum++;
            ForkFlag = 0;
        }

    }





}

float abs_float(float x)
{
	return x>0?x:-x;
}
int16 abs_int16(int16 x)
{
    return x>0?x:-x;
}