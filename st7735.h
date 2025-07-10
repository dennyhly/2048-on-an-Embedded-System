#ifndef ST7735_H
#define ST7735_H

#include "ST7735_INIT.h"
#include "tiles.h"
#include "gameHelper.h"
#include <string.h>
#include <stdio.h>

void startDisplay();
void blackOutDisplay();
void drawGame();
void drawTile(uint8_t x, uint8_t y, const int* tile);
void drawScoreStatic(uint8_t x, const int* tile);
void drawScoreDynamic(uint8_t x, const int* tile);
const int* getTileArray(int tile);
void clearTile(uint8_t x, uint8_t y);
void updateGame();
const int* scoreHelper(char digit);
void showScore();

void startDisplay() {
    setWindow(0, 129, 0, 128);
    int i = 0;
    int j = 0;
    sendCommand(0x2C); //RAMWR
    for (i = 0; i <= 129; ++i) { //makes the whole thing white
        for (j = 0; j <= 128; ++j) {
                sendData(255);
                sendData(255);
                sendData(255);
        }
    }
}

void blackOutDisplay() {
    setWindow(0, 129, 0, 128);
    int i = 0;
    int j = 0;
    sendCommand(0x2C); //RAMWR
    for (i = 0; i <= 129; ++i) { 
        for (j = 0; j <= 128; ++j) {
                sendData(0);
                sendData(0);
                sendData(0);
        }
    }
}

void drawGame() {
    int j, m = 0;
    setWindow (32, 32, 0, 128);
    sendCommand(0x2C);
    for (j = 0; j < 129; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }

    setWindow (64, 64, 0, 128);
    sendCommand(0x2C);
    for (j = 0; j < 129; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }

    setWindow (96, 96, 0, 128);
    sendCommand(0x2C);
    for (j = 0; j < 129; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }


    setWindow (0, 129, 32, 32);
    sendCommand(0x2C);
    for (j = 0; j < 132; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }

    setWindow (0, 129, 64, 64);
    sendCommand(0x2C);
    for (j = 0; j < 132; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }

    setWindow (0, 129, 96, 96);
    sendCommand(0x2C);
    for (j = 0; j < 132; ++j) {
        for (m = 0; m < 3; ++m) {
            sendData(0);
        }
    }
    
}

void drawTile(uint8_t x, uint8_t y, const int* tile) {
    setWindow(6 + (x*32), 25 + (x*32), 11 + (y*32), 19 + (y*32));

    sendCommand(0x2C);
    int i, byte = 0;
    for (i = 0; i < 180; ++i) {
        byte = pgm_read_byte(&(tile[i]));
        sendData(byte);
        sendData(byte);
        sendData(byte);
    }
}

void drawScoreStatic(uint8_t x, const int* tile) {
    setWindow(10 + (x*6), 15 + (x*6), 59, 67);

    sendCommand(0x2C);
    int i, byte = 0;
    for (i = 0; i < 54; ++i) {
        byte = pgm_read_byte(&(tile[i]));
        sendData(byte);
        sendData(byte);
        sendData(byte);
    }
}

void drawScoreDynamic(uint8_t x, const int* tile) {
    setWindow(46 + (x*6), 51 + (x*6), 59, 67);

    sendCommand(0x2C);
    int i, byte = 0;
    for (i = 0; i < 54; ++i) {
        byte = pgm_read_byte(&(tile[i]));
        sendData(byte);
        sendData(byte);
        sendData(byte);
    }
}

const int* getTileArray(int tile) {
    if (tile == 2) {return tile2;}
    else if (tile == 4) {return tile4;}
    else if (tile == 8) {return tile8;}
    else if (tile == 16) {return tile16;}
    else if (tile == 32) {return tile32;}
    else if (tile == 64) {return tile64;}
    else if (tile == 128) {return tile128;}
    else if (tile == 256) {return tile256;}
    else if (tile == 512) {return tile512;}
    else if (tile == 1024) {return tile1024;}
    else {return tile2048;}
}

void clearTile(uint8_t x, uint8_t y) {
    const int* tile = nullptr;
    setWindow(6 + (x*32), 25 + (x*32), 11 + (y*32), 19 + (y*32)); tile = clear20x9;

    sendCommand(0x2C);
    int i, byte = 0;
    for (i = 0; i < 180; ++i) {
        byte = pgm_read_byte(&(tile[i]));
        sendData(byte);
        sendData(byte);
        sendData(byte);
    }
}

void updateGame() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (slotFilled[i][j] != 0) {
                const int* arrayUsed = getTileArray(slotFilled[i][j]);
                drawTile(j, i, arrayUsed);//due to orientation of st7735
            }
            else if (slotFilled[i][j] == 0) {
                clearTile(j, i);//due to orientation of st7735
            }
        }
    }
}

const int* scoreHelper(char digit) {
    if (digit == '0') {return zero;}
    else if (digit == '1') {return one;}
    else if (digit == '2') {return two;}
    else if (digit == '3') {return three;}
    else if (digit == '4') {return four;}
    else if (digit == '5') {return five;}
    else if (digit == '6') {return six;}
    else if (digit == '7') {return seven;}
    else if (digit == '8') {return eight;}
    else if (digit == '9') {return nine;}
    else if (digit == 's') {return s;}
    else if (digit == 'c') {return c;}
    else if (digit == 'o') {return o;}
    else if (digit == 'r') {return r;}
    else if (digit == 'e') {return e;}
    else {return z;}
}

void showScore() {
    const int* tile = nullptr;
    char scoreWord[] = {'s','c','o','r','e','z','\0'};
    int score = returnScore();
    char manipulateScore[6] = {'0'};
    sprintf(manipulateScore,"%d", score);

    startDisplay();

    for (int i = 0; scoreWord[i] != '\0'; ++i) {
        tile = scoreHelper(scoreWord[i]);
        drawScoreStatic(i, tile);
    }

    for (int i = 0; manipulateScore[i] != '\0'; ++i) {
        tile = scoreHelper(manipulateScore[i]);
        drawScoreDynamic(i, tile);
    }
}

#endif