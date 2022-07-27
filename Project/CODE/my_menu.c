#include "headfile.h"
#include "menu.h"
#include "control.h"
#include "device.h"
#include "testmode.h"

struct page Page[3]={0};


void run(void);

void menu_init(void)
{
    uint8 i = 0;
    
    // for(i = 0;i<3;i++)
    // {
    //     Page[i].pos = 0;
    //     Page[i].rpos = 0;
    //     Page[i].count = 0;
    //     Page[i].dymantic_page = 0;
    // }

    add_title(&Page[0],"Test",4);
    add_func(&Page[0],"Run",run);
    add_func(&Page[0],"Inductance",inductance_test);
    add_func(&Page[0],"DriveTest",drive_test);
    add_value_float(&Page[0],"SpeedKp",&SpeedParam.Kp,0.5,NULL);
    add_value_float(&Page[0],"SpeedKi",&SpeedParam.Ki,0.5,NULL);
    add_value_float(&Page[0],"CoeKp",&CoeParam.Kp,0.2,NULL);
    add_value_float(&Page[0],"CoeKd",&CoeParam.Kd,0.2,NULL);
    add_value(&Page[0],"SlowTurn",&SlowSpeed.TurnSpeed,20,NULL);
    MenuInit(Page);
}

void run(void)
{
    oled_fill(0x00);
    inductance_init();
    motor_init();
    encoder_init();
    BEEP_IO = 1;
    pit_timer_ms(TIM_4,5);
    while(1)
    {
        if(ControlFlag == 1)
        {
            main_control();
            
            ControlFlag = 0;
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