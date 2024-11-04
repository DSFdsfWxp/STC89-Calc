
#include "src/util/logic.h"

/**
 * 51 Calculator
 * (2024.10.11)
 * 
 * ====
 * Achieved features:
 * - Matrix keys input scan
 * - Lcd display
 * - Led status display
 * - Beeper alert
 * - Multi modes system
 * - Backspace and All Clear
 * - Boot and calc animations
 * - Input keys shift (key mapping)
 * 
 * ====
 * Mode system features:
 * - Mode switching
 * - Input filter for each mode
 * ====
 * Display system features:
 * - Long content display (Screen overflow process)
 * - Shift, Angle unit and Last result hint at led
 * - Result keeping two decimal places
 * - Scientific notation for long result
 * - Result `inf` detecting and display
 * ====
 * Input system features:
 * - Key mapping
 * - Error feedback (beep alert)
 * - Backspace and All Clear
 * - Input charactor pool size up to 45
 * - Cursor display
 * 
 * ====
 * Calc Mode Features:
 * - `+`, `-`, `*`, `/`, `^` operators
 * - `sin`, `cos`, `tan`, `floor`, `log10` functions
 * - `%`, `!` operators
 * - Float number calculating
 * - Calculating priority
 * - Negetive number calculating
 * - `()` supported
 * - Error catching (NOT FULLY)
 * - Two angle units supported (`Deg` and `Rad`)
 * - Max stack deep up to 3
 * ====
 * B2D Mode (Bin to Dec) Features:
 * - Max result up to 65535
 * ====
 * D2B Mode (Dec to Bin) Features:
 * - Max input up to 4095
 */

void main() {
    uLogicInit();
    while (1) uLogicLoop();
}