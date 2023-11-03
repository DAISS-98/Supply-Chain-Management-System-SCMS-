/************************************************
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//TWKJ STM32开发板
//通旺科技@TWKJ
//作者：tianqingyong
//版本：V1.0
//修改日期:2019/10/22
//程序功能：封装和简化GPIO的读写操作
//V1.0 完成基本功能
************************************************/
#include "my_lcd1602.h"

/************************端口初始化*******************************/
void LCD_GPIO_init(void)
{
    u8 i;
#if !defined (USE_HAL_DRIVER)
    for(i=0;i<8;i++)
    {
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_Mode_Out_PP);
    }
    GPIO_Pin_Init(PIN_EN,GPIO_Mode_Out_PP);
    GPIO_Pin_Init(PIN_RW,GPIO_Mode_Out_PP);
    GPIO_Pin_Init(PIN_RS,GPIO_Mode_Out_PP);
#else
    for(i=0;i<8;i++)
    {
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
    }
    GPIO_Pin_Init(PIN_EN,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
    GPIO_Pin_Init(PIN_RW,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
    GPIO_Pin_Init(PIN_RS,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
}
void GPIO_data(u8 x)  //端口置入数据
{
    u8 i;
    for(i=0;i<8;i++)
    {
        if(x&(0x01<<i))
        {
            PinSet(Pins_Data_1602[i]);//DB0
        }
        else{
            PinReset(Pins_Data_1602[i]);//DB0
        }
    }
}
 
/******************************************************/
void LCD_En_Toggle(void) //发使能脉冲
{
    SET_EN;//使能1
    delay_us(5);//延时160us
    CLE_EN;
}

void LCD_Busy(void)//判断忙
{
    u8 i;
    u16 later0=0;
    for(i=0;i<8;i++)
    {
#if !defined (USE_HAL_DRIVER)
    GPIO_Pin_Init(Pins_Data_1602[i],GPIO_Mode_IPU);
#else
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_MODE_INPUT,GPIO_PULLUP);
#endif
    }

     CLR_RS;//RS = 0
     //delay_us(1);//延时10us
     SET_RW;//RW = 1
     //delay_us(1);//延时10us
     SET_EN;//EN = 1
     //delay_us(2);//延时20us
     while ((PinRead(Pins_Data_1602[7]))&&(later0<20000)) //循环等待忙检测端口 = 0
     {
        delay_us(5);
        later0++;
     }
     CLE_EN;//EN = 0

     //恢复端口为输出状态 
    for(i=0;i<8;i++)
    {
#if !defined (USE_HAL_DRIVER)
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_Mode_Out_PP);
#else
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
    }
}

//向液晶里面写入指令  时序：RS=L,RW=L,Data0-Data7=指令码，E=高脉冲
void LCD1602_WriteCmd(u8 x,char cmd) 
{
    if(cmd) LCD_Busy();
    //delay_us(1);//延时10us
    CLR_RS;//RS = 0 
    //delay_us(1);//延时10us
    CLE_RW;//RW = 0 
    //delay_us(4);//延时40us
    GPIO_data(x);//端口置入数据
    //delay_us(4);//延时40us
    LCD_En_Toggle();//发使能脉冲
    //delay_us(1);//延时100us
    LCD_Busy();//测忙

}
//向液晶里面写入数据  时序：RS=H,RW=L,Data0-Data7=指令码，E=高脉冲
void LCD1602_WriteData(u8 x) //向液晶里面写入数据 
{
    LCD_Busy();//测忙
    //delay_us(1);//延时10us
    SET_RS;//RS = 1 
    //delay_us(1);//延时10us
    CLE_RW;//RW = 0
    //delay_us(4);//延时40us
    GPIO_data(x);
    //delay_us(4);//延时40us
    LCD_En_Toggle();//发使能脉冲
    //delay_us(1);//延时100us
    LCD_Busy();//测忙
}

void LCD_SetXY(u8 x,u8 y)   //字符初始位置设定，x表示列，y表示行 
{ 
     u8 addr;
     if(y==0) 
          addr=0x80+x;
     else if(y==1)
          addr=0xC0+x;
     LCD1602_WriteCmd(addr,1) ;
}
/******************************************************************/

void LCD1602_Init( void )//初始化
{
    LCD_GPIO_init();
    delay_us(1500);//延时15ms
    LCD1602_WriteCmd( 0x38,0);//写指令38H 不检测忙信号
    delay_us(500);//延时5ms
    LCD1602_WriteCmd( 0x38,0);//写指令38H 不检测忙信号
    delay_us(500);//延时5ms
    LCD1602_WriteCmd( 0x38,0);// 写指令38H 不检测忙信号
//以后每次写指令、读/写数据操作之前需检测忙信号
    LCD1602_WriteCmd( 0x38,1);//显示模式设置
    LCD1602_WriteCmd( 0x08,1);//显示关闭
    LCD1602_WriteCmd( 0x01,1);//显示清屏
    LCD1602_WriteCmd( 0x06,1);//显示光标移动设置
    LCD1602_WriteCmd( 0x0C,1);//显示开、光标不显示
}
 
void LCD1602_SetCursor(u8 x,u8 y)
{
    LCD_SetXY(x,y);
    LCD1602_WriteCmd(0x0f, 1);
}
void LCD1602_CloseCursor(void)
{
    LCD1602_WriteCmd(0x0c, 1);
}
/*------------------------------------------------
                清屏函数
------------------------------------------------*/
void LCD1602_Clear(void) 
{
    LCD1602_WriteCmd(0x01,1);
    delay_ms(5);
}
/******************************************************/

void LCD1602_Write_Char(u8 x,u8 y,const char Data0)
{
    LCD_SetXY(x,y);
    LCD1602_WriteData(Data0);
}
void LCD1602_Write_String(u8 x,u8 y,const char *string) //向1602写一个字符串 
{
    //unsigned char i=0;
    LCD_SetXY(x,y);
    while(*string) 
    {
        LCD1602_WriteData(*string);
        string++;
    }
}

