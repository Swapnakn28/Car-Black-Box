
#include <xc.h>
#include "main.h"
#include "clcd.h"
extern unsigned int log_count, main_f, ev_ind, prev_ind;

void clear_log(char key) {

    if (ev_ind < 9) // if ev ind < 9 then updating prev index with event index
        prev_ind = ev_ind;
    clcd_print("  Cleared Log  ", LINE1(0)); //printing cleared log msg on clcd
    clcd_print("  Successfully ", LINE2(0));
    for (unsigned long i = 0; i < 160000; i++);
    log_count = 0; // making log count as zero
    main_f = 2; // go to menu
    ev_ind = 11;
    store_event(); //storing cleared log event

}