/**********************************************************************
* Develop by cheng_dev
* date: 2022.11
* Driver for ILI9225
***********************************************************************/


#include <stm32f10x.h>
#include "delay.h"
#include <stdio.h>
#include "ili9225.h"
#include "julia_set.h"
#include "pics.h"

#define JULIA_SIZE 60//limited by ram

extern int16_t c_rm, c_im;
extern double c_rmf, c_imf;

static uint16_t frac_data[JULIA_SIZE*JULIA_SIZE];
void testJuliaISet(void);
void testJuliaFSet(void);
int main(void)
{
  uint8_t counter=0;
  LCD_TYPE lcd = {
    IC_ILI9225,
    176,
    220,
    PORTRAIT//PORTRAIT LANDSCAPE PORTRAIT_REVERSE LANDSCAPE_REVERSE
  };
  
	delay_init(72);	//when overclock, max delay_ms is 1864
  
  lcd_set_type(lcd);//before lcd_init()
	lcd_init();
  
#if 1  
  //change direction
  //lcd_set_dir(LANDSCAPE_REVERSE);
  
  lcd_clear(RGB(0, 0, 255));
  delay_ms(1000);

	lcd_draw_point(30, 80, RGB(0, 63, 255));
  while (counter++ <1)
	{
		lcd_display_color_bar();
		delay_ms(1500);
		lcd_display_gray_bar();
		delay_ms(1500);
		lcd_fill_rect(10, 30, 150, 100, RGB(255, 0, 0));
		delay_ms(1500);
		lcd_fill_rect(20, 0, 176, 120, RGB(0, 255, 0));
		delay_ms(1500);
		lcd_fill_rect(0, 20, 176, 120, RGB(0, 0, 255));
		delay_ms(1500);
    
    lcd_clear(RGB(255, 0, 0));
    display_pics(20,50,96,96,gImage_bg2);
    display_pics(160,50,10,10,gImage_bg);
    delay_ms(1500);
	}
#endif

#if 1 
  lcd_clear(RGB(0, 0, 0));
  testJuliaFSet();
  //testJuliaISet();
#endif  
}
//fast
void testJuliaISet(void)
{
  julia_init();
  while(1)
  {
    juliaI(frac_data,JULIA_SIZE,JULIA_SIZE);
    display_pics((176-JULIA_SIZE)>>1,(220-JULIA_SIZE)>>1,JULIA_SIZE,JULIA_SIZE,(u8*)frac_data); 
    delay_ms(1);
    c_rm +=1;
    c_im +=1;
    if(c_rm > 50)
    {
      c_rm=-52;
      c_im=-56;
    }
  }  
  
}

void testJuliaFSet(void)
{
  julia_init();
  while(1)
  {
    juliaF(frac_data,JULIA_SIZE,JULIA_SIZE);
    display_pics((176-JULIA_SIZE)>>1,(220-JULIA_SIZE)>>1,JULIA_SIZE,JULIA_SIZE,(u8*)frac_data); 
    delay_ms(1);
    c_rmf +=0.01;
    c_imf +=0.02;
    if(c_rmf > 3.)
    {
      c_rmf=0.23;
      c_imf=0.20;
    }
  }  
  
}

