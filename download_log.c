
/*
 * File:   
 * Author: <NAME>
 *
 * Created on 1 December, 2023, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "uart.h"
#include "i2c.h"
#include "eeprom.h"

extern unsigned int log_count, main_f, prev_ind, ev_ind;

void download_log() {
    if (ev_ind < 9) // if ev ind < 9 then updating prev index with event index
        prev_ind = ev_ind;
    ev_ind = 10; // updating event ind with download log index
    store_event(); //storing download log event
    puts("# Time     EV SP\n\r"); //displaying in teraterm
    for (unsigned int j = 0; j < log_count; j++) {
        putch(j + '0');
        putch(' ');
        putch(read_ext_eeprom(j * 10 + 0));
        putch(read_ext_eeprom(j * 10 + 1));
        putch(':');
        putch(read_ext_eeprom(j * 10 + 2));
        putch(read_ext_eeprom(j * 10 + 3));
        putch(':');
        putch(read_ext_eeprom(j * 10 + 4));
        putch(read_ext_eeprom(j * 10 + 5));
        putch(' ');
        putch(read_ext_eeprom(j * 10 + 6));
        putch(read_ext_eeprom(j * 10 + 7));
        putch(' ');
        putch(read_ext_eeprom(j * 10 + 8));
        putch(read_ext_eeprom(j * 10 + 9));
        puts("\n\r");
    }
    main_f = 2; //returning to menu
    return;

}
