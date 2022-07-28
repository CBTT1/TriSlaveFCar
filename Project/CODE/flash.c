#include "headfile.h"
#include "control.h"
#include "device.h"
#include "element_process.h"

sfr IAP_DATA_A = 0xC2; 
sfr IAP_ADDRH_A = 0xC3; 
sfr IAP_ADDRL_A = 0xC4; 
sfr IAP_CMD_A = 0xC5; 
sfr IAP_TRIG_A = 0xC6; 
sfr IAP_CONTR_A = 0xC7; 
sfr IAP_TPS_A = 0xF5;



void IapIdle() 
{ 
 IAP_CONTR_A = 0; //�ر� IAP ����
 IAP_CMD_A = 0; //�������Ĵ���
 IAP_TRIG_A = 0; //��������Ĵ���
 IAP_ADDRH_A = 0x80; //����ַ���õ��� IAP ����
 IAP_ADDRL_A = 0; 
} 

char IapRead(int addr) 
{ 
 char dat; 
 
 IAP_CONTR_A = 0x80; //ʹ�� IAP
 IAP_TPS_A = 24; //���ò����ȴ����� 12MHz
 IAP_CMD_A = 1; //���� IAP ������
 IAP_ADDRL_A = addr; //���� IAP �͵�ַ
 IAP_ADDRH_A = addr >> 8; //���� IAP �ߵ�ַ
 IAP_TRIG_A = 0x5a; //д��������(0x5a)
 IAP_TRIG_A = 0xa5; //д��������(0xa5)
 _nop_(); 
 dat = IAP_DATA_A; //�� IAP ����
 IapIdle();
	return dat; 
} 

void IapProgram(int addr, char dat) 
{ 
 IAP_CONTR_A = 0x80; //ʹ�� IAP
 IAP_TPS_A = 24; //���ò����ȴ����� 12MHz
 IAP_CMD_A = 2; //���� IAP д����
 IAP_ADDRL_A = addr; //���� IAP �͵�ַ
 IAP_ADDRH_A = addr >> 8; //���� IAP �ߵ�ַ
 IAP_DATA_A = dat; //д IAP ����
 IAP_TRIG_A = 0x5a; //д��������(0x5a)
 IAP_TRIG_A = 0xa5; //д��������(0xa5)
 _nop_(); 
 IapIdle(); //�ر� IAP ����
} 
void IapErase(int addr) 
{ 
 IAP_CONTR_A = 0x80; //ʹ�� IAP
 IAP_TPS_A = 24; //���ò����ȴ����� 12MHz
 IAP_CMD_A = 3; //���� IAP ��������
IAP_ADDRL_A = addr; //���� IAP �͵�ַ
 IAP_ADDRH_A = addr >> 8; //���� IAP �ߵ�ַ
 IAP_TRIG_A = 0x5a; //д��������(0x5a)
 IAP_TRIG_A = 0xa5; //д��������(0xa5)
 _nop_(); //
 IapIdle(); //�ر� IAP ����
} 




int16 write_po=0;
int16 read_po=0;
void write_float(float x)
{
	char *p;
	p = &x;
	IapProgram(write_po++,*p);
	p++;
	IapProgram(write_po++,*p);
	p++;
	IapProgram(write_po++,*p);
	p++;
	IapProgram(write_po++,*p);
}

void write_int16(int16 x)
{
	char *p;
	p = &x;
	IapProgram(write_po++,*p);
	p++;
	IapProgram(write_po++,(x<<8)>>8);
}

void read_float(float *x)
{
	char *p;
	p = x;
	*p = IapRead(read_po++);
	p++;
	*p = IapRead(read_po++);
	p++;
	*p = IapRead(read_po++);
	p++;
	*p = IapRead(read_po++);
}

void read_int16(int16* x)
{
	char *p;
	p = x;
	*x = 0;
	*p = IapRead(read_po++);
	p++;
	*p = IapRead(read_po++);
}


void write_flash()
{
	write_po = 0;
	oled_fill(0x00);
	IapErase(0x0000);
	IapErase(0x0202);
	
	write_float(SteerPid.Kp);
	write_float(SteerPid.Kd);
	write_float(ParamCoeBalance);
	write_float(SpeedParam.Kp);
	write_float(SpeedParam.Ki);
	write_int16(SlowSpeed.TurnSpeed);
	write_int16(RateKd);
	write_int16(RdSetIntegral[0].In);
	write_int16(RdSetIntegral[0].Out1);
	write_int16(RdSetIntegral[0].Out2);

	oled_p8x16str(32,3,"write ok");
}

void read_flash()
{
	read_po = 0;
	oled_fill(0x00);

	read_float(&SteerPid.Kp);
	read_float(&SteerPid.Kd);
	read_float(&ParamCoeBalance);
	read_float(&SpeedParam.Kp);
	read_float(&SpeedParam.Ki);
	read_int16(&SlowSpeed.TurnSpeed);
	read_int16(&RateKd);
	read_int16(&RdSetIntegral[0].In);
	read_int16(&RdSetIntegral[0].Out1);
	read_int16(&RdSetIntegral[0].Out2);

	oled_p8x16str(32,3,"read ok");
}