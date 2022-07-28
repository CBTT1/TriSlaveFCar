#include "headfile.h"
#include "control.h"
#include "device.h"
#include "menu.h"
#include "math.h"


void inductance_test()
{
    int16 Error ; 
    oled_fill(0x00);
    pit_timer_ms(TIM_4,5);
    oled_p6x8char(0 ,0,'1');
    oled_p6x8char(64,0,'4');

    oled_p6x8char(0 ,1,'2');
    oled_p6x8char(64,1,'3');

    oled_p6x8char(0 ,2,'5');
    oled_p6x8char(64,2,'6');

    oled_p6x8char(0 ,3,'7');
    oled_p6x8char(64,3,'8');
    oled_p6x8str(16,6,"Error");
    while(1)
    {
        oled_int16(16,0,AD1);
        oled_int16(80,0,AD4);


        oled_int16(16,1,AD2);
        oled_int16(80,1,AD3);
        
        oled_int16(16,2,AD5);
        oled_int16(80,2,AD6);

        oled_int16(16,3,AD7);
        oled_int16(80,3,AD8);

        Error = ((float)(AD1-AD4))/((AD2+AD3)*sqrt((AD2+AD3)))*1000;
        oled_int16(60,6,Error);
        delay_ms(100);
        if (KEY_5 == 0)
        {
            delay_ms((uint16)KEY_DelayTime);
            if ((KEY_5 == 0))
            {
                MenuRender(1);
                return;
            }
        }
    }
}

void drive_test()
{
    oled_fill(0x00);
    motor_init();
    pit_timer_ms(TIM_4,5);
    oled_p6x8str(16,1,"Left");
    oled_p6x8str(16,3,"Right");
    while(1)
    {
        oled_printf_int32(50,1,LeftMotorSpeed.Current,4);
        oled_printf_int32(50,3,RightMotorSpeed.Current,4);
        pwm_duty(RIGHT_FRONT,2000);
        pwm_duty(RIGHT_BACK,0);//右后
        pwm_duty(LEFT_FRONT,2000);//左前
        pwm_duty(LEFT_BACK,0); //左后
        if (KEY_5 == 0)
        {
            delay_ms((uint16)KEY_DelayTime);
            if ((KEY_5 == 0))
            {
                pwm_duty(RIGHT_FRONT,0);
                pwm_duty(RIGHT_BACK,0);//右后
                pwm_duty(LEFT_FRONT,0);//左前
                pwm_duty(LEFT_BACK,0); //左后
                MenuRender(1);
                return;
            }
        }
    }
}

void integral_test()
{
    oled_fill(0x00);
    while (1)
    {
        oled_int16(32,3,PathIntegral);
        delay_ms(200);
        if (KEY_5 == 0)
        {
            delay_ms((uint16)KEY_DelayTime);
            if ((KEY_5 == 0))
            {
                pwm_duty(RIGHT_FRONT,0);
                pwm_duty(RIGHT_BACK,0);//右后
                pwm_duty(LEFT_FRONT,0);//左前
                pwm_duty(LEFT_BACK,0); //左后
                MenuRender(1);
                return;
            }
        }
    }
    
}

void wireless_test()
{
    oled_fill(0x00);
    while(1)
    {
        if(KEY_4 == 0)
        {
            delay_ms((uint16)KEY_DelayTime);
            if(KEY_4 == 0)
            {
                wireless_ch573_send_buff("Hi\n",3);
            }
        }
        if (KEY_5 == 0)
        {
            delay_ms((uint16)KEY_DelayTime);
            if ((KEY_5 == 0))
            {
                MenuRender(1);
                return;
            }
        }
    }
}