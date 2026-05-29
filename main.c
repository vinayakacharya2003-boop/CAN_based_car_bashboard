/*
 * File:   main.c
 * Author: VINAYAK ACHARYA
 *
 * Created on 26 February, 2026, 6:43 PM
 */





#include <xc.h>
#include "header.h"

char gear[][2] = {"GN", "G1", "G2", "G3", "G4", "G5", "GR", "_C"};

void init_config() {
    init_adc();
    init_clcd();
    init_can();
    init_digital_keypad();
}

void main(void) {

    init_config();

    unsigned int speed = 0;
    unsigned char key;
    unsigned char i = 0;
    unsigned char flag = 0; // 0 = normal, 1 = GA mode
    char speed_data[4];
    uint16_t msg_id;
    uint8_t data[8];
    uint8_t len;
    uint8_t received_index;


    clcd_print("SPEED", LINE1(0));
    clcd_print("GEAR", LINE1(10));

    while (1) {

        /* Integer based ADC conversion */
        speed = ((read_adc(CHANNEL4)) / 10.23);

        /* Convert integer to string */
        speed_data[0] = (speed / 100) + '0';
        speed_data[1] = (speed / 10) % 10 + '0';
        speed_data[2] = speed % 10 + '0';
        speed_data[3] = '\0';

        can_transmit(SPEED_MSG_ID, speed_data, 4);
        __delay_ms(80); 

        key = read_digital_keypad(STATE_CHANGE);

        /* Auto mode */
        if (key == SWITCH3) {
            i = 7; // GA
            flag = 1; // Enable GA mode
        }            /* Gear Up */
        else if (key == SWITCH1) {

            if (flag) { // If in GA mode
                i = 0; // Back to Neutral
                flag = 0;
            } else {
                if (i < 6)
                    i++;
            }
        }            /* Gear Down */
        else if (key == SWITCH2) {

            if (flag) { // If in GA mode
                i = 0;
                flag = 0;
            } else {
                if (i > 0)
                    i--;
            }
        }

        can_transmit(GEAR_MSG_ID, gear[i], 2);
        __delay_ms(80); 
               
    }
}

/*
 * File:   main.c
 * Author: VINAYAK ACHARYA
 *
 * Created on 26 February, 2026, 10:12 PM
 */





#include <xc.h>
#include "header.h"

unsigned int blink_timer = 0;
unsigned char blink_state = 0;
static unsigned char indicator_flag = 0;
static unsigned char flag = 0;

void init_config(void) {
    init_adc();
    init_clcd();
    init_can();
    init_digital_keypad();


}

void clear_indicators(void) {
    clcd_print("  ", LINE2(8));
    clcd_print("  ", LINE2(6));
}

void main(void) {
    unsigned int rpm = 0;
    unsigned char key;
    uint8_t rpm_data[5]; // FIX: uint8_t not char
    uint8_t ind_data[1]; // NEW: indicator CAN payload
    uint16_t msg_id;
    uint8_t data[9];
    uint8_t len;

    init_config();

    clcd_print("RPM", LINE1(0));
    clcd_print("INDICATOR", LINE1(6));

    while (1) {
        /* -------- RPM CALCULATION -------- */
        rpm = ((read_adc(CHANNEL4) / 10.23) * 60);



        rpm_data[0] = (rpm / 1000) % 10 + '0';
        rpm_data[1] = (rpm / 100) % 10 + '0';
        rpm_data[2] = (rpm / 10) % 10 + '0';
        rpm_data[3] = (rpm % 10) + '0';
        rpm_data[4] = '\0';

        can_transmit(RPM_MSG_ID, rpm_data, 5);
        
        __delay_ms(80);

        /* -------- SWITCH READ -------- */
        key = read_digital_keypad(STATE_CHANGE);

        if (key == SWITCH1)
            indicator_flag = 1; // RIGHT
        else if (key == SWITCH2)
            indicator_flag = 2; // HAZARD
        else if (key == SWITCH3)
            indicator_flag = 3; // LEFT
        else if (key == SWITCH4)
            indicator_flag = 0; // OFF

        ind_data[0] = indicator_flag;
        can_transmit(INDICATOR_MSG_ID, ind_data, 2);
       
        __delay_ms(80);

    }
}

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


