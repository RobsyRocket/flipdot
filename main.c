#ifndef F_CPU
#define F_CPU       16000000UL
#endif

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "main.h"

#define HIGH            1
#define LOW             0
#define PULSE_LENGTH    2

// Pin mapping for Flip-Dot matrix row control transistors
row_t row = {
    { // set row [0..12]
        {&DDRA,&PORTA,PA0}, // Arduino Pin 22
        {&DDRA,&PORTA,PA2}, // Arduino Pin 24
        {&DDRA,&PORTA,PA4}, // Arduino Pin 26
        {&DDRA,&PORTA,PA6}, // Arduino Pin 28
        {&DDRC,&PORTC,PC7}, // Arduino Pin 30
        {&DDRC,&PORTC,PC5}, // Arduino Pin 32
        {&DDRC,&PORTC,PC3}, // Arduino Pin 34
        {&DDRC,&PORTC,PC1}, // Arduino Pin 36
        {&DDRD,&PORTD,PD7}, // Arduino Pin 38
        {&DDRG,&PORTG,PG1}, // Arduino Pin 40
        {&DDRL,&PORTL,PL7}, // Arduino Pin 42
        {&DDRL,&PORTL,PL5}, // Arduino Pin 44
        {&DDRL,&PORTL,PL3}  // Arduino Pin 46
    },
    { // clear row [0..12]
        {&DDRA,&PORTA,PA1}, // Arduino Pin 23
        {&DDRA,&PORTA,PA3}, // Arduino Pin 25
        {&DDRA,&PORTA,PA5}, // Arduino Pin 27
        {&DDRA,&PORTA,PA7}, // Arduino Pin 29
        {&DDRC,&PORTC,PC6}, // Arduino Pin 31
        {&DDRC,&PORTC,PC4}, // Arduino Pin 33
        {&DDRC,&PORTC,PC2}, // Arduino Pin 35
        {&DDRC,&PORTC,PC0}, // Arduino Pin 37
        {&DDRG,&PORTG,PG2}, // Arduino Pin 39
        {&DDRG,&PORTG,PG0}, // Arduino Pin 41
        {&DDRL,&PORTL,PL6}, // Arduino Pin 43
        {&DDRL,&PORTL,PL4}, // Arduino Pin 45
        {&DDRL,&PORTL,PL2}  // Arduino Pin 47
    }
};

//Pin mapping to FP2800 for column addressing
port_t col[5] = {
        {&DDRL,&PORTL,PL1}, // Arduino Pin 48
        {&DDRL,&PORTL,PL0}, // Arduino Pin 49
        {&DDRB,&PORTB,PB3}, // Arduino Pin 50
        {&DDRB,&PORTB,PB2}, // Arduino Pin 51
        {&DDRB,&PORTB,PB1}  // Arduino Pin 52
};

//Pin mapping to FP2800 for column output level
port_t data = {
         &DDRB,&PORTB,PB0   // Arduino Pin 53   
};

//Pin mapping to FP2800 for column enable pulse
port_t enable = {
         &DDRK,&PORTK,PK7   // Arduino Pin A15   
};

void set_row(uint8_t n, uint8_t val) {
    if(val == LOW) {
        *row.set[n].port   &= ~(1<<row.set[n].bit); 
    } else { 
        *row.clear[n].port &= ~(1<<row.clear[n].bit); 
        *row.set[n].port   |=  (1<<row.set[n].bit); 
    }
}

void clear_row(uint8_t n, uint8_t val) {
    if(val == LOW) {
        *row.clear[n].port &= ~(1<<row.clear[n].bit); 
    } else { 
        *row.set[n].port   &= ~(1<<row.set[n].bit); 
        *row.clear[n].port |=  (1<<row.clear[n].bit); 
    }
}

void set_rows(uint16_t data) {
    for(uint8_t i=0; i<13; i++) {
        set_row(i,(data & (1<<i)));
    }
}

void clear_rows(uint16_t data) {
    for(uint8_t i=0; i<13; i++) {
        clear_row(i,(data & (1<<i)));
    }
}

void select_col(uint8_t n) {
    for(uint8_t i=0; i<5; i++) {
        if(n & (1<<i)) {
            *col[i].port |=  (1<<col[i].bit);
        } else {
            *col[i].port &= ~(1<<col[i].bit);
        }
    }
}

void set_col(uint8_t n) {
    select_col(n);
    *data.port   |=  (1<<data.bit);
    *enable.port |=  (1<<enable.bit);
    _delay_ms(PULSE_LENGTH);    
    *enable.port &= ~(1<<enable.bit);
}

void clear_col(uint8_t n) {
    select_col(n);
    *data.port   &= ~(1<<data.bit);
    *enable.port |=  (1<<enable.bit);
    _delay_ms(PULSE_LENGTH);    
    *enable.port &= ~(1<<enable.bit);
}

int main(void) {

    // Main Loop
    while(1) {
        set_rows(0x1803); // 0b01100000000011
        set_col(0);        
    }
    return 0;
}

