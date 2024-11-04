
#ifndef _lib_key
#define _lib_key

#include "lib/type.h"

/**
 * @brief last key pressed time (ms)
 */
extern uint16 keyPressedTime;

/**
 * @brief scan key input
 * @retval `0` - no any keys down
 * @retval `-1` - multi keys down
 * @retval `1~16` - matrix keys down
 * @retval `17~20` - stand alone keys down
 */
int8 scanKey();

#endif /* _lib_key */