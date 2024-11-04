
#include <INTRINS.H>
#include "lib/time.h"

void delayMs(uint16 ms) {
	unsigned char data i, j;

	while(ms) {
		_nop_();
		i = 2;
		j = 199;
		do {
			while (--j);
		} while (--i);
		ms--;
	}
}