
#ifndef _src_mode_calc
#define _src_mode_calc

#include "lib/type.h"

void moCalcInit();

bool moCalcInputFilter(uint8 kval);

bool moCalcExecute(const char *input);

#endif /* _src_mode_calc */