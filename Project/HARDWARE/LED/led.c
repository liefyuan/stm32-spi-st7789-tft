#include "led.h"
//////////////////////////////////////////////////////////////////////////////////

//STM32F407������
//LED��������

//////////////////////////////////////////////////////////////////////////////////

//��ʼ��PA1   ��ʹ������ڵ�ʱ��
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��

    //GPIOF9,F10��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//LED2��ӦIO��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

    GPIO_SetBits(GPIOA,GPIO_Pin_1);//PA1  ���øߣ�����

}






