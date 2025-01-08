
#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"

extern unsigned char time[9]; //hh:mm:ss
extern unsigned int main_f, ev_ind, prev_ind;

void settime(char key) {
    CLEAR_DISP_SCREEN;
    if (ev_ind < 9)
        prev_ind = ev_ind;
    unsigned int arr[9]; //12:30:15
    for (int i = 0; i < 8; i++) // storing time values as integer
        arr[i] = time[i] - 48;
    unsigned int delay = 0, hh = ((arr[0] * 10) + arr[1]), mm = ((arr[3] * 10) + arr[4]), ss = ((arr[6] * 10) + arr[7]), lp11 = 0, lp12 = 0, mode_flag = 0, field_flag = 0;
    // field_flag = 0- sec  ;; 1- min ;; 2 - hours
    while (1) {
        key = read_switches(1); // avoiding long press
        switch_scroll(key); // if sw1,2,3 pressed ..event will updated
        
        clcd_print("    Set Time    ", LINE1(0));
        key = read_switches(LEVEL_CHANGE); //reading switch

        if (key == MK_SW12) //selecting field
        {
            lp12++;
            if (lp12 > 300) //long press returning to menu
            {
                main_f = 2;
                lp12 = 0;
                return;
            }
        } else if (lp12 < 300 && lp12 != 0) // sw12 short press changing fields (ss,mm,hh))
        {
            lp12 = 0;
            if (field_flag == 2) //after hh if switch pressed means it will go to sec
                field_flag = 0;
            else
                ++field_flag;
            delay = 0;
        }
        if (field_flag == 0) //ss mode
        {
            if (key == MK_SW11) //sw11 pressed means incrementing ss
            {
                lp11++;
                if (lp11 > 300) //long press - store event, save time , menu
                {
                    hh = ((hh / 10) << 4) | (hh % 10); // RTC will store in BCD , that's why we are converting into BCD
                    mm = ((mm / 10) << 4) | (mm % 10);
                    ss = ((ss / 10) << 4) | (ss % 10);
                    write_ds1307(HOUR_ADDR, hh); //writing time to the RTC
                    write_ds1307(MIN_ADDR, mm);
                    write_ds1307(SEC_ADDR, ss);

                    main_f = 2; //updating main flag to 2 return to menu
                    ev_ind = 12;
                    store_event(); //storing event
                    lp11 = 0;
                    return;
                }
            } else if (lp11 < 300 && lp11 != 0) //short press 
            {
                ss++, lp11 = 0; //incrementing secs
                if (ss > 59) //if exceeds updating to 0
                {
                    ss = 0;
                }
            }
            if (delay++ == 100) //blinking sec field
            {
                clcd_putch((ss / 10) + '0', LINE2(12));
                clcd_putch((ss % 10) + '0', LINE2(13));
            } else if (delay == 200) {
                delay = 0;
                clcd_putch(' ', LINE2(12));
                clcd_putch(' ', LINE2(13));
            }
            clcd_putch((hh / 10) + '0', LINE2(2)); //displaying hh and mm
            clcd_putch((hh % 10) + '0', LINE2(3));
            clcd_putch((mm / 10) + '0', LINE2(7));
            clcd_putch((mm % 10) + '0', LINE2(8));
        } else if (field_flag == 1) //mm mode
        {
            if (key == MK_SW11) //sw11 pressed means incrementing mm 
            {
                lp11++;
                if (lp11 > 300) //long press - store event, save time , menu
                {
                    hh = ((hh / 10) << 4) | (hh % 10); //same like secs
                    mm = ((mm / 10) << 4) | (mm % 10);
                    ss = ((ss / 10) << 4) | (ss % 10);
                    write_ds1307(HOUR_ADDR, hh);
                    write_ds1307(MIN_ADDR, mm);
                    write_ds1307(SEC_ADDR, ss);
                    main_f = 2;
                    ev_ind = 12;
                    store_event();
                    lp11 = 0;
                    return;
                }
            }
            else if (lp11 < 300 && lp11 != 0) {
                mm++, lp11 = 0;
                if (mm > 59) //if exceeds updating to 0
                {
                    mm = 0;
                }
            }
            if (delay++ == 100) {
                clcd_putch((mm / 10) + '0', LINE2(7));
                clcd_putch((mm % 10) + '0', LINE2(8));
            } else if (delay == 200) {
                delay = 0;
                clcd_putch(' ', LINE2(7));
                clcd_putch(' ', LINE2(8));
            }

            clcd_putch((hh / 10) + '0', LINE2(2));
            clcd_putch((hh % 10) + '0', LINE2(3));
            clcd_putch((ss / 10) + '0', LINE2(12));
            clcd_putch((ss % 10) + '0', LINE2(13));
        } else if (field_flag == 2) //hh mode
        {
            if (key == MK_SW11) //sw11 pressed means incrementing hh
            {
                lp11++;
                if (lp11 > 300) //long press - store event, save time , menu
                {
                    hh = ((hh / 10) << 4) | (hh % 10);
                    mm = ((mm / 10) << 4) | (mm % 10);
                    ss = ((ss / 10) << 4) | (ss % 10);
                    write_ds1307(HOUR_ADDR, hh);
                    write_ds1307(MIN_ADDR, mm);
                    write_ds1307(SEC_ADDR, ss);
                    main_f = 2;
                    ev_ind = 12;
                    lp11 = 0;
                    store_event();
                    return;
                }
            }
            else if (lp11 < 300 && lp11 != 0) {
                hh++, lp11 = 0;
                if (hh > 23) //if exceeds updating to 0
                {
                    hh = 0;
                }
            }
            clcd_putch((mm / 10) + '0', LINE2(7));
            clcd_putch((mm % 10) + '0', LINE2(8));
            clcd_putch((ss / 10) + '0', LINE2(12));
            clcd_putch((ss % 10) + '0', LINE2(13));

            if (delay++ == 100) {
                clcd_putch((hh / 10) + '0', LINE2(2));
                clcd_putch((hh % 10) + '0', LINE2(3));
            } else if (delay == 200) {
                delay = 0;
                clcd_putch(' ', LINE2(2));
                clcd_putch(' ', LINE2(3));
            }
        }
        clcd_print("  ", LINE2(0));
        clcd_print(" : ", LINE2(4));
        clcd_print(" : ", LINE2(9));
        clcd_print("  ", LINE2(14));
    }
}