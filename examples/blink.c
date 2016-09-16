#include "hw.h"
#include "delay.h"

void main(void){
    /* at89lp specific: configure P1.3 as push pull output */
	P1M0 &= ~(1<<3);
	P1M1 |= (1<<3);

	P1_3 = 0;   
	while (1)
	{
		delay_ms(500);
		P1_3 = !P1_3;
	}
}
