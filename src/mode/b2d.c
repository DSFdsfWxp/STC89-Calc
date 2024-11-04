
#include <STRING.H>
#include "src/module/lcd.h"
#include "src/mode/b2d.h"

void moB2dInit() {
    mLcdSetHint("B2D ");
}

bool moB2dInputFilter(uint8 kval) {
    return kval == '0' || kval == '1';
}

bool moB2dExecute(const char *input) {
    int8 i;
    uint32 res;

    res = 0;
    if (!input[0]) return false;

    for (i=0; input[i]; i++) {
        if (i == 16) return false;

        res = res << 1;
        if (input[i] == '1') res |= 1;
    }

    mLcdSetOutputF(res);

    return true;
}