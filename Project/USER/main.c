#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd_driver.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
    delay_init(168);      //��ʼ����ʱ����
    uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200

    LED_Init();					  //��ʼ��LED
	
	printf("system init!\r\n");
	
	LCD_Init();
	
	Lcd_Full(RED);
	delay_ms(1000);
	
    while(1)
    {
		Lcd_Full(GREEN);
//		LED_ON;
//		delay_ms(500);
		
		Lcd_Full(WHITE);
//		LED_OFF;
//		delay_ms(50);
		
		Lcd_Full(BLUE);
//		LED_ON;
//		delay_ms(50);
		
		Lcd_Full(RED);
//		LED_OFF;		
    }
}
