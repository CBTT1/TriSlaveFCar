#include "headfile.h"
#include "control.h"
#include "device.h"
#include "menu.h"

void inductance_test()
{
    oled_fill(0x00);
    pit_timer_ms(TIM_4,5);
    oled_p6x8char(0,0,'1');
    oled_p6x8char(0,2,'2');
    oled_p6x8char(0,4,'3');
    oled_p6x8char(0,6,'4');
    while(1)
    {
        oled_int16(16,0,AD1);
        oled_int16(16,2,AD2);
        oled_int16(16,4,AD3);
        oled_int16(16,6,AD4);
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