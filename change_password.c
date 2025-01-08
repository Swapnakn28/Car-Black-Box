
/*
 * File:   
 * Author: <NAME>
 *
 * Created on 1 December, 2023, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "i2c.h"
#include "eeprom.h"
#include "matrix_keypad.h"

extern unsigned int main_f, ev_ind, prev_ind;

void change_pass(char key) {
    unsigned char arr[13]; // 1 1 1 1    0 1 0 1   0 1 0 1\0
    unsigned int count = 0, valid = 0, delay = 0, flag = 0;
    CLEAR_DISP_SCREEN;
    if (ev_ind < 9) // if ev ind < 9 then updating prev index with event index
        prev_ind = ev_ind;
    clcd_print("Enter Old Paswod", LINE1(0));
    while (1) {
        switch_scroll(key);
        if (count < 4) {
            key = read_switches(STATE_CHANGE); // reading switches
            if (delay++ == 5000) //blinking | 
            {
                clcd_putch('|', LINE2(count));
            } else if (delay == 10000) {
                delay = 0;
                clcd_putch(' ', LINE2(count));
            }

            if (key == MK_SW11) // storing 0
            {
                if (valid == 1) //storing new password from 4 to 7
                    arr[count + 4] = '0';
                else if (valid == 2) //storing re enter new password from 8 to 11
                    arr[count + 8] = '0';
                else //storing old password from 0 to 3
                    arr[count] = '0';
                clcd_putch('*', LINE2(count));
                count++;
            } else if (key == MK_SW12) //storing 1
            {
                if (valid == 1) //similarly
                    arr[count + 4] = '1';
                else if (valid == 2)
                    arr[count + 8] = '1';
                else
                    arr[count] = '1';
                clcd_putch('*', LINE2(count));
                count++;
            }
        } else {
            if (valid == 0) // for first time 
            {
                for (unsigned int i = 0; i < 50000; i++);
                flag = 0;
                //checking old password is matched with eeprom value 200-203
                if ((arr[0] != read_ext_eeprom(200)) || (arr[1] != read_ext_eeprom(201)) || (arr[2] != read_ext_eeprom(202)) || (arr[3] != read_ext_eeprom(203))) {
                    flag = 1;
                }
                if (flag == 1) //if not matched returning to menu
                {
                    main_f = 2;
                    CLEAR_DISP_SCREEN;
                    for (unsigned int i = 0; i < 3000; i--);
                    clcd_print("Incorrect password ", LINE1(0)); //printing incorrect msg
                    for (unsigned long i = 0; i < 160000; i++);
                    return;
                }
                // if matched updating valid to 1 again reading new password
                count = 0;
                valid = 1;
                CLEAR_DISP_SCREEN;
                clcd_print("Enter New PW    ", LINE1(0));

            } else {
                if (valid == 2) // for second time
                {
                    for (int i = 4; i < 8; i++) //comparing new password is matching with reentered new password
                    {
                        if (arr[i] != arr[i + 4]) //if not matched displaying msg and returning back to menu
                        {
                            main_f = 2;
                            CLEAR_DISP_SCREEN;
                            for (unsigned int i = 0; i < 3000; i--);
                            clcd_print("Incorrect password ", LINE1(0));
                            for (unsigned long i = 0; i < 160000; i++);
                            return;
                        }
                    }
                    //if matched writing in eeprom
                    write_ext_eeprom(200, arr[4]);
                    write_ext_eeprom(201, arr[5]);
                    write_ext_eeprom(202, arr[6]);
                    write_ext_eeprom(203, arr[7]);
                    clcd_print("Changed password", LINE1(0));
                    clcd_print("  Successfully   ", LINE2(0));
                    for (unsigned long i = 0; i < 160000; i++);
                    main_f = 2;
                    ev_ind = 13;
                    store_event();
                    return;
                } else //if valid == 1
                {
                    valid = 2; //updating valid to 2
                    count = 0;
                    CLEAR_DISP_SCREEN;
                    clcd_print("Re Enter New PW ", LINE1(0)); //reading re enter new password
                }
            }
        }

    }

}