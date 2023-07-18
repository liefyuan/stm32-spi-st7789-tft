#ifndef __HAL_SPI_DMA_H__
#define __HAL_SPI_DMA_H__

#include "stdint.h"
#include "sys.h"
#include "st7789_hal_spi.h"

#define SCREEN_SIZE      ST7789_LCD_WIDTH*ST7789_LCD_HEIGHT
#define DMA_SPI_BUF_SIZE (480*20)

extern uint16_t Spi2DmaBuffer[DMA_SPI_BUF_SIZE];	//�������ݻ�����

void DMA_Config(uint32_t TX_Buff,uint32_t SENDBUFF_SIZE);
void DMA_Write_buf(uint32_t SizeLen);
void dma_hal_spi_clear(uint16_t color);

#endif // __HAL_SPI_DMA_H__



