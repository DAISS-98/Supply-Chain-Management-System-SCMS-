
 #ifndef __MY_TIMER_H
#define __MY_TIMER_H
#include "my_include.h"

/***********************************************
�������ܣ���ʼ����ʱ��
TIMx����ʱ��ѡ��TIM1��TIM2��TIM3��TIM4
arr���Զ�����ֵ
psc��ʱ�ӷ�Ƶϵ��
itEnable���Ƿ�ʹ���жϣ�trueΪʹ�ܣ�falseΪ��ʹ��
************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc,bool itEnable); 
 
#endif
