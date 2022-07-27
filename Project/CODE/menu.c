/*
 * menu.c
 *
 *  Created on: 2021��2��21��
 *      Author: ����
 */

#include "headfile.h"
#include "menu.h"
#include "SEEKFREE_OLED.h"
#include"zf_gpio.h"
#include "zf_delay.h"

uint32 cntpage=0;//ҳ��
struct page *navigate[32];//ָ��

enum
{
    KEY_UP = 1,
    KEY_DOWN,
    KEY_ADD,
    KEY_SUB,
    KEY_ENTER,
    KEY_LEAVE
}KEY_TYPE;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg������Ӳ˵�����
//  @param      tg              ��Ӽ��ĵ�ַ
//  @param      name            ���� �13λ
//  @param          v                           ��ǰ�����һ���˵���ַ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_subpage(struct page * tg, char * name, struct page * v)
{
    struct item it;
    char i;
    it.type = subpage;
    for(i=0;i<13 && name[i];i++)
        it.name[i] = name[i];
    it.name[i] = 0;
    it.chf = 0;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg�����ֵ����
//  @param      tg                                  ��һ���˵�
//  @param      name                            ����
//  @param          v                                           ��ֵ�Ĵ�С
//  @param          changedCallBack             ����ָ�� ִ���޸ĸ�ֵ�ĺ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_value(struct page * tg, char * name, int16 * v, int16 dt, void (*changedCallBack)())
{
    struct item it;
    char i;

    it.type = value;
    for(i=0;i<13 && name[i];i++)
            it.name[i] = name[i];
    it.name[i] = 0;
    it.dlt = dt;
    it.chf = changedCallBack;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg�����ֵ����
//  @param      tg                                  ��һ���˵�
//  @param      name                            ����
//  @param          v                                           ��ֵ�Ĵ�С
//  @param          changedCallBack             ����ָ�� ִ���޸ĸ�ֵ�ĺ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_title(struct page * tg, char * name, char length)
{
    struct item it;
    char i;

    it.type = title;
    it.name[0] = ' ';
    for(i=1;i<6;i++)
            it.name[i] = '-';
    for(;i<6+length;i++)
            it.name[i] = name[i - 6];
    for(;i<19;i++)
            it.name[i] = '-';
    it.name[i] = 0;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg����Ӹ���������
//  @param      tg                                  ��һ���˵�
//  @param      name                            ����
//  @param          v                                           ��ֵ�Ĵ�С
//  @param          changedCallBack             ����ָ�� ִ���޸ĸ�ֵ�ĺ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_value_float(struct page * tg, char * name, float * v, float dt, void (*changedCallBack)())
{
    struct item it;
    char i;

    it.type = value_float;
    for(i=0;i<13 && name[i];i++)
            it.name[i] = name[i];
    it.name[i] = 0;
    it.dlt_float = dt;
    it.chf = changedCallBack;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg�����int32������
//  @param      tg                                  ��һ���˵�
//  @param      name                            ����
//  @param          v                                           ��ֵ�Ĵ�С
//  @param          changedCallBack             ����ָ�� ִ���޸ĸ�ֵ�ĺ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_value_int32(struct page * tg, char * name, int32 * v, int32 dt, void (*changedCallBack)())
{
    struct item it;
    char i;

    it.type = value_int32;
    for(i=0;i<13 && name[i];i++)
            it.name[i] = name[i];
    it.name[i] = 0;
    it.dlt = dt;
    it.chf = changedCallBack;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg����ӿ�������
//  @param      tg                  ��һ���˵�
//  @param      name            ����
//  @param          v                           Ϊ1 0��ֵ 1Ϊon 0Ϊoff
//  @param          operate             ִ�п��ز����ĺ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_switc(struct page * tg, char * name, int16 * v, void (*operate)())
{
    struct item it;
    char i;

    it.type = switc;

    for(i=0;i<18 && name[i];i++)
        it.name[i] = name[i];
    it.name[i] = 0;
    it.dlt = 0;
    it.chf = operate;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��tg����Ӻ�������
//  @param      tg                  ��һ���˵�
//  @param      name            ����
//  @param          v                           ִ�иò�������������
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void add_func(struct page * tg, char * name, void (*v)())
{
    struct item it;
    char i;

    it.type = func;
    for(i=0;i<20 && name[i];i++)
        it.name[i] = name[i];
    it.name[i] = 0;
    it.chf = 0;
    it.addr = v;
    tg->itemlist[tg->count++] = it;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ʾ���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void OLED_BeginUpdate(void)
{
    oled_fill(0x00);
    oled_p6x8char(0,navigate[cntpage]->pos - navigate[cntpage]->rpos, '-');
}

void OLED_EndUpdate(void)
{

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ӡ��� �˵�ɨ��
//  @param      full_update                 0 �� 1 ѡ��ȫ����ӡ����ֻˢ�±仯����
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void MenuRender(char full_update)
{
		int tmp = 0;
		int i;
    if(full_update)
    {
        OLED_BeginUpdate();
        for(i = navigate[cntpage]->rpos; i < navigate[cntpage]->rpos + 8 && i < navigate[cntpage]->count; i++)
        {

            oled_p6x8str(6,i - navigate[cntpage]->rpos,navigate[cntpage]->itemlist[i].name);

            switch(navigate[cntpage]->itemlist[i].type)
            {
                case title:
                    break;
                case subpage:
                    break;
                case switc:
                    tmp = *((int16*)navigate[cntpage]->itemlist[i].addr);
                    if(tmp)
                        oled_p6x8str(18*6,i - navigate[cntpage]->rpos,"On");
                    else
                        oled_p6x8str(18*6,i - navigate[cntpage]->rpos,"Off");
                    break;
                case func:
                    break;
                case value:
                {
                    if(!navigate[cntpage]->dymantic_page)
                    {
                        tmp = *((int16*)navigate[cntpage]->itemlist[i].addr);
                        oled_int16((uint8)14*6,(uint8)(i - navigate[cntpage]->rpos),( int16) tmp);
                    }
                    break;
                }
                case value_float:
                {
                    if(!navigate[cntpage]->dymantic_page)
                    {
                        float temp_float = (*((float*)navigate[cntpage]->itemlist[i].addr));
                        oled_printf_float(14*6,i - navigate[cntpage]->rpos,temp_float,5, 4);
                    }
                    break;
                }
                default: break;
            }
        }

        if(!navigate[cntpage]->dymantic_page) oled_p6x8char(0,navigate[cntpage]->pos - navigate[cntpage]->rpos,'-');

        OLED_EndUpdate();
    }
    else
    {

        if(navigate[cntpage]->dymantic_page)//ֻ���¶�̬ҳ��ֻ����value���͵������ʾ���
        {
            for(i = navigate[cntpage]->rpos; i < navigate[cntpage]->rpos + 8 && i < navigate[cntpage]->count; i++)
            {
                switch(navigate[cntpage]->itemlist[i].type)
                {
                    case value:
                    {
                        int16 tmpv = *((int16*)navigate[cntpage]->itemlist[i].addr);
                        oled_int16(15*6,i - navigate[cntpage]->rpos,tmpv);
                        break;
                    }
                    case value_int32:
                    {
                        int32 tmpv = *((int32*)navigate[cntpage]->itemlist[i].addr);
                        oled_printf_int32(15*6,i - navigate[cntpage]->rpos,tmpv, 5);
                        break;
                    }
                    case value_float:
                    {
                        float tmpv = *((float*)navigate[cntpage]->itemlist[i].addr);
                        oled_printf_float(14*6,i - navigate[cntpage]->rpos,tmpv,5, 4);
                        break;
                    }
                    default: break;
                }
            }
            delay_ms(10);
        }
    }
}

void MenuInit(struct page *mainpage)
{
    navigate[0] = mainpage;

    MenuRender(1);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ִ�а�������
//  @param      cmd                                     �������� 0 Ϊ�޲���
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void MenuCmd(char cmd)
{
		void (*funcs)() = NULL;
    if(cmd)
    {
        switch(cmd)
        {
            case KEY_UP:
            {
                if(navigate[cntpage]->dymantic_page)
                {
                    if(navigate[cntpage]->rpos > 0) navigate[cntpage]->rpos--;
                }
                else
                {
                    if(navigate[cntpage]->pos > 0)
                    {
                        navigate[cntpage]->pos--;
                        if(navigate[cntpage]->rpos > navigate[cntpage]->pos) navigate[cntpage]->rpos--;
                    }
                    else
                    {
                        navigate[cntpage]->pos = navigate[cntpage]->count - 1;
                        if(navigate[cntpage]->count > 8) navigate[cntpage]->rpos = navigate[cntpage]->count - 8;
                    }
                }

                break;
            }
            case KEY_DOWN:
            {
                    if(navigate[cntpage]->dymantic_page)
                    {
                        if(navigate[cntpage]->rpos + 8 < navigate[cntpage]->count) navigate[cntpage]->rpos++;
                    }
                    else
                    {
                        if(navigate[cntpage]->pos < navigate[cntpage]->count - 1)
                        {
                            navigate[cntpage]->pos++;
                            if(navigate[cntpage]->rpos < navigate[cntpage]->pos - 7) navigate[cntpage]->rpos++;
                        }
                        else
                        {
                            navigate[cntpage]->pos = 0;
                            navigate[cntpage]->rpos = 0;
                        }
                    }

                    break;
            }
            case KEY_ADD:
            {
                    if(navigate[cntpage]->dymantic_page) return;

                    if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == value)
                    {
                        *((int16*)navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr) += navigate[cntpage]->itemlist[navigate[cntpage]->pos].dlt;

                         funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].chf;
                        if(funcs) funcs();
                    }
                    if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == value_float)
                    {
                        *((float*)navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr) += navigate[cntpage]->itemlist[navigate[cntpage]->pos].dlt_float;

                         funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].chf;
                        if(funcs) funcs();
                    }
                    break;
            }
            case KEY_SUB:
            {
                    if(navigate[cntpage]->dymantic_page) return;

                    if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == value)
                    {
                        *((int16*)navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr) -= navigate[cntpage]->itemlist[navigate[cntpage]->pos].dlt;

                        funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].chf;
                        if(funcs) funcs();
                    }
                    if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == value_float)
                    {
                        *((float*)navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr) -= navigate[cntpage]->itemlist[navigate[cntpage]->pos].dlt_float;

                        funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].chf;
                        if(funcs) funcs();
                    }
                    break;
            }
            case KEY_ENTER:
            {
										
                    if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == subpage)
                    {
                        if(cntpage < 32)
                        {
												
                                navigate[cntpage + 1] = navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr;
																
                                cntpage++;
                        }
												
                    }
                    else if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == func)
                    {
											
                        funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr;
											
                        if(funcs) funcs();
                        MenuRender(0);
                        return;
                    }
                    else if(navigate[cntpage]->itemlist[navigate[cntpage]->pos].type == switc)
                    {
                        int16 * value = (int16*)navigate[cntpage]->itemlist[navigate[cntpage]->pos].addr;
                        funcs = navigate[cntpage]->itemlist[navigate[cntpage]->pos].chf;
                        if(value)
                        {
                                if(*value) *value = 0;
                                else *value = 1;
                        }
                        if(funcs) funcs();
                    }
                break;
            }
            case KEY_LEAVE:
            {
                if(cntpage != 0)
                {
                        cntpage--;
                }
                break;
            }
            default: break;
        }
        MenuRender(1);
    }
    else MenuRender(0);
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      ����ɨ��
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
char key_scan (void)
{
    if (KEY_1 == 0)
    {
        delay_ms((uint16)KEY_DelayTime);
        if ((KEY_1 == 0))
        {
            return 1;
        }
    }

    if (KEY_2 == 0)
    {
        delay_ms( (uint16)KEY_DelayTime);
        if ((KEY_2 == 0))
        {
            return 2;
        }
    }

    if (KEY_3 == 0)
    {
        delay_ms((uint16)KEY_DelayTime);
        if ((KEY_3 == 0))
        {
            return 3;
        }
    }

    if (KEY_4 == 0)
    {
        delay_ms( (uint16)KEY_DelayTime);
        if ((KEY_4 == 0))
        {
            return 4;
        }
    }

    if (KEY_5 == 0)
    {
        delay_ms((uint16)KEY_DelayTime);
        if ((KEY_5 == 0))
        {
            return 5;
        }
    }

    if (KEY_6 == 0)
    {
        delay_ms( (uint16)KEY_DelayTime);
        if ((KEY_6 == 0))
        {
            return 6;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ������ʼ��
//  @param      void
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void key_init(void)
{
    gpio_pull_set(KEY_1, PULLUP);
    gpio_pull_set(KEY_2 , PULLUP);
    gpio_pull_set(KEY_3 , PULLUP);
    gpio_pull_set(KEY_4 , PULLUP);
    gpio_pull_set(KEY_5 , PULLUP);
    gpio_pull_set(KEY_6 , PULLUP);
    gpio_pull_set(KEY_7 , PULLUP);
}


