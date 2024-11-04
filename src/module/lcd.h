
#ifndef _src_module_lcd
#define _src_module_lcd

#include "lib/type.h"

#define LCD_MAX_POOL_SIZE 45
#define LCD_MAX_RESULT_INTEGER_LEN 8

#define LCD_C_Sin   (uint8)115
#define LCD_C_Cos   (uint8) 99
#define LCD_C_Tan   (uint8)116
#define LCD_C_Log10 (uint8)108
#define LCD_C_Floor (uint8)102

#define LCD_C_Div   (uint8)253
#define LCD_C_Ptr   (uint8)255

extern bool mLcdHasErr;
extern uint8 mLcdPool[LCD_MAX_POOL_SIZE + 1];

void mLcdInit();
void mLcdReset();

void mLcdSetHint(const char *hint);
void mLcdSetOutputF(float num);
void mLcdSetOutputC(const char* txt);

void mLcdInsert(char c);
void mLcdDelete();
void mLcdClear();

#endif /* _src_module_lcd */