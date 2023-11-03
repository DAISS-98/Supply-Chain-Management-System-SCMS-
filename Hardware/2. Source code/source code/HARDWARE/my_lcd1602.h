/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.1
//�޸�����:2020/11/18
//�����ܣ���װ�ͼ�GPIO�Ķ�д����
//V1.0  2019/10/22  1����ɻ�������
//V1.1  2020/11/18  1�����ӹ�꿪���͹رպ���
************************************************/
#ifndef __MY_LCD1602_H
#define __MY_LCD1602_H

#include "my_include.h"

/********************���˿ڶ���*********************************/
#define PIN_RS          PB5//����/����˿ں�
#define PIN_RW          PB6//��/дѡ��˿ں�
#define PIN_EN          PB7//

const static MyPinDef Pins_Data_1602[] = {PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15};

/***********************����ָ��***********************************/
#define SET_EN          PinSet(PIN_EN)//EN ʹ��  ���1
#define CLE_EN          PinReset(PIN_EN)//      ���0 
#define SET_RW          PinSet(PIN_RW)      //RW ��д  ���1
#define CLE_RW          PinReset(PIN_RW)//      ���0
#define SET_RS          PinSet(PIN_RS)//RS ָ��  ���1
#define CLR_RS          PinReset(PIN_RS)//      ���0
/*********************��������Ժ궨��*****************************/
#define LCD_Puts        LCD_PutStr
#define LCD_1Put        LCD_PutChar
/******************************************************************/

void LCD1602_Init(void);
void LCD1602_SetCursor(u8 x,u8 y);
void LCD1602_CloseCursor(void);
void LCD1602_Clear(void);
void LCD1602_Write_String(u8 x,u8 y,const  char *string);
void LCD1602_Write_Char(u8 x,u8 y,const char disChar);

#endif
