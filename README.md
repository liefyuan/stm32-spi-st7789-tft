# 刷图测试工程

# 基本介绍
主控芯片：STM32F407VGT6
屏幕驱动芯片：ST7789
屏幕分辨率：240x320
材质：ips
屏幕驱动方式：四线SPI方式（SDA，SCL，CS，DC）

屏幕引脚：
- VCC
- GND
- SDA
- SCL
- CS
- RES
- DC
- BLK

# 刷图测试
自测满屏刷一种颜色
| 方式 | 一帧耗时 | 帧率 |
|--|--|--|
| GPIO模拟SPI | 310ms | 3.2fps |
| 硬件SPI外设 | 188ms | 5.3fps |
| 硬件SPI外设+DMA发送 | 58.6ms | 17fps |


# 刷图方式选择
- 打开GPIO模拟SPI：`lcd_driver.c`文件里面宏`#define USE_HAL_SPI`置为 0
- 打开硬件SPI外设：`lcd_driver.c`文件里面宏`#define USE_HAL_SPI`置为 1

如何打开spi和DMA呢？

- 先打开硬件SPI外设：`lcd_driver.c`文件里面宏`#define USE_HAL_SPI`置为 1
- 再打开DMA：`st7789_hal_spi.h`文件里面宏`#define USE_DMA_SPI`置为 1

