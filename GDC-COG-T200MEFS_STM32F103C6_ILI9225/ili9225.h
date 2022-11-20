/**********************************************************************
* Develop by cheng_dev
* date: 2022.11
* Driver for ILI9225
***********************************************************************/

#ifndef __ILI9225_H
#define __ILI9225_H

#include <stm32f10x.h>

#ifdef RGB16BIT
#define RGB(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3) ))
#else
#define RGB(R,G,B) ((unsigned short int)((((R)>>3)<<11) | (((G)>>2)<<5)	| ((B)>>3) ))
#endif

#define ILI9225_HORIZONTAL_WINDOW_ADDR_HE (0x36u)  // Horizontal Address Start Position
#define ILI9225_HORIZONTAL_WINDOW_ADDR_HS (0x37u)  // Horizontal Address End Position
#define ILI9225_VERTICAL_WINDOW_ADDR_VE   (0x38u)  // Vertical Address Start Position
#define ILI9225_VERTICAL_WINDOW_ADDR_VS   (0x39u)  // Vertical Address End Position
#define ILI9225_RAM_ADDR_SET_HS           (0x20u)  // Horizontal GRAM Address Set
#define ILI9225_RAM_ADDR_SET_VS           (0x21u)  // Vertical GRAM Address Set
#define ILI9225_GRAM_DATA_REG             (0x22u)  // GRAM Data Register

//direction
#define PORTRAIT            (0x01 << 0)
#define LANDSCAPE           (0x01 << 1)
#define PORTRAIT_REVERSE    (0x01 << 2)
#define LANDSCAPE_REVERSE   (0x01 << 3)

#define IC_ILI9225          (0x01 << 0)

#define TRANSPORT_DATA_8BIT 1
#define TRANSPORT_DATA_16BIT 0

#define LCD_CMD  0	//Ð´ÃüÁî
#define LCD_DATA 1	//Ð´Êý¾Ý

typedef struct
{
  uint8_t ic_type;
  uint16_t size_x;
  uint16_t size_y;
  uint8_t dir;
}LCD_TYPE;

void adjust_coordinate(void);
void lcd_set_dir(uint8_t dir_);
void lcd_set_dir_regs(void);
void lcd_set_origin(uint16_t x, uint16_t y);
void lcd_set_type(LCD_TYPE lcd_type);

void lcd_send_data(uint16_t data_cmd,uint8_t mode);
void lcd_send_data_8bit(uint8_t data_cmd,uint8_t mode);
void lcd_set_register_data(uint16_t regid, uint16_t dat);

void lcd_init(void);
void lcd_clear(uint16_t color);
void lcd_display_color_bar(void);
void lcd_display_gray_bar(void);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_fill_rect(uint16_t x, uint16_t y, uint16_t cx, uint16_t cy, uint16_t color);
void display_pics(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, const uint8_t *data);
#endif	   
