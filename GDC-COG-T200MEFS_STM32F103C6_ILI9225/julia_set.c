/**********************************************************************
* Develop by cheng_dev
* date: 2022.11
* Julia set 
***********************************************************************/

#include "julia_set.h"

static CompleI x0;
static CompleF xf;

static uint16_t colors[128];

int16_t x_minI=-30, y_minI=-30;
double x_minF=-JULIA_EDGE, y_minF=-JULIA_EDGE, x_maxF=JULIA_EDGE, y_maxF=JULIA_EDGE;

int K=25;//100
long M=25000;//100

int KF=10;//100
double MF=64.;//100

int16_t c_rm=-52, c_im=-56;
double c_rmf=0.23, c_imf=0.24;

void julia_init(void)
{
  uint16_t i;
	for(i = 0;i < 128;i++)
	{
		colors[i]= RGB565(i*7%128,(i+1)*5%128,(i+2)*3%128); 
	}
}

int16_t fastlog(int16_t x)
{
  float tmp;
  uint32_t ix, exp;
  tmp = (float)x;
  ix = *(uint32_t*)&tmp;
  exp = (ix >> 23) & 0xff;
  return exp-127;
}

void juliaI(uint16_t *data, uint16_t size_x,uint16_t size_y)
{	
  uint16_t i, j, tmp_rm, *p;
  int k;
  p = data;

	for(i=0;i<size_x;i++)
  {
		for(j=0;j<size_y;j++)
    {
			k=0;
			x0.R=x_minI+i;
			x0.I=y_minI+j;
      //x0=x0^2+C C=p+q*i
      while(k++<K)
      {
        tmp_rm = x0.R;
        x0.R=x0.R*x0.R-x0.I*x0.I + c_rm;
        x0.I=2*tmp_rm*x0.I+c_im;        
        if((x0.R*x0.R+x0.I*x0.I)>>1 > M)
        {
          *(p++) = colors[k%128];
          break;
        } 
      }
			if((x0.R*x0.R+x0.I*x0.I)>>1 <= M)
      {
        *(p++) = colors[0];
      } 
		}
	}
  
}

void juliaF(uint16_t *data, uint16_t size_x,uint16_t size_y)
{	
  double dx,dy,tmp_rm=0;
  uint16_t i, j, *p;
  int k;
  p = data;
	dx=(x_maxF-x_minF)/(size_x-1);
	dy=(y_maxF-y_minF)/(size_y-1);
	for(i=0;i<size_x;i++)
  {
		for(j=0;j<size_y;j++)
    {
			k=0;
			xf.R=x_minF+i*dx;
			xf.I=y_minF+j*dy;
      //xf=xf^2+C C=c_rmf+c_imf*i
      while(k++<KF)
      {
        tmp_rm = xf.R;
        xf.R=xf.R*xf.R-xf.I*xf.I + c_rmf;
        xf.I=2*tmp_rm*xf.I+c_imf;        
        if(xf.R*xf.R+xf.I*xf.I > MF)
        {
          *(p++) = colors[k];
          break;
        } 
      }
			if(xf.R*xf.R+xf.I*xf.I <= MF)
      {
        *(p++) = colors[0];
      } 
		}
	}
  
}

//Julia(data,160,80);



