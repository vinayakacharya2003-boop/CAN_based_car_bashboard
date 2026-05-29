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

