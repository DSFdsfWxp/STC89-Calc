
#include <Atmel/REGX52.h>
#include "lib/type.h"
#include "lib/led.h"
#include "lib/time.h"
#include "lib/lcd1602.h"
#include "src/module/sound.h"
#include "src/anime.h"

code const char title[] = "51 Calculator";

void uAnimeBoot() {
    int8 i;

    LCD1602_Cls();
    ledPwmMask = 0xff;
    ledPwm(175, 1);
    
    for (i=0; i<sizeof(title)-1; i++) {
        delayMs(25);
        LCD1602_DispChar(1, 1 + i, title[i]);
    }

    for (i=0; i<6; i++) {
        delayMs(200);
        LCD1602_DispString(2,10, i%2 ? "       " : "LOADING");
    }

    mSoundAlert();
    LCD1602_Cls();
}

void uAnimeCalc() {
    uint8 ledStatus = P1;

    ledPwmMask = 0x80; // 0b10000000
    ledPwm(200, 1);
    while (ledPwmMask < 0xff) {
        delayMs(37);
        // lit direction -->
        ledPwmMask |= ledPwmMask >> 1;
    }

    delayMs(1000);
    P1 = ledStatus;
}