## 1. Hardware
Adapted to  GDC-COG-T200MEFS and QCG22QVGA916
### 1.1 Difference
(1) For GDC-COG-T200MEFS:
stm32f103c6
PB0-7  <-> DB0-7
Configure in ili9225.h: 
#define TRANSPORT_DATA_8BIT 1
#define TRANSPORT_DATA_16BIT 0

(2) For QCG22QVGA916
stm32f103rc
PB0-15  <-> DB0-15

Change IC from stm32f103c6 to stm32f103rc in project (not required).
Configure in ili9225.h: 
#define TRANSPORT_DATA_8BIT 0
#define TRANSPORT_DATA_16BIT 1
### 1.2 Common:
PA0  CS
PA1  DC/RS
PA2  RD
PA3  WR
PA4  RST
LED：2.8V-3.0V (recommend)

## 2. Software
Features:
1. Direction: PORTRAIT、LANDSCAPE、PORTRAIT_REVERSE、LANDSCAPE_REVERSE
2. Animation of fractal


## 3.References: 
 -  [Driver of ILI9225 1](https://github.com/ouening/STM32-HAL-examples/blob/master/f767zi-tftlcd/Src/tftlcd.c)
- [Driver of ILI9225 2](https://github.com/BasicCode/ILI9225/blob/master/STM32/ILI9225.c)
- [Driver of ILI9225: 2.0inch Arduino SPI Module ILI9225 SKU: MAR2001](http://www.lcdwiki.com/zh/2.0inch_Arduino_SPI_Module_ILI9225_SKU:MAR2001)
- [Julia Set](https://blog.csdn.net/qq_43606914/article/details/106077326)
