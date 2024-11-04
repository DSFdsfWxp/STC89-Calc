
#include <Atmel/REGX52.H>
#include "lib/led.h"
#include "lib/time.h"
#include "lib/lcd1602.h"

struct {
    int16 speed;
    int8 count;
    uint16 litTime;
    int8 phase;
} ledPwmContext;

uint8 ledPwmMask = 0xff;

void ledPwmInterrupt() interrupt 1 {
    uint16 nextStartPoint;

    TR0 = 0;
    if (!ledPwmContext.speed) return;

    P1 = P1 & ledPwmMask ? ~ledPwmMask : 0xff;
    
    if (P1) {
        if (0x2b33u - ledPwmContext.litTime <= ledPwmContext.speed && ledPwmContext.phase > 0) {
            ledPwmContext.litTime = 0x2b33u;
            ledPwmContext.phase = -1;
        } else if (ledPwmContext.litTime <= ledPwmContext.speed && ledPwmContext.phase < 0) {
            ledPwmContext.litTime = 0x0;
            ledPwmContext.phase = 1;
            if (!(--ledPwmContext.count)) {
                P1 = 0xff;
                return;
            }
        } else {
            ledPwmContext.litTime += ledPwmContext.phase * ledPwmContext.speed;
        }
    }

    nextStartPoint = P1 & ledPwmMask ? ledPwmContext.litTime + 0xd4ccu : 0xffffu - ledPwmContext.litTime;
    TL0 = nextStartPoint;
    TH0 = nextStartPoint >> 8;
    TF0 = 0;
    TR0 = 1;
}

void ledPwm(int16 speed, int8 count) {
    ledPwmContext.speed = speed;
    ledPwmContext.count = count;
    ledPwmContext.litTime = 0;
    ledPwmContext.phase = 1;

    // 16 bit mode (manual reload)
    TMOD &= 0xf0;
    TMOD |= 1;

    // 1ms a block
    TL0 = 0xcc;
    TH0 = 0xd4;

    // setup
    TF0 = 0;
    ET0 = TR0 = 1;

    // enable global interrupt
    EA = 1;
}
