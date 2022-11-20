/**********************************************************************
* Develop by cheng_dev
* date: 2022.11
* Julia set 
***********************************************************************/

#ifndef __LED_H
#define __LED_H
#include <stm32f10x.h>

#define RGB565(r,g,b)       ((unsigned short int)((((r)>>3)<<11) | (((g)>>2)<<5)	| ((b)>>3) ))

#define JULIA_EDGE 1.2

typedef struct{	
	int16_t  R;
	int16_t  I;
}CompleI;

typedef struct{	
	double  R;
	double  I;
}CompleF;

void juliaF(uint16_t *data, uint16_t a,uint16_t b);
void juliaI(uint16_t *data, uint16_t size_x,uint16_t size_y);
void julia_init(void);

#endif



