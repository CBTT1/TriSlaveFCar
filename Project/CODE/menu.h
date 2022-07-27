/*
 * menu.h
 *
 *  Created on: 2021��2��21��
 *      Author: ����
 */

#ifndef __MENU_H_
#define __MENU_H_

#include "headfile.h"

//�ṹ�嶨��
struct item                                     //�˵�ѡ��
{
    enum
    {
        subpage=1,                      //ҳ������ ����ҳ
        value,
        switc,
        func,
        value_float,
        value_int32,
        title
    }type;

    int32 dlt;                          //ÿ�θĶ��Ĳ�ֵ ������value
    float dlt_float;
    void *addr;                             //�����oled�ϵ�ֵ ��Ϊ��ҳ���� ��Ϊ��ַ
    void (*chf);                              //������һ����ַ

    char name[20];
};

struct page
{
    uint8 rpos;                          //��һ��λ��
    uint8 pos;                           //���λ��
    uint8 count;
    uint8 dymantic_page;                 //��̬ҳ

    struct item itemlist[35];
};


//����
//zyx1

#define KEY_1       P72                     //up
#define KEY_2       P65                       //down
#define KEY_3       P64                      //add
#define KEY_4       P67                      //sub
#define KEY_5       P40                      //enter
#define KEY_6       P66                      //leave
#define KEY_7       P3_6
#define KEY_DelayTime   100                     //����ɨ����ʱʱ��


//��������������
//��������
void add_subpage(struct page * tg, char * name, struct page * v);
void add_value(struct page * tg, char * name, int16 * v, int16 dt, void (*changedCallBack)());
void add_value_float(struct page * tg, char * name, float * v, float dt, void (*changedCallBack)());
void add_switc(struct page * tg, char * name, int16 * v, void (*operate)());
void add_func(struct page * tg, char * name, void (*v)());
void add_value_int32(struct page * tg, char * name, int32 * v, int32 dt, void (*changedCallBack)());
void add_title(struct page * tg, char * name, char length);
void MenuRender(char full_update);
void OLED_BeginUpdate(void);
void MenuCmd(char cmd);
void key_init(void);//������ʼ��
char key_scan(void);
void MenuInit(struct page *mainpage);
void OLED_EndUpdate(void);
#endif /* LIBRARIES_HDU_LIBRARIESNEW_MENU_H_ */
