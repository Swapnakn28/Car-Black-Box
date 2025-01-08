/*
 * File:   
 * Author: <NAME>
 *
 * Created on 1 December, 2023, 10:40 AM
 */

#include <xc.h>
#include "main.h"
#include "matrix_keypad.h"
#include "clcd.h"
#include "timer0.h"
#include "i2c.h"
#include "eeprom.h"

extern unsigned int main_f, c;

void password(char key) {
    unsigned char arr[5];
    unsigned int count1 = 0, valid1 = 2, flag1 = 0, c_flag = 0, s_flag = 0, delay1 = 0;
    clcd_print(" Enter Password  ", LINE1(0));
    clcd_print("                ", LINE2(0));
    while (1) {
        switch_scroll(key); // updating event if sw 1 or 2 or 3 pressed
        if (count1 < 4) // reading password from user for 4 times
        {
            key = read_switches(STATE_CHANGE); //reading switch
            if (delay1++ == 10) //blinking | 
            {
                clcd_putch('|', LINE2(count1));
            } else if (delay1 == 50) {
                delay1 = 0;
                clcd_putch(' ', LINE2(count1));
            }

            if (s_flag == 0) //at starting of password only oning timers set c = 5
            {
                s_flag = 1; //updating s_flag to 1
                TMR0ON = 1;
                c = 5;
            }
            if (c == 0) {
                main_f = 0; //if any switch is not pressed in these 5 secs returning to dashboard
                TMR0ON = 0;
                return;
            }
            if (key == MK_SW11) // mksw11 pressed storing 0 
            {
                arr[count1] = '0';
                clcd_putch('*', LINE2(count1));
                count1++; //incrementing count1
                c = 5; //updating c to 5 againg timer will decrement from 5
            } else if (key == MK_SW12) //mksw12 - store 1 
            {
                arr[count1] = '1';
                clcd_putch('*', LINE2(count1));
                count1++; //incrementing count1
                c = 5; //updating c to 5
            }
        } else //after reading 4 times
        {
            flag1 = 0; //check our entered 4 input is matched with data present in 200 - 203 mem of external eeprom
            if ((arr[0] != read_ext_eeprom(200)) || (arr[1] != read_ext_eeprom(201)) || (arr[2] != read_ext_eeprom(202)) || (arr[3] != read_ext_eeprom(203))) {
                flag1 = 1; //if not matched updating flag1
            }
            if (flag1 == 1) //not matched
            {
                if (valid1 != 0) //if valid attempt displays remaining attemps again ask for password
                {
                    for (unsigned int i = 0; i < 50000; i++);
                    count1 = 0;
                    valid1 = valid1 - 1; //decrementing valid attemps 
                    clcd_putch('0' + (valid1 + 1), LINE2(0)); //displaying no of attemps left in clcd
                    clcd_print(" attempts left", LINE2(1));
                    for (unsigned long int i = 0; i < 160000; i++);
                    clcd_print("                ", LINE2(0));
                }
                else //if attemps over
                {
                    if (c_flag == 0) // c will updating to 120 
                    {
                        TMR0ON = 1;
                        c = 120;
                        c_flag = 1;
                    }
                    //printing you are blocked for 120 secs after 120 secs again it will ask for password
                    clcd_print("YOU ARE BLOCKED ", LINE1(0));
                    clcd_print(" FOR  ", LINE2(0));
                    clcd_putch('0' + (c / 100), LINE2(6));
                    clcd_putch('0' + ((c / 10) % 10), LINE2(7));
                    clcd_putch('0' + (c % 10), LINE2(8));
                    clcd_print("  SECS   ", LINE2(9));
                    if (c == 0) //after decrementing 120 - 0. 
                    {
                        valid1 = 2; //againg updating valid attemps 
                        c_flag = 0;
                        clcd_print(" Enter Password  ", LINE1(0));
                        clcd_print("                ", LINE2(0));
                        s_flag = 0;
                        TMR0ON = 0; //timer of 
                        count1 = 0;
                    }
                }
            } else //if password match returning to menu
            {
                main_f = 2;
                count1 = 0, valid1 = 2, flag1 = 0, c_flag = 0, s_flag = 0, delay1 = 0;
                return;
            }
        }
    }
}