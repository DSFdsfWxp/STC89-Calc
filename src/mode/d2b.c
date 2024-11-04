
#include <STDLIB.H>
#include <STRING.H>
#include "src/module/lcd.h"
#include "src/mode/d2b.h"

void moD2bInit() {
    mLcdSetHint("D2B ");
}

bool moD2bInputFilter(uint8 kval) {
    return kval >= '0' && kval <= '9';
}

bool moD2bExecute(const char *input) {
    uint16 n, i;
    char txt[13];

    if (!input[0]) return false;
    n = atoi(input);
    if (n > 4095) return false;
    memset(txt, '0', 12);
    txt[12] = 0;

    for (i = 11; i<12; i--) {
        txt[i] = '0' + (n & 1);
        n = n >> 1;
    }

    mLcdSetOutputC(txt);

    return true;
}