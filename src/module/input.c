
#include "lib/type.h"
#include "lib/key.h"
#include "src/module/lcd.h"
#include "src/module/status.h"
#include "src/module/input.h"

code const uint8 keyMap[] = {
    '7',           '8',            '9',          LCD_C_Div,
    '4',           '5',            '6',          'x',
    '1',           '2',            '3',          '-',
    '0',           '.',            '%',          '+',
    KEY_KEY_SHIFT, KEY_KEY_UNUSED, KEY_OP_Del,   KEY_OP_Equal
};

code const uint8 keyShiftMap[] = {
    KEY_MODE_CALC, KEY_MODE_B2D,   KEY_MODE_D2B,   KEY_KEY_UNUSED,
    LCD_C_Sin,     LCD_C_Cos,      LCD_C_Tan,      KEY_KEY_UNUSED,
    LCD_C_Floor,   LCD_C_Log10,    KEY_KEY_UNUSED, '!',
    '(',           ')',            '%',            '^',
    KEY_KEY_SHIFT, KEY_KEY_UNUSED, KEY_KEY_UNUSED, KEY_OP_AngleUnitChange
};

uint8 mInputScan() {
    data int8 k = scanKey() - 1;

    if (k < 0) return 0;

    if (keyMap[k] == KEY_KEY_SHIFT) {
        LED_Shift = !LED_Shift;
        return 0;
    }
    
    if (!LED_Shift) {
        LED_Shift = 1;
        return keyShiftMap[k];
    } else if (keyPressedTime > 500 && keyMap[k] == KEY_OP_Del) {
        return KEY_OP_AC;
    } else {
        return keyMap[k];
    }
}