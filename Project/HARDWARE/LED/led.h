#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 

//STM32F407������
//LED��������	   
						  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED2 PAout(1)	// D2

#define LED_ON     GPIO_SetBits(GPIOA, GPIO_Pin_1)
#define LED_OFF    GPIO_ResetBits(GPIOA, GPIO_Pin_1)


void LED_Init(void);//��ʼ��		 				    
#endif
