

/*
 * File:   
 * Author: <NAME>
 *
 * Created on 1 December, 2023, 10:40 AM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"

extern char time[9], ev[][3];
extern unsigned int sp, ev_ind;

void dashboard() {
    //displaying current event details
    clcd_print("  TIME    EV  SP", LINE1(0));
    clcd_print(time, LINE2(0));
    clcd_print("  ", LINE2(8)); //time
    clcd_print(ev[ev_ind], LINE2(10)); //event
    clcd_print("  ", LINE2(12));
    clcd_putch('0' + (sp / 10), LINE2(14)); //speed
    clcd_putch('0' + (sp % 10), LINE2(15));
}


