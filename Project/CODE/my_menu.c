#include "headfile.h"
#include "menu.h"
#include "control.h"
#include "device.h"
#include "testmode.h"
#include "flash.h"
#include "element_process.h"

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

    // add_title(&Page[0],"Test",4);
    add_func(&Page[0],"Run",run);
    add_subpage(&Page[0],"Param",&Page[1]);
    add_subpage(&Page[0],"TestMode",&Page[2]);

    {
        add_func(&Page[1],"ReadFlash",read_flash);
        add_func(&Page[1],"WriteFlash",write_flash);
        add_switc(&Page[1],"SwitchRD",&RoundaboutSwitch,NULL);
        add_switc(&Page[1],"SwitchFORK",&ForkSwitch,NULL);
        add_value_float(&Page[1],"SteerKp",&SteerPid.Kp,0.2,NULL);
        add_value(&Page[1],"RateKd",&RateKd,1,NULL);
        add_value_float(&Page[1],"Coe",&ParamCoeBalance,0.1,NULL);
        // add_value_float(&Page[1],"SteerKd",&SteerPid.Kd,0.2,NULL);
        add_value_float(&Page[1],"SpeedKp",&SpeedParam.Kp,0.5,NULL);
        add_value_float(&Page[1],"SpeedKi",&SpeedParam.Ki,0.5,NULL);
        add_value(&Page[1],"SpeedTurn",&SlowSpeed.TurnSpeed,20,NULL);
        add_value(&Page[1],"RdIn",&RdSetIntegral[0].In,5,NULL);
        add_value(&Page[1],"RdOut1",&RdSetIntegral[0].Out1,5,NULL);
        add_value(&Page[1],"RdOut2",&RdSetIntegral[0].Out2,5,NULL);
    }

    {       
        add_func(&Page[2],"Inductance",inductance_test);
        add_func(&Page[2],"Drive",drive_test);
        add_func(&Page[2],"Integral",integral_test);
        add_func(&Page[2],"Wireless",wireless_test);
    }
    MenuInit(Page);
}

void run(void)
{
    oled_fill(0x00);
    inductance_init();
    steer_init();
    motor_init();
    encoder_init();
    BEEP_IO = 1;
    pit_timer_ms(TIM_4,5);
    while(1)
    {
        if(ControlFlag == 1)
        {
            element_process();
            main_control();
            if(RoundaboutFlag > 0)
            {
                wireless_ch573_send_buff(&RoundaboutFlag,1);
                wireless_ch573_send_buff(&PathIntegral,4);
                wireless_ch573_send_buff(&RdIntegral.Error,4);
            }
            if(ForkFlag > 0)
            {
                wireless_ch573_send_buff(&ForkFlag,1);
            }

            
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