
#ifndef _lib_led
#define _lib_led

#include "lib/type.h"

/** `0` for off, `1` for on */
extern uint8 ledPwmMask;

void ledPwm(int16 speed, int8 count);

#endif /* _lib_led */