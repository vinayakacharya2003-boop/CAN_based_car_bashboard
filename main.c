/*
 * File:   main.c
 * Author: VINAYAK ACHARYA
 *
 * Created on 6 March, 2026, 8:17 AM
 */




#include <xc.h>
#include "header.h"

void init_config() {
    init_clcd();
    init_can();
    init_timer0();
    PEIE = 1;
    GIE = 1;
    TRISB0 = 0;
    TRISB7 = 0;
}

void main(void) {

    init_config();
    clcd_print("SPD", LINE1(0));
    clcd_print("RPM", LINE1(4));
    clcd_print("GR", LINE1(9));
    clcd_print("IND", LINE1(12));

    while (1) {
        process_can_bus();       
    }
}


