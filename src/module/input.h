
#ifndef _src_module_input
#define _src_module_input

#include "lib/type.h"

#define KEY_N_SpecialRegionStart  (uint8)128
#define KEY_N_SpecialRegionEnd    (uint8)159

#define KEY_MODE_D2B   (uint8)128
#define KEY_MODE_B2D   (uint8)129
#define KEY_MODE_CALC  (uint8)131

#define KEY_OP_Equal            (uint8)132
#define KEY_OP_AngleUnitChange  (uint8)133
#define KEY_OP_AC               (uint8)134
#define KEY_OP_Del              (uint8)135

#define KEY_KEY_SHIFT  (uint8)158
#define KEY_KEY_UNUSED (uint8)159

uint8 mInputScan();

#endif /* _src_module_input */