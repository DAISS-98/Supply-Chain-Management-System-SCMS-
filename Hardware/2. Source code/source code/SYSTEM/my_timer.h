
 #ifndef __MY_TIMER_H
#define __MY_TIMER_H
#include "my_include.h"

/***********************************************
函数功能：初始化定时器
TIMx：定时器选择，TIM1、TIM2、TIM3、TIM4
arr：自动重载值
psc：时钟分频系数
itEnable：是否使能中断，true为使能，false为不使能
************************************************/
void TIMx_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc,bool itEnable); 
 
#endif
