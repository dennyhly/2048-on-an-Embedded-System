#ifndef ST7735_INIT_H
#define ST7735_INIT_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "spiAVR.h"
#include "helper.h"

//PORTB PIN 0 is AO, PIN 1 is reset, PIN 2 is CS, PIN 3 is SDA, PIN 5 is SCK

void sendCommand(uint8_t command) {
    PORTB = SetBit(PINB, 0, 0);
    SPI_SEND(command);
}

void sendData(uint8_t data) {
    PORTB = SetBit(PINB, 0, 1);
    SPI_SEND(data);
}

void reset() {
    PORTB = SetBit(PINB, 1, 0);
    _delay_ms(200);
    PORTB = SetBit(PINB, 1, 1);
    _delay_ms(200);
}

void ST7735_init() {
    reset();
    sendCommand(0x01);
    _delay_ms(150);
    sendCommand(0x11);
    _delay_ms(200);
    sendCommand(0x3A);
    sendData(0x06);
    _delay_ms(10);
    sendCommand(0x29);
    _delay_ms(200);
}

void setWindow(uint8_t xStart, uint8_t xEnd, uint8_t yStart, uint8_t yEnd) {
    sendCommand(0x2A); //CASET
    sendData(0x00); //Buffer
    sendData(xStart); 
    sendData(0x00);
    sendData(xEnd); 
    
    sendCommand(0x2B); //RASET
    sendData(0x00); //Buffer
    sendData(yStart); 
    sendData(0x00);
    sendData(yEnd); 
}

#endif