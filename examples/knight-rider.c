#include "hw.h"
#include "delay.h"

void main(void)
{
	char i = 0;
	while (1) {
		char count = 5;
	        while (count--) {
			for (i = 0; i < 8; i++) {
				P1 = ~(1 << i);
				//P2 ^= (1 << 4);
				delay_ms(100);
			}
			for (; --i >= 0;) {
				P1 = ~(1 << i);
				//P2 ^= (1 << 4);
				delay_ms(100);
			}
		}
		count = 5;
       		while (count--) {
			unsigned char x;
			for (i = 0; i < 4; i++) {
				x = (1 << i);
				x = x | (x << 4);
				P1 = ~x;
				//P2 ^= (1 << 4);
				delay_ms(100);
			}
			for (; --i >= 0;) {
				x = (1 << i);
				x = x | (x << 4);
				P1 = ~x;
				//P2 ^= (1 << 4);
				delay_ms(100);
			}
		}
	}
}
