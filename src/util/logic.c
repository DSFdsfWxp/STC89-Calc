
#include "lib/type.h"
#include "lib/time.h"
#include "src/mode/b2d.h"
#include "src/mode/d2b.h"
#include "src/mode/calc.h"
#include "src/module/input.h"
#include "src/module/lcd.h"
#include "src/module/status.h"
#include "src/module/sound.h"
#include "src/util/anime.h"
#include "src/util/logic.h"

typedef struct {
    const void (*init)();
    const bool (*inputFilter)(uint8 val);
    const bool (*execute)(const char *input);
} calcMode_t;

code const calcMode_t calcModes[] = {
    {moB2dInit, moB2dInputFilter, moB2dExecute},
    {moD2bInit, moD2bInputFilter, moD2bExecute},
    {moCalcInit, moCalcInputFilter, moCalcExecute}
};

code const uint8 calcModeToKeyValMap[] = {
    KEY_MODE_B2D,
    KEY_MODE_D2B,
    KEY_MODE_CALC
};

uint8 ulCurrentMode;

void uLogicInit() {
    mLcdInit();
    mLedReset();
    mSoundReset();
    ulCurrentMode = 2;

    delayMs(500);
    uAnimeBoot();

    LED_Deg = 0;
    calcModes[2].init();
}

bool uLogicTrySwitchMode(uint8 kval) {
    uint8 i;
    for (i=0; i<sizeof(calcModeToKeyValMap)/sizeof(uint8); i++) {
        if (calcModeToKeyValMap[i] == kval) {
            mLcdReset();
            mLedReset();
            mSoundReset();
            LED_Deg = 0;
            
            ulCurrentMode = i;
            delayMs(200);
            calcModes[i].init();

            return true;
        }
    }
    return false;
}

void uLogicLoop() {
    uint8 kval;

    kval = mInputScan();
    delayMs(10);

    if (kval) {
        if (kval >= KEY_N_SpecialRegionStart &&
            kval <= KEY_N_SpecialRegionEnd)
        {
            if (!uLogicTrySwitchMode(kval)) {
                switch (kval) {
                    case KEY_OP_Equal:
                        if (calcModes[ulCurrentMode].execute(mLcdPool)) {
                            LED_LastResult = 1;
                            uAnimeCalc();
                        } else mSoundAlert();
                        break;
                    case KEY_OP_AngleUnitChange:
                        LED_Deg = !LED_Deg;
                        LED_Rnd = !LED_Rnd;
                        break;
                    case KEY_OP_AC:
                        mLcdClear();
                        break;
                    case KEY_OP_Del:
                        LED_LastResult = 0;
                        mLcdDelete();
                        break;
                    default:
                        mSoundAlert();
                        break;
                }
            }
        } else if (calcModes[ulCurrentMode].inputFilter(kval)) {
            LED_LastResult = 0;
            mLcdInsert(kval);
        } else mSoundAlert();

        if (mLcdHasErr) mSoundAlert();
        mLcdHasErr = false;
    }
}