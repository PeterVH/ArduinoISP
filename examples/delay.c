
#include "hw.h"

void delay_ms(int time)
{
 int i, j;
 for (j=0; j< time; j++)
   for (i=0; i<(OSCILLATOR/10000); i++)
     ;
}

