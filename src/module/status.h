
#ifndef _src_module_status
#define _src_module_status

#include <Atmel/REGX52.H>

/** Is key shift on */
sbit LED_Shift        = P1 ^ 7;
/** Is Rnd the angle unit current */
sbit LED_Rnd          = P1 ^ 6;
/** Is Deg the angle unit current */
sbit LED_Deg          = P1 ^ 5;
/** Is the input modified after last result displayed */
sbit LED_LastResult   = P1 ^ 0;

void mLedReset();

#endif /* _src_module_status */