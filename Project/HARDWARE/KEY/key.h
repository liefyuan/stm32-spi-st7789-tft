#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

//STM32F407������
//������������
								  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0--��ӦK1����


/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define WK_UP 	PAin(0)		//PA0
*/


#define WKUP_PRES   1

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
