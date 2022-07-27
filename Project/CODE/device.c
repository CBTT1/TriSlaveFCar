#include "headfile.h"
#include "device.h"
#include "control.h"

// int16 SpeedLeft = 0,SpeedRight = 0;
int16 AD1 = 0,AD2 = 0,AD3 = 0,AD4 = 0,AD5 = 0,AD6 = 0,AD7 = 0,AD8 = 0;

/****电感读数******/
void inductance_read()
{
    AD1 = adc_once(ADC_1,ADC_11BIT);
	AD2 = adc_once(ADC_2,ADC_11BIT);
	AD3 = adc_once(ADC_3,ADC_11BIT);
	AD4 = adc_once(ADC_4,ADC_11BIT);
	AD5 = adc_once(ADC_5,ADC_11BIT);
	AD6 = adc_once(ADC_6,ADC_11BIT);
	AD7 = adc_once(ADC_7,ADC_11BIT);
	AD8 = adc_once(ADC_8,ADC_11BIT);
}

/****编码器读数****/
void encoder_read()
{
    static int16 Num=0;
	Num++;
	if(Num>=4)
	{
		Num = 0;

        LeftMotorSpeed.Current = (ENCODER_LEFT_CNT == 1?ctimer_count_read(ENCODER_LEFT_DIR):-ctimer_count_read(ENCODER_LEFT_DIR));
        RightMotorSpeed.Current = -(ENCODER_RIGHT_CNT == 1?ctimer_count_read(ENCODER_RIGHT_DIR):-ctimer_count_read(ENCODER_RIGHT_DIR));
        ctimer_count_clean(ENCODER_LEFT_DIR);
        ctimer_count_clean(ENCODER_RIGHT_DIR);
		// con_flag = 1;
    }
}

/***编码器初始化***/
void encoder_init()
{
    ctimer_count_init(ENCODER_LEFT_DIR);
	ctimer_count_init(ENCODER_RIGHT_DIR);
}

/***电机初始化***/
void motor_init()
{
	pwm_init(LEFT_FRONT,12000,0);
	pwm_init(LEFT_BACK,12000,0);
	pwm_init(RIGHT_FRONT,12000,0);
	pwm_init(RIGHT_BACK,12000,0);
}

/***电感初始化****/
void inductance_init()
{
	adc_init(ADC_1,ADC_SYSclk_DIV_2);
	adc_init(ADC_2,ADC_SYSclk_DIV_2);
	adc_init(ADC_3,ADC_SYSclk_DIV_2);
	adc_init(ADC_4,ADC_SYSclk_DIV_2);
	adc_init(ADC_5,ADC_SYSclk_DIV_2);
	adc_init(ADC_6,ADC_SYSclk_DIV_2);
	adc_init(ADC_7,ADC_SYSclk_DIV_2);
	adc_init(ADC_8,ADC_SYSclk_DIV_2);
}