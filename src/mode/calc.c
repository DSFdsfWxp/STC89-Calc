
#include <STDLIB.H>
#include <STRING.H>
#include <MATH.H>
#include "src/module/status.h"
#include "src/module/lcd.h"
#include "src/mode/calc.h"

#define PI 3.1415927f
#define isDigit(c) (c >= '0' && c <= '9')

typedef struct {
    /** Left Symbol (e.g.:`+`) */
    int8 lSym;
    /** Left Extra Symbol (e.g.:`sin`) */
    int8 lExSym;
    /** Number */
    float num;
    /** Right Extra Symbol (e.g.:`!`) */
    int8 rExSym;
    /** Right Symbol / Left Symbol of next block */
    int8 rSym;
} block_t;

// Last calc result
// Avoid ptr operate costing too much code space
float calcResult = 0.0f;

// operator `!`
float fact(float v) {
    float res = 1, i;
    for (i=1; i<=v; i++) res *= i;
    return res;
}

float cAtof(const char *s) {
    float a = 0.0f;
    int8 e = 0;
    int8 c;

    // integer part
    while ((c = *s++) != '\0' && isDigit(c)) {
        a = a * 10.0f + (c - '0');
    }
    // rest part
    if (c == '.') {
        while ((c = *s++) != '\0' && isDigit(c)) {
        a = a * 10.0f + (c - '0');
        e = e - 1;
        }
    }
    // move '.'
    while (e < 0) {
        a *= 0.1f;
        e ++;
    }

    return a;
}

bool calc(const char *txt, uint8 len) reentrant {
    float reg[3] = {0};
    int8 symReg[2] = {255, 255};
    block_t block = {0, 0, 0.0f, 0, 0};
    int8 i, tmp, tmp2;
    static uint8 deep = 0;

    // stack deep check
    if (++ deep > 3) {
        deep = 0;
        return false;
    }
    // something starts with '-'
    if (txt[0] == '-' || txt[0] == '+')
        block.rSym = txt[i++] == '-';

    for (i=0; i<len; i++) {
        
        // parse from string

        block.lExSym = i;
        while (txt[i] <= 't' &&
               txt[i] >= 'a')
            block.lExSym = i ++;
        
        if (txt[i] == '(') {
            tmp = ++ i;
            tmp2 = 1;
            while (txt[i] && tmp2) {
                switch (txt[i ++]) {
                    case '(':
                        tmp2 ++;
                        break;
                    case ')':
                        tmp2 --;
                        break;
                }
            }
            if (!calc(txt + tmp, i - tmp)) {
                deep = 0;
                return false;
            }
            block.num = calcResult;
        } else {
            block.num = cAtof(txt + i);
            while (txt[i] >= '.' &&
                   txt[i] <= '9')
                i ++;
        }

        block.rExSym = i;
        while (txt[i] <= '%' && txt[i])
            i ++;
        
        // parse symbol
        // symbol id / 2 == level id
        block.lSym = block.rSym;
        switch (txt[i]) {
            case ')':
            case 0:
            case '+':
                block.rSym = 0;
                break;
            case '-':
                block.rSym = 1;
                break;
            case 'x':
                block.rSym = 2;
                break;
            case LCD_C_Div:
                block.rSym = 3;
                break;
            case '^':
                block.rSym = 4;
                break;
        }

        // calculate block value

        // functions
        for (tmp=block.lExSym; 
             tmp>=0 && txt[tmp]>='a' && txt[tmp]<='t';
             tmp--)
        {
            switch (txt[tmp]) {
                case 's':
                    block.num = sin(!LED_Rnd ? block.num : block.num * PI / 180.0f);
                    break;
                case 'c':
                    block.num = cos(!LED_Rnd ? block.num : block.num * PI / 180.0f);
                    break;
                case 't':
                    block.num = tan(!LED_Rnd ? block.num : block.num * PI / 180.0f);
                    break;
                case 'f':
                    block.num = floor(block.num);
                    break;
                case 'l':
                    if (block.num <= 0) {
                        deep = 0;
                        return false;
                    }
                    block.num = log10(block.num);
                    break;
            }
        }

        // `!`, `%` operators
        for (tmp=block.rExSym;
             txt[tmp] && (txt[tmp] <= '%' && txt[tmp]);
             tmp++)
        {
            switch (txt[tmp]) {
                case '!':
                    if (block.num != floor(block.num) || block.num < 0) {
                        deep = 0;
                        return false;
                    }
                    block.num = fact(block.num);
                    break;
                case '%':
                    block.num /= 100.0f;
                    break;
            }
        }

        // add to reg
        // process level up or down

        // save symbols for level down
        if (block.lSym < 4) symReg[block.lSym/2] = block.lSym;

        if (block.lSym/2 >= block.rSym/2) {
            // save to block value to current level's reg
            switch (block.lSym) {
                case 0:
                case 1:
                    reg[0] += block.lSym ? -block.num : block.num;
                    break;
                case 2:
                case 3:
                    if (block.lSym == 3 && block.num == 0) {
                        deep = 0;
                        return false;
                    }
                    reg[1] = block.lSym == 2 ?
                        reg[1] * block.num : reg[1] / block.num;
                    break;
                case 4:
                    reg[2] = pow(reg[2], block.num);
                    break;
            }

            // level down
            if (block.lSym/2 != block.rSym/2) {
                for (tmp=block.lSym/2-1; tmp>=block.rSym/2; tmp--) {
                    switch (symReg[tmp]) {
                        case 0:
                        case 1:
                            reg[0] += symReg[tmp] ? -reg[1] : reg[1];
                            break;
                        case 2:
                        case 3:
                            reg[1] = symReg[tmp] == 2 ?
                                reg[1] * reg[2] : reg[1] / reg[2];
                            break;
                        case 255: // 255 for never used or reset
                            reg[tmp] = reg[tmp+1];
                            break;
                    }
                }
                // reset
                if (block.rSym/2 == 0) symReg[1] = 255;
            }
        } else{
            // level up
            reg[block.rSym/2] = block.num;
        }
    }

    calcResult = reg[0];
    deep --;
    return true;
}

void moCalcInit() {
    mLcdSetHint(NULL);
}

bool moCalcInputFilter(uint8 kval) {
    return true;
}

bool moCalcExecute(const char *input) {
    if (calc(input, strlen(input))) {
        mLcdSetOutputF(calcResult);
        return true;
    } else {
        mLcdSetOutputC("Error");
        return false;
    }
}