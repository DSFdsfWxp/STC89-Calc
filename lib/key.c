
#include <Atmel/REGX52.h>
#include "lib/time.h"
#include "lib/key.h"
#include "lib/lcd1602.h"

uint16 keyPressedTime;

int8 scanKey() {
	int8 row, res, out, mask, valid;

	P0 = out = mask = valid = keyPressedTime = 0;
	
	// scan the stand alone keys
	P3 = 0xff;
	// read in
	res = (~P3) & 0x0f;
	
	if (res) {
		// multi key pressed
		if (res!=1 && res%2) {
			P3 = 0xf0;
			return -1;
		}

		if (res > 2) res = res==4 ? 3 : 4;
		out = 16 + res;
		mask = 0x0f;
	} else {
		for (row=0; row<4; row++) {
			// scan the matrix keys
			// P34~P37 -> read, set to 1 to read
			// P30~P33 -> scan, set to 0 to select row
			P3 = 0xf0 | ~(uint8)(1 << row);

			res = ((~P3) & 0xf0) >> 4;

			// no key pressed in this row
			if (!res) continue;

			// multi key pressed
			if ((res!=1 && res%2) || (res && out)) {
				P3 = 0xf0;
				return -1;
			}

			if (res > 2) res = res==4 ? 3 : 4;
			out = row * 4 + res;
			mask = 0xf0;
		}
	}

	// make sure it's valid
	// avoid user pressing stand alone keys during matrix keys scan
	// if it's actually pressed, it should still be pressed now
	// if it's valid, wait for releasing
	if (mask) {
		P3 = mask | 0xf0;

		delayMs(20);
		while ((~P3) & mask) {
			valid = 1;
			keyPressedTime ++;
			delayMs(1);
		}
		delayMs(20);

		P3 = 0xf0;
		return valid ? out : 0;
	} else {
		// no key pressed
		P3 = 0xf0;
		return 0;
	}
}