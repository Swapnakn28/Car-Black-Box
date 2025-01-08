
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
extern unsigned char time[9], ev[][3];
extern unsigned int ev_ind, sp, log_count;

void store_event() {
    unsigned char arr[10]; //taking arr for storing event details
    arr[0] = time[0]; //storing time in arr
    arr[1] = time[1];
    arr[2] = time[3];
    arr[3] = time[4];
    arr[4] = time[6];
    arr[5] = time[7];
    arr[6] = ev[ev_ind][0]; //storing event in arr
    arr[7] = ev[ev_ind][1];
    arr[8] = sp / 10 + '0'; //storing speed in arr
    arr[9] = sp % 10 + '0';

    if (log_count > 9) //if event log count is greater than 9 then
    {
        log_count = 0;
        for (int i = 0; i < 9; i++) //swaping  events from bottom to top
        {
            for (int j = 0; j < 10; j++) // 
                write_ext_eeprom(log_count * 10 + j, read_ext_eeprom(log_count * 10 + j + 10)); //ex; storing 10th mem value to 0 
            log_count++; //incrementing log count
        }
    }
    for (int i = 0; i < 10; i++) //updating last event with current event
        write_ext_eeprom(log_count * 10 + i, arr[i]); //writing each char in external eeprom
    log_count++;

}