#ifndef __DEVICE_H
#define __DEVICE_H

#define LEFT_FRONT  PWM6_P75
#define LEFT_BACK   PWM5_P74
#define RIGHT_FRONT PWM8_P77
#define RIGHT_BACK PWM7_P76

#define ENCODER_LEFT_DIR CTIM0_P34
#define ENCODER_LEFT_CNT P60
#define ENCODER_RIGHT_DIR CTIM3_P04 
#define ENCODER_RIGHT_CNT P61

#define ADC_1 ADC_P15 //ADC_P00
#define ADC_2 ADC_P13
#define ADC_3 ADC_P10
#define ADC_4 ADC_P11
#define ADC_5 ADC_P05
#define ADC_6 ADC_P06
#define ADC_7 ADC_P00
#define ADC_8 ADC_P01

#define BEEP_IO P71

extern int16 AD1,AD2,AD3,AD4,AD5,AD6,AD7,AD8;
extern int16 SpeedLeft,SpeedRight;

void inductance_read();
void encoder_read();
void encoder_init();
void motor_init();
void inductance_init();

#endif