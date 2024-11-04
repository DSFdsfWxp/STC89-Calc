
#include <Atmel/REGX52.H>
#include "lib/time.h"
#include "src/module/sound.h"

void mSoundAlert() {
    P2_3 = 0;
    delayMs(25);
    P2_3 = 1;
}

void mSoundReset() {
    P2_3 = 1;
}