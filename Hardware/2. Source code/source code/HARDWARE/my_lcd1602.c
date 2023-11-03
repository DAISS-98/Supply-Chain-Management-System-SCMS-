/************************************************
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//TWKJ STM32������
//ͨ���Ƽ�@TWKJ
//���ߣ�tianqingyong
//�汾��V1.0
//�޸�����:2019/10/22
//�����ܣ���װ�ͼ�GPIO�Ķ�д����
//V1.0 ��ɻ�������
************************************************/
#include "my_lcd1602.h"

/************************�˿ڳ�ʼ��*******************************/
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
void GPIO_data(u8 x)  //�˿���������
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
void LCD_En_Toggle(void) //��ʹ������
{
    SET_EN;//ʹ��1
    delay_us(5);//��ʱ160us
    CLE_EN;
}

void LCD_Busy(void)//�ж�æ
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
     //delay_us(1);//��ʱ10us
     SET_RW;//RW = 1
     //delay_us(1);//��ʱ10us
     SET_EN;//EN = 1
     //delay_us(2);//��ʱ20us
     while ((PinRead(Pins_Data_1602[7]))&&(later0<20000)) //ѭ���ȴ�æ���˿� = 0
     {
        delay_us(5);
        later0++;
     }
     CLE_EN;//EN = 0

     //�ָ��˿�Ϊ���״̬ 
    for(i=0;i<8;i++)
    {
#if !defined (USE_HAL_DRIVER)
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_Mode_Out_PP);
#else
        GPIO_Pin_Init(Pins_Data_1602[i],GPIO_MODE_OUTPUT_PP,GPIO_PULLUP);
#endif
    }
}

//��Һ������д��ָ��  ʱ��RS=L,RW=L,Data0-Data7=ָ���룬E=������
void LCD1602_WriteCmd(u8 x,char cmd) 
{
    if(cmd) LCD_Busy();
    //delay_us(1);//��ʱ10us
    CLR_RS;//RS = 0 
    //delay_us(1);//��ʱ10us
    CLE_RW;//RW = 0 
    //delay_us(4);//��ʱ40us
    GPIO_data(x);//�˿���������
    //delay_us(4);//��ʱ40us
    LCD_En_Toggle();//��ʹ������
    //delay_us(1);//��ʱ100us
    LCD_Busy();//��æ

}
//��Һ������д������  ʱ��RS=H,RW=L,Data0-Data7=ָ���룬E=������
void LCD1602_WriteData(u8 x) //��Һ������д������ 
{
    LCD_Busy();//��æ
    //delay_us(1);//��ʱ10us
    SET_RS;//RS = 1 
    //delay_us(1);//��ʱ10us
    CLE_RW;//RW = 0
    //delay_us(4);//��ʱ40us
    GPIO_data(x);
    //delay_us(4);//��ʱ40us
    LCD_En_Toggle();//��ʹ������
    //delay_us(1);//��ʱ100us
    LCD_Busy();//��æ
}

void LCD_SetXY(u8 x,u8 y)   //�ַ���ʼλ���趨��x��ʾ�У�y��ʾ�� 
{ 
     u8 addr;
     if(y==0) 
          addr=0x80+x;
     else if(y==1)
          addr=0xC0+x;
     LCD1602_WriteCmd(addr,1) ;
}
/******************************************************************/

void LCD1602_Init( void )//��ʼ��
{
    LCD_GPIO_init();
    delay_us(1500);//��ʱ15ms
    LCD1602_WriteCmd( 0x38,0);//дָ��38H �����æ�ź�
    delay_us(500);//��ʱ5ms
    LCD1602_WriteCmd( 0x38,0);//дָ��38H �����æ�ź�
    delay_us(500);//��ʱ5ms
    LCD1602_WriteCmd( 0x38,0);// дָ��38H �����æ�ź�
//�Ժ�ÿ��дָ���/д���ݲ���֮ǰ����æ�ź�
    LCD1602_WriteCmd( 0x38,1);//��ʾģʽ����
    LCD1602_WriteCmd( 0x08,1);//��ʾ�ر�
    LCD1602_WriteCmd( 0x01,1);//��ʾ����
    LCD1602_WriteCmd( 0x06,1);//��ʾ����ƶ�����
    LCD1602_WriteCmd( 0x0C,1);//��ʾ������겻��ʾ
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
                ��������
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
void LCD1602_Write_String(u8 x,u8 y,const char *string) //��1602дһ���ַ��� 
{
    //unsigned char i=0;
    LCD_SetXY(x,y);
    while(*string) 
    {
        LCD1602_WriteData(*string);
        string++;
    }
}

