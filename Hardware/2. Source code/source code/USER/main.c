#include "my_include.h"

void scanKeyAnddealKey(void);
void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str);
void displayOfCollectedData(void);//��ʾ�ɼ���������

unsigned int i; //�м���� i
char dis0[200];//Һ����ʾ�ݴ�����
char dis1[23];//Һ����ʾ�ݴ�����
char dis2[23];//Һ����ʾ�ݴ�����


unsigned char setMode  = 0;//���ñ�־


unsigned int sensorVal = 0; //��ǰ�ɼ�ֵ
unsigned int adcx;	 	

u8 startCheckWireLess = 0;//���������������
u8 wirelessFlag = 0;//��������

int main(void)
{
    USARTx_Init(USART1,9600);//���ڳ�ʼ��Ϊ9600
//    USARTx_Init(USART2,9600);//���ڳ�ʼ��Ϊ9600
    
    My_KEY_Init();//��ʼ������
//    My_LED_Init(); //�����ʼ��  
    
    LCD1602_Init(); //Һ����ʼ��    
	LCD1602_Write_String(0,0,"init .. ");//��ʾ�ַ���

    buzzer=1;relay=1;//�ϵ綯����
    delay_ms(200);
    buzzer=0;relay=0;//�رն���
    
    TCS34725_Init();//��ʼ��������
    
    i = 5;startCheckWireLess=1;//������� һ��ʱ��ȡ��
    while(i-- && wirelessFlag == 0) //��ʱ�ж�����������
    {USARTSendString(USART1,"ATE0\r\n");  delay_ms(100);} 
    startCheckWireLess = 0;//ȡ�����
    if(wirelessFlag == 1)
    {    
        i = 50;while(i--){delay_ms(100);}      //wifi���� ������ʱ5s������   
		USARTSendString(USART1,"AT+CWMODE_CUR=3\r\n");// ����ģʽ
		
//    USARTSendString(USART1,"AT+CIPMODE=0\r\n");//����ipģʽ
//    delay_ms(50);
        USARTSendString(USART1,"AT+CIPMUX=1\r\n");//�򿪶�����
        delay_ms(50);
        USARTSendString(USART1,"AT+CIPSERVER=1,8080\r\n");//�����˿ں�
        delay_ms(50);   //������Ϊ����� ������
    }   
   
    while(1)
    {
	    scanKeyAnddealKey();//����ɨ�輰����
			
        if(myReadFlag_tick == true ) //��ʱ��ȡʱ�䵽
        {
            myReadFlag_tick = false; //�����־
            
            if(setMode == 0) //�ɼ�ģʽ
            {
                if(tcs_rgb.r > 255 || tcs_rgb.g > 255 ||  tcs_rgb.b > 255 )//�����쳣
                {
                    sprintf(dis1,"Rxxx Gxxx Bxxx ");		//��ӡ					
                    LCD1602_Write_String(0,0,dis1);//��ʾ			
                }
      
            
            }             
        }
                
        if(mySendFlag_tick == true )//��ʱ����ʱ�䵽
        {
            mySendFlag_tick = false;//�����־
            
            TCS34725_GetRawData(&tcs_rgb);  //�����Σ�ʵ�ʲ���ʱ���ֶ�������ɫ������һ�ε���ɫ
            
            if(setMode == 1)//�ϱ�����
            {
                setMode = 2;//�ϱ�����
                sprintf(dis0,"%s\r\n\r\n",dis1);	  //��ӡ			
                if(wirelessFlag == 0)USARTSendString(USART1 ,dis0);	 //����	 ��Ӧ�� ����
         
            }                 
        }      
        My_UartMessage_Process();//����������        
    }
}


void scanKeyAnddealKey(void)
{
    My_KeyScan();//����ɨ��
    
    if(KeyIsPress(KEY_1))
    {
        setMode++;//���ñ�־
        if(setMode >= 2) setMode = 0;        
    }
}

void OnGetUartMessage(const _uart_msg_obj *uartMsgRec)
{
    char *strPtr;    

    if((strPtr=strstr(uartMsgRec->payload,"SL"))!=NULL)//���յ��ַ��� *MD....#
    {
//        setL=ParseInteger(strPtr+2,3);//��ȡ���ò���        
    }
}


void My_ESP8266_SendStrStr(USART_TypeDef* USARTx, const char *str)
{
//    u8 i;
//    for(i=0;i<2;i++)
    {
        My_USART_printf(USARTx,"AT+CIPSEND=%d,%d\r\n",0,strlen(str));  
        delay_ms(10);
        USARTSendBytes(USARTx,str,strlen(str));//��������
   
    }
}


void checkWireLessMode(u8 recBuf)//�����������
{
    static u8 checkIn = 0;//�鿴��һ�α�־�Ƿ����
    if(startCheckWireLess == 1 && wirelessFlag == 0)//�����������
    {
        if(recBuf == 'O')checkIn = 1;
          
    }
}


