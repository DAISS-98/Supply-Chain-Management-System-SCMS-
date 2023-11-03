#include "my_include.h"

void scanKeyAnddealKey(void);
void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str);
void displayOfCollectedData(void);//显示采集到的数据

unsigned int i; //中间变量 i
char dis0[200];//液晶显示暂存数组
char dis1[23];//液晶显示暂存数组
char dis2[23];//液晶显示暂存数组


unsigned char setMode  = 0;//设置标志


unsigned int sensorVal = 0; //当前采集值
unsigned int adcx;	 	

u8 startCheckWireLess = 0;//启动检测无线种类
u8 wirelessFlag = 0;//无线种类

int main(void)
{
    USARTx_Init(USART1,9600);//串口初始化为9600
//    USARTx_Init(USART2,9600);//串口初始化为9600
    
    My_KEY_Init();//初始化输入
//    My_LED_Init(); //输出初始化  
    
    LCD1602_Init(); //液晶初始化    
	LCD1602_Write_String(0,0,"init .. ");//显示字符串

    buzzer=1;relay=1;//上电动作下
    delay_ms(200);
    buzzer=0;relay=0;//关闭动作
    
    TCS34725_Init();//初始化传感器
    
    i = 5;startCheckWireLess=1;//启动检测 一段时间取消
    while(i-- && wirelessFlag == 0) //此时判断是无线类型
    {USARTSendString(USART1,"ATE0\r\n");  delay_ms(100);} 
    startCheckWireLess = 0;//取消检测
    if(wirelessFlag == 1)
    {    
        i = 50;while(i--){delay_ms(100);}      //wifi启动 必须延时5s或以上   
		USARTSendString(USART1,"AT+CWMODE_CUR=3\r\n");// 设置模式
		
//    USARTSendString(USART1,"AT+CIPMODE=0\r\n");//设置ip模式
//    delay_ms(50);
        USARTSendString(USART1,"AT+CIPMUX=1\r\n");//打开多连接
        delay_ms(50);
        USARTSendString(USART1,"AT+CIPSERVER=1,8080\r\n");//启动端口号
        delay_ms(50);   //以上作为服务端 局域网
    }   
   
    while(1)
    {
	    scanKeyAnddealKey();//按键扫描及处理
			
        if(myReadFlag_tick == true ) //定时读取时间到
        {
            myReadFlag_tick = false; //清除标志
            
            if(setMode == 0) //采集模式
            {
                if(tcs_rgb.r > 255 || tcs_rgb.g > 255 ||  tcs_rgb.b > 255 )//数据异常
                {
                    sprintf(dis1,"Rxxx Gxxx Bxxx ");		//打印					
                    LCD1602_Write_String(0,0,dis1);//显示			
                }
      
            
            }             
        }
                
        if(mySendFlag_tick == true )//定时发送时间到
        {
            mySendFlag_tick = false;//清除标志
            
            TCS34725_GetRawData(&tcs_rgb);  //读两次，实际测试时发现读到的颜色总是上一次的颜色
            
            if(setMode == 1)//上报数据
            {
                setMode = 2;//上报结束
                sprintf(dis0,"%s\r\n\r\n",dis1);	  //打印			
                if(wirelessFlag == 0)USARTSendString(USART1 ,dis0);	 //发送	 对应的 数据
         
            }                 
        }      
        My_UartMessage_Process();//处理串口数据        
    }
}


void scanKeyAnddealKey(void)
{
    My_KeyScan();//按键扫描
    
    if(KeyIsPress(KEY_1))
    {
        setMode++;//设置标志
        if(setMode >= 2) setMode = 0;        
    }
}

void OnGetUartMessage(const _uart_msg_obj *uartMsgRec)
{
    char *strPtr;    

    if((strPtr=strstr(uartMsgRec->payload,"SL"))!=NULL)//接收到字符串 *MD....#
    {
//        setL=ParseInteger(strPtr+2,3);//提取设置参数        
    }
}


void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str)
{
//    u8 i;
//    for(i=0;i<2;i++)
    {
        My_USART_printf(USARTx,"AT+CIPSEND=%d,%d\r\n",0,strlen(str));  
        delay_ms(10);
        USARTSendBytes(USARTx,str,strlen(str));//发送数据
   
    }
}


void checkWireLessMode(u8 recBuf)//检查无线类型
{
    static u8 checkIn = 0;//查看第一次标志是否接受
    if(startCheckWireLess == 1 && wirelessFlag == 0)//启动检测无线
    {
        if(recBuf == 'O')checkIn = 1;
          
    }
}


