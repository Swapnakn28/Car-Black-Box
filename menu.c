



#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"

//lp - long press
//ptr - ind seleting


extern unsigned int main_f, menu_f;

void menu(char key) {
    CLEAR_DISP_SCREEN;
    char options[][17] = {"View Log        ", "Download Log    ", "Clear Log       ", "Set Time         ", "Change Password  "};
    unsigned int ind = 0, count = 0, lp12 = 0, lp11 = 0, ptr = 0;
    for(unsigned long i = 0;i<160000;i++);
    while (1) {
        key = read_switches(LEVEL_CHANGE);
        switch_scroll(key);
        if (count % 2 == 0) // displaying -> in top in clcd
        {
            clcd_putch(0x7E, LINE1(0));
            clcd_putch(' ', LINE2(0));
        }
        clcd_putch(' ', LINE1(1)); //displaying options 
        clcd_putch(' ', LINE2(1));
        clcd_print(options[ind], LINE1(2));
        clcd_print(options[ind + 1], LINE2(2));

        if (key == MK_SW11) // long press - selecting option
        {
            lp11++;
            if (lp11 > 300) //long press
            {
                main_f = 3; // menu enter
                menu_f = ptr; //menu flag will updated to the -> option index
                lp11 = 0;
                return;
            }
        } else if (lp11 < 300 && lp11 != 0) //short press  scroll up
        {
            lp11 = 0;
            if (count > 0 && count--) {
                if (count % 2 != 0) {
                    ind--;
                    clcd_putch(' ', LINE1(0)); // displaying -> in bottome in clcd
                    clcd_putch(0x7e, LINE2(0));
                } else {
                    ptr--; //decrementing -> pointing index
                }
            }
        }
        else if (key == MK_SW12) //sw12 long press returning to dashboard
        {
            lp12++;
            if (lp12 > 300) {
                main_f = 0;
                lp12 = 0;
                return;
            }
        } else if (lp12 < 300 && lp12 != 0) //sw12 short press ind will incrementing
        {
            lp12 = 0;
            if (count < 7 && ++count) {
                if (count % 2 == 0) {
                    ind++;
                } else {
                    ptr++; //pointer will also increment if count is odd 
                    clcd_putch(' ', LINE1(0)); // -> is in line 2
                    clcd_putch(0x7e, LINE2(0));
                }
            }
        }
    }
}