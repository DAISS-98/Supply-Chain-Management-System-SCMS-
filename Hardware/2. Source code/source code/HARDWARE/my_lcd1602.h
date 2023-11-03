/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.1
//修改日期:2020/11/18
//程序功能：封装和简化GPIO的读写操作
//V1.0  2019/10/22  1、完成基本功能
//V1.1  2020/11/18  1、增加光标开启和关闭函数
************************************************/
#ifndef __MY_LCD1602_H
#define __MY_LCD1602_H

#include "my_include.h"

/********************各端口定义*********************************/
#define PIN_RS          PB5//数据/命令端口号
#define PIN_RW          PB6//读/写选择端口号
#define PIN_EN          PB7//

const static MyPinDef Pins_Data_1602[] = {PB8,PB9,PB10,PB11,PB12,PB13,PB14,PB15};

/***********************基本指令***********************************/
#define SET_EN          PinSet(PIN_EN)//EN 使能  输出1
#define CLE_EN          PinReset(PIN_EN)//      输出0 
#define SET_RW          PinSet(PIN_RW)      //RW 读写  输出1
#define CLE_RW          PinReset(PIN_RW)//      输出0
#define SET_RS          PinSet(PIN_RS)//RS 指令  输出1
#define CLR_RS          PinReset(PIN_RS)//      输出0
/*********************程序兼容性宏定义*****************************/
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
