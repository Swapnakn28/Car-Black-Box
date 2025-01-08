
/*
 * File:   
 * Author: <NAME>
 *
 * Created on 1 December, 2023, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "matrix_keypad.h"
#include "i2c.h"
#include "eeprom.h"
extern unsigned int log_count, main_f, ev_ind, prev_ind;

void view_log(char key) {
    static unsigned int log = 0, lp12 = 0;
    key = read_switches(LEVEL_CHANGE); //reading switches
    if (key == MK_SW12) //scroll down
    {
        lp12++;
        if (lp12 > 30) //long press
        {
            main_f = 2; //menu 
            log = 0, lp12 = 0; //updating to 0's
            return;
        }
    } else if (lp12 < 30 && lp12 != 0) //short press sw12
    {
        lp12 = 0;
        if (log < (log_count - 1)) //incrementing log 
            log++;
    } else if (key == MK_SW11) //scroll up
    {
        if (log > 0) //decrementing log when log > 0
            log--;
    }
    clcd_print("# TIME     EV SP ", LINE1(0)); //displaying that log event in clcd
    clcd_putch(log + '0', LINE2(0));
    clcd_putch(' ', LINE2(1));
    clcd_putch(read_ext_eeprom(log * 10 + 0), LINE2(2));
    clcd_putch(read_ext_eeprom(log * 10 + 1), LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(read_ext_eeprom(log * 10 + 2), LINE2(5));
    clcd_putch(read_ext_eeprom(log * 10 + 3), LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(read_ext_eeprom(log * 10 + 4), LINE2(8));
    clcd_putch(read_ext_eeprom(log * 10 + 5), LINE2(9));
    clcd_putch(' ', LINE2(10));
    clcd_putch(read_ext_eeprom(log * 10 + 6), LINE2(11));
    clcd_putch(read_ext_eeprom(log * 10 + 7), LINE2(12));
    clcd_putch(' ', LINE2(13));
    clcd_putch(read_ext_eeprom(log * 10 + 8), LINE2(14));
    clcd_putch(read_ext_eeprom(log * 10 + 9), LINE2(15));
}





