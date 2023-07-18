#include "st7789_hal_spi.h"
#include "sys.h"

#include "stm32f4xx_spi.h"
#include "delay.h"

#if USE_DMA_SPI
#include "hal_spi_dma.h"
#endif // USE_DMA_SPI

SPI_InitTypeDef  SPI_InitStructure;

void hal_spi2_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //SPI2��ʱ����Դ��APB1 42MHz
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//ʹ��GPIOB��SPI2ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);	//ʹ��GPIOB��SPI2ʱ��

    //PB13--SCL ,PB15--SDI��ʼ������,
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//PB13,PB15���ù������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2); //PB13����Ϊ SPI2

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); //PB15����Ϊ SPI2

    //����ֻ���SPI�ڳ�ʼ��
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,ENABLE);//��λSPI2
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2,DISABLE);//ֹͣ��λSPI2

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
    hal_spi2_rw_data(0xff);//��������
}

//Һ��IO��ʼ������
void hal_spi_gpio_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10  | GPIO_Pin_11 \
		| GPIO_Pin_12 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_SetBits(GPIOB,GPIO_Pin_14);//MISO�������Ϊ��

    hal_spi2_init();//��ʼ��Ӳ��SPI2
	
#if USE_DMA_SPI
	DMA_Config((uint32_t)Spi2DmaBuffer, DMA_SPI_BUF_SIZE);
#endif // USE_DMA_SPI
}

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t hal_spi2_rw_data(uint16_t TxData)
{
    uint8_t retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }
    SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
    {
        retry++;
        if(retry>200)return 0;
    }
    return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����
}
/******************************************************************************************/
//����Ļдһ��8λָ��
void hal_spi_write_cmd(uint8_t cmd)
{
	//SPI д����ʱ��ʼ
    HAL_SPI_CS_CLR;
    HAL_SPI_DC_CLR;
	
	hal_spi2_rw_data(cmd);

    HAL_SPI_CS_SET;
}

//��SPI���ߴ���һ��8λ����
void hal_spi_write_8bit_data(uint8_t data)
{
	//SPI д����ʱ��ʼ
    HAL_SPI_CS_CLR;
    HAL_SPI_DC_SET;
	
	hal_spi2_rw_data(data);
	
	HAL_SPI_CS_SET;
}

//����Ļдһ��16λ����
void hal_spi_write_16bit_data(uint16_t data)
{	
    HAL_SPI_CS_CLR;
    HAL_SPI_DC_SET;

	//д���8λ����
	hal_spi2_rw_data(data>>8);
	//д���8λ����
	hal_spi2_rw_data(data);
	
    HAL_SPI_CS_SET;
}

//��Ļ��λ
void hal_spi_hard_reset(void)
{
    HAL_SPI_RST_CLR;//RST�������Ϊ��
    delay_ms(1000);
    HAL_SPI_RST_SET;//RST�������Ϊ��
    delay_ms(100);
}

/**********************************************************************************/
/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�,Y_IncMode��ʾ������y������x
����ֵ����
*************************************************/
void hal_spi_setregion(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
{
    hal_spi_write_cmd(0x2a);
    hal_spi_write_8bit_data(x_start>>8);
    hal_spi_write_8bit_data(x_start);
    hal_spi_write_8bit_data(x_end>>8);
    hal_spi_write_8bit_data(x_end);

    hal_spi_write_cmd(0x2b);
    hal_spi_write_8bit_data(y_start>>8);
    hal_spi_write_8bit_data(y_start);
    hal_spi_write_8bit_data(y_end>>8);
    hal_spi_write_8bit_data(y_end);

    hal_spi_write_cmd(0x2c); //��ʼд��GRAM
}

// ��Ļ����
void hal_spi_clear(uint16_t color)
{
	uint16_t row,column;

	hal_spi_setregion(0, 0, LCD_WIDTH, LCD_HEIGHT);
	
    HAL_SPI_CS_CLR;
    HAL_SPI_DC_SET;
	for(row = 0;row < LCD_WIDTH; row++)             //ROW loop
	{
		for(column = 0;column < LCD_HEIGHT; column++) //column loop
		{
			hal_spi_write_16bit_data(color); 
		}
	}
	HAL_SPI_CS_SET;
}

void hal_spi_lcd_init(void)
{
	/********************************************************/
    delay_ms(1000);
    hal_spi_write_cmd(ST7789_CMD_SLPOUT); 	//Sleep Out
    delay_ms(120);
    //-----------------------ST7789V Frame rate setting-----------------//
    //************************************************
    hal_spi_write_cmd(ST7789_CMD_COLMOD);       //65k mode
    hal_spi_write_8bit_data(0x05);

    hal_spi_write_cmd(ST7789_CMD_VCMOFSET); 	//VCOM
    hal_spi_write_8bit_data(0x1A);

    hal_spi_write_cmd(ST7789_CMD_MADCTL);       // ��Ļ��ʾ��������
    hal_spi_write_8bit_data(0x00);
    //-------------ST7789V Frame rate setting-----------//
    hal_spi_write_cmd(ST7789_CMD_PORCTRL);		//Porch Setting
    hal_spi_write_8bit_data(0x05);
    hal_spi_write_8bit_data(0x05);
    hal_spi_write_8bit_data(0x00);
    hal_spi_write_8bit_data(0x33);
    hal_spi_write_8bit_data(0x33);

    hal_spi_write_cmd(ST7789_CMD_GCTRL);		//Gate Control
    hal_spi_write_8bit_data(0x05);			//12.2v   -10.43v
    //--------------ST7789V Power setting---------------//
    hal_spi_write_cmd(ST7789_CMD_VCOMS);//VCOM
    hal_spi_write_8bit_data(0x3F);

    hal_spi_write_cmd(ST7789_CMD_LCMCTRL); //Power control
    hal_spi_write_8bit_data(0x2c);

    hal_spi_write_cmd(ST7789_CMD_VDVVRHEN);	//VDV and VRH Command Enable
    hal_spi_write_8bit_data(0x01);

    hal_spi_write_cmd(ST7789_CMD_VRHS);		//VRH Set
    hal_spi_write_8bit_data(0x0F);		//4.3+( vcom+vcom offset+vdv)

    hal_spi_write_cmd(ST7789_CMD_VDVSET);		//VDV Set
    hal_spi_write_8bit_data(0x20);				//0v

    hal_spi_write_cmd(ST7789_CMD_FRCTR2);		//Frame Rate Control in Normal Mode
    hal_spi_write_8bit_data(0X01);			     //111Hz

    hal_spi_write_cmd(ST7789_CMD_PWCTRL1);		//Power Control 1
    hal_spi_write_8bit_data(0xa4);
    hal_spi_write_8bit_data(0xa1);

    hal_spi_write_cmd(ST7789_CMD_PWCTRL2);		//Power Control 1
    hal_spi_write_8bit_data(0x03);

    hal_spi_write_cmd(ST7789_CMD_EQCTRL);		//Equalize time control
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x08);
    //---------------ST7789V gamma setting-------------//
    hal_spi_write_cmd(ST7789_CMD_PVGAMCTRL); //Set Gamma
    hal_spi_write_8bit_data(0xD0);
    hal_spi_write_8bit_data(0x05);
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x08);
    hal_spi_write_8bit_data(0x14);
    hal_spi_write_8bit_data(0x28);
    hal_spi_write_8bit_data(0x33);
    hal_spi_write_8bit_data(0x3F);
    hal_spi_write_8bit_data(0x07);
    hal_spi_write_8bit_data(0x13);
    hal_spi_write_8bit_data(0x14);
    hal_spi_write_8bit_data(0x28);
    hal_spi_write_8bit_data(0x30);

    hal_spi_write_cmd(ST7789_CMD_NVGAMCTRL); //Set Gamma
    hal_spi_write_8bit_data(0xD0);
    hal_spi_write_8bit_data(0x05);
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x09);
    hal_spi_write_8bit_data(0x08);
    hal_spi_write_8bit_data(0x03);
    hal_spi_write_8bit_data(0x24);
    hal_spi_write_8bit_data(0x32);
    hal_spi_write_8bit_data(0x32);
    hal_spi_write_8bit_data(0x3B);
    hal_spi_write_8bit_data(0x14);
    hal_spi_write_8bit_data(0x13);
    hal_spi_write_8bit_data(0x28);
    hal_spi_write_8bit_data(0x2F);

    hal_spi_write_cmd(ST7789_CMD_INVON);		//����

    hal_spi_write_cmd(ST7789_CMD_DISPON);		//������ʾ
}
/*********************************************END OF FILE********************************************/

