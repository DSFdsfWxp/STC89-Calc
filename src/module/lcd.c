
#include <MATH.H>
#include <STRING.H>
#include "lib/lcd1602.h"
#include "src/module/lcd.h"

int8 lHintLen;
int8 lUsedPoolSize;

uint8 mLcdPool[LCD_MAX_POOL_SIZE + 1];
bool mLcdHasErr;

void mLcdReset() {
    lHintLen = 0;
    mLcdHasErr = false;
    mLcdClear();
    LCD1602_Cls();
}

void mLcdInit() {
    LCD1602_Init();
    mLcdReset();
}

void mLcdSetHint(const char *hint) {
    if (hint) {
        lHintLen = (int8) strlen(hint);
        LCD1602_DispString(1, 1, hint);
    }
    LCD1602_DispChar(1, 1 + lHintLen, LCD_C_Ptr);
}

void mLcdClearOutput() {
    int8 i;
    for (i=1; i<=16; i++)
        LCD1602_DispChar(2, i, ' ');
}

void mLcdSetOutputF(float num) {
    int8 len, p, i, negetive;
    uint32 tmp;

    // start from right side
    p = 16;
    negetive = num < 0 ? 1 : 0;
    if (negetive) num = -num;
    len = (int8)log10(num) + 1;
    
    if (len <= 0) len = 1;
    if (len > LCD_MAX_RESULT_INTEGER_LEN) num /= pow(10, len - 1);
    if (num * 100.0f - floor(num * 100.0f) >= 0.5f) num += 0.01f;

    mLcdClearOutput();

    if (!num) {
        LCD1602_DispChar(2, 16, '0');
        return;
    }

    if (num / 2.0f == num) {
        LCD1602_DispString(2, 14, "INF");
        return;
    }

    // Scientific notation
    if (len > LCD_MAX_RESULT_INTEGER_LEN) {
        LCD1602_DispUnInt(2, 15, len - 1, 2);
        LCD1602_DispChar(2, p -= 2, 'e');
        len = (uint8)log10(num) + 1;
        p --;
    }

    // Not a integer
    if (num!=floor(num)) {
        tmp = (uint32) ((num - floor(num)) * 100.0f);
        for (i=0; i<2; i++) {
            LCD1602_DispChar(2, p --, '0' + tmp % 10);
            tmp /= 10;
        }
        LCD1602_DispChar(2, p --, '.');
    }

    // Integer part
    tmp = (uint32) num;
    for (i=0; i<len; i++) {
        LCD1602_DispChar(2, p --, '0' + tmp % 10);
        tmp /= 10;
    }

    // Symbol part
    if (negetive) LCD1602_DispChar(2, p --, '-');
}

void mLcdSetOutputC(const char* txt) {
    uint8 len = strlen(txt);
    mLcdClearOutput();
    LCD1602_DispString(2, len > 16 ? 1 : 16 - len + 1, txt);
}

void mLcdFlush() {
    uint8 p, pp;

    // long content display
    pp = lUsedPoolSize > 15 - lHintLen ? lUsedPoolSize - 15 + lHintLen : 0;
    p = lHintLen + 1;

    while (pp < lUsedPoolSize)
        LCD1602_DispChar(1, p ++, mLcdPool[pp ++]);
    
    LCD1602_DispChar(1, p ++, LCD_C_Ptr);

    // fill the rest
    while (p <= 16)
        LCD1602_DispChar(1, p ++, ' ');
}

void mLcdInsert(char c) {
    if (lUsedPoolSize == LCD_MAX_POOL_SIZE) {
        mLcdHasErr = true;
        return;
    }
    
    mLcdPool[lUsedPoolSize ++] = c;
    mLcdFlush();
}

void mLcdDelete() {
    if (lUsedPoolSize == 0) {
        mLcdHasErr = true;
        return;
    }

    mLcdPool[-- lUsedPoolSize] = 0;
    mLcdFlush();
}

void mLcdClear() {
    memset(mLcdPool, 0, LCD_MAX_POOL_SIZE + 1);
    lUsedPoolSize = 0;
    mLcdFlush();
}