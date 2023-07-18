#include "stm32f4xx_dma.h"
#include "hal_spi_dma.h"
#include "delay.h"

#if USE_DMA_SPI

volatile uint8_t sendFlg = 0;

uint16_t Spi2DmaBuffer[DMA_SPI_BUF_SIZE];	//�������ݻ�����
	
void DMA_Config(uint32_t TX_Buff,uint32_t SENDBUFF_SIZE)
{
    // �жϽṹ��
    NVIC_InitTypeDef NVIC_InitStructure;		
    // DMA�ṹ��
    DMA_InitTypeDef DMA_InitStructure;  		
    /* ʹ��DMAʱ�� */  		
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
    /* ��λ��ʼ��DMA������ */  
    DMA_DeInit(DMA1_Stream4);								
    /* ȷ��DMA��������λ��� */  
    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE);	

    /* ���� DMA Stream */
    /* ͨ��0��������4 */	  
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;
    /* �����ַ */  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;	
    /* �ڴ��ַ(Ҫ����ı�����ָ��) ,DMA�洢��0��ַ*/  	
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)TX_Buff;	
    /* ���򣺴洢�������� */			
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    /* ���ݴ����� ,������Ϊ0�� ʵ�ʷ���ʱ����������*/	    
    DMA_InitStructure.DMA_BufferSize = (uint32_t)SENDBUFF_SIZE;		
    /* ���������ģʽ */		
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    /* �洢������ģʽ */  	
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    /* �������ݳ���:16λ */	 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    /* �ڴ����ݳ���:16λ */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    /* DMAģʽ������ģʽ */  		
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    /* ���ȼ����� */	 		
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    /* ����FIFO */
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        		
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;   
    /* ����ͻ�����δ��� */  
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;    		
    /* �洢��ͻ�����δ��� */  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 

    /* ��ʼ��DMA Stream */		
    DMA_Init(DMA1_Stream4, &DMA_InitStructure);
    /* ������������ж�  */		
    DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);

    // �жϳ�ʼ�� 
    /* DMA�����ж�Դ */  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;	
    /* �������ȼ� */  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    /* ��Ӧ���ȼ� */  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				
    /* ʹ���ⲿ�ж�ͨ�� */ 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						 
    /* ����NVIC */		
    NVIC_Init(&NVIC_InitStructure);
}

void DMA1_Stream4_IRQHandler(void)
{
    // DMA �������
    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))	
    {
        // ���DMA������ɱ�־
        DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);	
        // Ƭѡ���ߣ����ݷ������	
        HAL_SPI_CS_SET;	
		sendFlg = 1;
    }
}

void DMA_Write_buf(uint32_t SizeLen)
{   
	// �رշ��� DMA		
	DMA_Cmd(DMA1_Stream4, DISABLE);	
	// ���÷��͵�������    
	DMA_SetCurrDataCounter(DMA1_Stream4, SizeLen);
	// �������
	SPI2->DR;		
	// ����DMA��־λ	
	DMA_ClearFlag(DMA1_Stream4, DMA_IT_TCIF4);
	// Ƭѡ����,��������
	HAL_SPI_CS_CLR;
	HAL_SPI_DC_SET;
	// �������� DMA
	DMA_Cmd(DMA1_Stream4, ENABLE);
	
	while(!sendFlg); 
	sendFlg = 0;
}

void dma_hal_spi_clear(uint16_t color)
{
    int i = 0;
    for(i = 0 ; i < DMA_SPI_BUF_SIZE; i++)
    {
        Spi2DmaBuffer[i] = color; // �������������ֶ�����
    }
	
	hal_spi_setregion(0, 0, LCD_WIDTH, LCD_HEIGHT);
	for(i = 0; i < 16/*(SCREEN_SIZE/DMA_SPI_BUF_SIZE)*/; i++)
	{
		SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE); 
		
		DMA_Write_buf(DMA_SPI_BUF_SIZE);
	}
}
#endif // USE_DMA_SPI
/*********************************************END OF FILE********************************************/
