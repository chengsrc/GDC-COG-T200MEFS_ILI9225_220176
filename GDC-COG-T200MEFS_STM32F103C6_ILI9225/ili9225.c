/**********************************************************************
* Develop by cheng_dev
* date: 2022.11
* Driver for ILI9225
***********************************************************************/

#include <stm32f10x.h>
#include "delay.h"
#include "ili9225.h"

#define CS_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_0)
#define CS_SET  GPIO_SetBits(GPIOA, GPIO_Pin_0)

#define DC_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define DC_SET  GPIO_SetBits(GPIOA, GPIO_Pin_1)

#define RD_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define RD_SET  GPIO_SetBits(GPIOA, GPIO_Pin_2)

#define WR_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define WR_SET  GPIO_SetBits(GPIOA, GPIO_Pin_3)

#define RST_CLR  GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define RST_SET  GPIO_SetBits(GPIOA, GPIO_Pin_4)


static LCD_TYPE lcd = {
  IC_ILI9225,
  176,
  220,
  PORTRAIT
};

void lcd_set_type(LCD_TYPE lcd_type)
{
  lcd = lcd_type;
  adjust_coordinate();
}

void lcd_set_dir(uint8_t dir_)
{
  lcd.dir = dir_;
  lcd_set_dir_regs();
  adjust_coordinate();
}

void lcd_set_dir_regs(void)
{
  switch(lcd.dir)
  {
    case PORTRAIT:
      lcd_set_register_data(0x03, 0x1030);
      break;
    case LANDSCAPE:
      lcd_set_register_data(0x03, 0x1018);       
      break;
    case PORTRAIT_REVERSE:
      lcd_set_register_data(0x03, 0x1000);       
      break;
    case LANDSCAPE_REVERSE:
      lcd_set_register_data(0x03, 0x1028);
    break;
  }
}

void lcd_set_origin(uint16_t x, uint16_t y) 
{
	if (lcd.dir & (PORTRAIT|PORTRAIT_REVERSE))  
  {
		lcd_set_register_data(0x20, x);
		lcd_set_register_data(0x21, y);
	} else {
		lcd_set_register_data(0x21, x);
		lcd_set_register_data(0x20, y);
	}
}

void adjust_coordinate(void)
{
  uint16_t tmp;
  if((lcd.dir & (PORTRAIT|PORTRAIT_REVERSE))  && lcd.size_x > lcd.size_y)
  {
    tmp = lcd.size_x;
    lcd.size_x = lcd.size_y;
    lcd.size_y = tmp;
  }else if((lcd.dir & (LANDSCAPE|LANDSCAPE_REVERSE))  && lcd.size_x < lcd.size_y)
  {
    tmp = lcd.size_x;
    lcd.size_x = lcd.size_y;
    lcd.size_y = tmp;
  }  
}

void lcd_send_data(uint16_t data_cmd,uint8_t mode)
{
	CS_CLR;
  if(LCD_CMD == mode)
    DC_CLR;
  else
    DC_SET;
	RD_SET;
#if TRANSPORT_DATA_8BIT	
	WR_CLR;
	GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (data_cmd >> 8 & 0x00FF);
	WR_SET;
    
  WR_CLR;
	GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (data_cmd & 0x00FF);
	WR_SET;
#elif TRANSPORT_DATA_16BIT
  WR_CLR;
	GPIOB->ODR= data_cmd & 0xFFFF;
	WR_SET; 
#endif
}

void lcd_send_data_8bit(uint8_t data_cmd,uint8_t mode)
{
	CS_CLR;
  if(LCD_CMD == mode)
    DC_CLR;
  else
    DC_SET;
	RD_SET;

	WR_CLR;
	GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (data_cmd >> 8 & 0x00FF);
	WR_SET;

}

void lcd_set_register_data(uint16_t regid, uint16_t dat)
{
	lcd_send_data(regid, LCD_CMD); 
  lcd_send_data(dat, LCD_DATA);
}


void lcd_reset(void)
{
	RST_SET;
	RST_CLR;	//keep low level to perform reset
	delay_ms(50);
	RST_SET;
	delay_ms(50);
}

void lcd_init_registers(void)
{
//************* Start Initial Sequence **********// 
	lcd_set_register_data(0x00D0, 0x0003);
	lcd_set_register_data(0x00EB, 0x0B00);
	lcd_set_register_data(0x00EC, 0x004F); //0f
	lcd_set_register_data(0x00C7, 0x003F); //030f

	lcd_set_register_data(0x0001, 0x011C);
	lcd_set_register_data(0x0002, 0x0100); //0100
	lcd_set_register_data(0x0003, 0x1038);
	lcd_set_register_data(0x0007, 0x0000);
	lcd_set_register_data(0x0008, 0x0808);
  //lcd_set_register_data(0x000C, 0x0001);
	lcd_set_register_data(0x000F, 0x0A01);
	delay_ms(10);
  lcd_set_register_data(0x0B,0x1100); //frame cycle
	lcd_set_register_data(0x0C,0x0000); // RGB interface setting R0Ch=0x0110 for RGB 18Bit and R0Ch=0111for RGB16Bit
	lcd_set_register_data(0x0F,0x1401); // Set frame rate----0801
	lcd_set_register_data(0x15,0x0000); //set system interface
//------------- Power On sequence --------------//
	lcd_set_register_data(0x0010, 0x0000);
	lcd_set_register_data(0x0011, 0x1B41); //The register setting is suitable for VCI=2.8V
	delay_ms(50);
	lcd_set_register_data(0x0012, 0x300E); //The register setting is suitable for VCI=2.8V
	lcd_set_register_data(0x0013, 0x0061); //The register setting is suitable for VCI=2.8V
	lcd_set_register_data(0x0014, 0x5569); //The register51etting is suitable for VCI=2.8V   5569  4e65  00-7F65  vcomh
//-------------- Set GRAM area -----------------//
	lcd_set_register_data(0x0030, 0x0000);
	lcd_set_register_data(0x0031, 0x00DB);
	lcd_set_register_data(0x0032, 0x0000);
	lcd_set_register_data(0x0033, 0x0000);
	lcd_set_register_data(0x0034, 0x00DB);
	lcd_set_register_data(0x0035, 0x0000);
	lcd_set_register_data(0x0036, 0x00AF);
	lcd_set_register_data(0x0037, 0x0000);
	lcd_set_register_data(0x0038, 0x00DB);
	lcd_set_register_data(0x0039, 0x0000);
// ----------- Adjust the Gamma Curve ----------//     
	lcd_set_register_data(0x0050, 0x0100);
	lcd_set_register_data(0x0051, 0x0609);
	lcd_set_register_data(0x0052, 0x0c09);
	lcd_set_register_data(0x0053, 0x0205);
	lcd_set_register_data(0x0054, 0x090c);
	lcd_set_register_data(0x0055, 0x0906);
	lcd_set_register_data(0x0056, 0x0001);
	lcd_set_register_data(0x0057, 0x0502);
	lcd_set_register_data(0x0058, 0x0003);
	lcd_set_register_data(0x0059, 0x0300);
	lcd_set_register_data(0x0020, 0x0000);
	lcd_set_register_data(0x0021, 0x0000);
  //lcd_set_register_data(0x0007, 0x0012);
	lcd_set_register_data(0x0007, 0x1017);

  lcd_set_dir_regs();
}

void lcd_gpio_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if TRANSPORT_DATA_8BIT    
	GPIO_InitStructure.GPIO_Pin = ((uint16_t)0x00FF);
#elif TRANSPORT_DATA_16BIT
  GPIO_InitStructure.GPIO_Pin = ((uint16_t)0xFFFF); 
#endif 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void lcd_init(void)
{
  lcd_gpio_config();
  lcd_reset();
	lcd_init_registers();
}

//START END
//Calc gram address of rectangle.
void lcd_set_window(uint16_t start_x, uint16_t start_y, uint16_t end_x, uint16_t end_y)
{
  switch(lcd.dir)
  {
    case PORTRAIT:    
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HS, start_x );
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HE, end_x );
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VS, start_y );
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VE, end_y );

      lcd_set_register_data(ILI9225_RAM_ADDR_SET_HS, start_x ); 
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_VS, start_y );
      break;
    
    case LANDSCAPE:
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HS, start_y);
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HE, end_y);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VS, lcd.size_x-end_x-1);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VE, lcd.size_x-start_x-1);
        
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_HS, start_y);
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_VS, lcd.size_x-end_x-1);
      break; 
    
    case PORTRAIT_REVERSE:
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HS, lcd.size_x-end_x-1);
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HE, lcd.size_x-start_x-1);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VS, lcd.size_y-end_y-1);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VE, lcd.size_y-start_y-1);
        
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_HS, lcd.size_x-start_x-1);
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_VS, lcd.size_y-start_y-1);  
      break; 
    case LANDSCAPE_REVERSE:
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HS, lcd.size_y-end_y-1);
      lcd_set_register_data(ILI9225_HORIZONTAL_WINDOW_ADDR_HE, lcd.size_y-start_y-1);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VS, start_x);
      lcd_set_register_data(ILI9225_VERTICAL_WINDOW_ADDR_VE, end_x);
        
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_HS, lcd.size_y-start_y-1);
      lcd_set_register_data(ILI9225_RAM_ADDR_SET_VS, start_x);  
      break; 
    default:;
  }
  //ready for data
  lcd_send_data(ILI9225_GRAM_DATA_REG, LCD_CMD);
}
//START SIZE
void lcd_set_window_size(uint16_t start_x, uint16_t start_y, uint16_t size_x, uint16_t size_y)
{
  lcd_set_window(start_x, start_y, start_x + size_x -1, start_y + size_y-1);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
	lcd_set_window(x, y, x, y);  
	lcd_send_data(color, LCD_DATA);
}

void lcd_fill_rect(uint16_t start_x, uint16_t start_y, uint16_t size_x, uint16_t size_y, uint16_t color)
{
	uint32_t n, temp;
	lcd_set_window_size(start_x, start_y, size_x, size_y);
	temp = (uint32_t)size_x * size_y;
	for(n=0; n<temp; n++)
	{
		lcd_send_data(color, LCD_DATA);
	}
}

void lcd_clear(uint16_t color)
{
	uint32_t n, temp;
	lcd_set_window_size(0, 0, lcd.size_x, lcd.size_y);
	temp = (uint32_t)lcd.size_x * lcd.size_y;
	for(n=0; n<temp; n++)
	{
		lcd_send_data(color, LCD_DATA);
	}
}

void lcd_display_gray_bar(void)
{
  static uint16_t gray[]=
  {
    RGB(0x00, 0x00, 0x00),
    RGB(0x10, 0x10, 0x10),
    RGB(0x20, 0x20, 0x20),
    RGB(0x30, 0x30, 0x30),
    RGB(0x40, 0x40, 0x40),
    RGB(0x50, 0x50, 0x50),
    RGB(0x60, 0x60, 0x60),
    RGB(0x70, 0x70, 0x70),
    RGB(0x80, 0x80, 0x80),
    RGB(0x90, 0x90, 0x90),
    RGB(0xA0, 0xA0, 0xA0),
    RGB(0xB0, 0xB0, 0xB0),
    RGB(0xC0, 0xC0, 0xC0),
    RGB(0xD0, 0xD0, 0xD0),
    RGB(0xE0, 0xE0, 0xE0),
    RGB(0xF0, 0xF0, 0xF0),
  };
	int i=0,j;
	uint8_t step;
	step = lcd.size_y / 16;
	lcd_set_window_size(0, 0, lcd.size_x, lcd.size_y); 
	
	CS_CLR;
	DC_SET;
	RD_SET;
	
	for(j=0; j<lcd.size_y; j++)
	{
		for(i=0; i< lcd.size_x; i++)
		{
#if TRANSPORT_DATA_8BIT
			WR_CLR;
			GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (gray[j/step%16] >> 8 & 0xFF);
			WR_SET;
	
			WR_CLR;
			GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (gray[j/step%16] & 0xFF);
			WR_SET;
#elif TRANSPORT_DATA_16BIT
      WR_CLR;
      GPIOB->ODR= gray[j/step%16];
      WR_SET; 
#endif   
		}
	}
	
	CS_SET;	
}


void lcd_display_color_bar(void)
{
  static uint16_t colorbar[]=
  {
      RGB(0xFF, 0xFF, 0x00),
      RGB(0xFF, 0x00, 0x00),
      RGB(0x00, 0xFF, 0x00),
      RGB(0x00, 0x00, 0xFF),

      RGB(0xFF, 0x00, 0xFF),
      RGB(0x00, 0xFF, 0xFF),
      RGB(0x00, 0x00, 0x00),
      RGB(0xFF, 0xFF, 0xFF),

  };
	int i=0, j/*, index=0*/;
	uint8_t step;

	step = lcd.size_y / 8;
	lcd_set_window_size(0, 0, lcd.size_x, lcd.size_y); 
	
	CS_CLR;
	DC_SET;
	RD_SET;

	for(j=0; j<lcd.size_y; j++)
	{
		for(i=0; i< lcd.size_x; i++)
		{
#if TRANSPORT_DATA_8BIT
			WR_CLR;
			GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (colorbar[j/step%8] >> 8 & 0xFF);
			WR_SET;
	
			WR_CLR;
			GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (colorbar[j/step%8] & 0xFF);
			WR_SET;
#elif TRANSPORT_DATA_16BIT
      WR_CLR;
      GPIOB->ODR= colorbar[j/step%8];
      WR_SET; 
#endif
		}
	}
	
	CS_SET;	
}

void display_pics(uint16_t x, uint16_t y, uint16_t size_x, uint16_t size_y, const uint8_t *data)   
{
	uint32_t j, tmp;
  
	lcd_set_window_size(x, y, size_x, size_y);
  CS_CLR;
	DC_SET;
	RD_SET;
  tmp = size_x*size_y;
  
  for(j=0; j<tmp; j++)
	{
#if TRANSPORT_DATA_8BIT
    WR_CLR;
    GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (*(data+2*j) );
    WR_SET;
    
    WR_CLR;
    GPIOB->ODR= (GPIOB->ODR & 0xFF00) | (*(data+2*j+1) );
    WR_SET;
#elif TRANSPORT_DATA_16BIT
    WR_CLR;
    GPIOB->ODR= *(data+2*j) << 8 | *(data+2*j+1);
    WR_SET; 
#endif
  }
  
  CS_SET;
}  
