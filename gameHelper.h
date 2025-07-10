#ifndef GAMEHELPER_H
#define GAMEHELPER_H

#include "periph.h"
#include "tiles.h"
#include "st7735.h"
#include "time.h"

int score = 0;

int slotFilled[4][4] = {
    {0, 0, 0, 0}, //i
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
    //j
};

bool merged[4][4] = {
    {0, 0, 0, 0}, //i
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
    //j
};

void resetMerged();
void resetSlotFilled();
bool checkValidMove();
bool checkWin();
void placeRandomTile();
bool detectTile(int slotX, int slotY);
bool sameNumber(int firstX, int firstY, int secondX, int secondY);
void sendNumbers(int firstX, int firstY, int secondX, int secondY);
void combineNumbers(int firstX, int firstY, int secondX, int secondY);
int returnScore();
void resetScore();
void moveUp();
void moveRight();
void moveLeft();
void moveDown();
void moveBlocks(int direction);


void resetMerged() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            merged[i][j] = 0;
        }
    }
}

void resetSlotFilled() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            slotFilled[i][j] = 0;
        }
    }
};

bool checkValidMove() {//check for handling when box is not full but stops
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (slotFilled[i][j] == 0) {
                return true;
            }

            if ((i > 0 && sameNumber(i, j, i - 1, j)) || (i < 3 && sameNumber(i, j, i + 1, j)) ||
                (j > 0 && sameNumber(i, j, i, j - 1)) || (j < 3 && sameNumber(i, j, i, j + 1))) {
                return true;
            }
        }
    }
return false;
};

bool checkWin() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (slotFilled[i][j] == 2048) {return true;}
        }
    }
    return false;
}

void placeRandomTile() {
    if (!checkValidMove()) {return;}

    int selectTile = (rand() % 10);
    int tile = (selectTile < 9) ? 2 : 4;
    int startCol = rand() % 4;
    int startRow = rand() % 4;
    int row, col = 0;


    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            row = (startRow + i) % 4;
            col = (startCol + j) % 4;
            if (slotFilled[row][col] == 0) {slotFilled[row][col] = tile; return;}
        }
    }
};

bool detectTile(int slotX, int slotY) {
    if (slotFilled[slotX][slotY] != 0) {return true;}
    return false;
}

bool sameNumber(int firstX, int firstY, int secondX, int secondY) {
if (slotFilled[firstX][firstY] == slotFilled[secondX][secondY]) {return true;}
    return false;
}

void sendNumbers(int firstX, int firstY, int secondX, int secondY) {
    slotFilled[secondX][secondY] = slotFilled[firstX][firstY];
    slotFilled[firstX][firstY] = 0;
}

void combineNumbers(int firstX, int firstY, int secondX, int secondY) {
    int firstSlot = slotFilled[firstX][firstY];
    int secondSlot = slotFilled[secondX][secondY];
    slotFilled[secondX][secondY] = firstSlot + secondSlot;
    slotFilled[firstX][firstY] = 0;
    score += slotFilled[secondX][secondY];
}

int returnScore() {
    return score;
}

void resetScore() {
    score = 0;
}

void moveUp() {
    resetMerged();
    int i, j, k = 0;
    for(j = 0; j < 4; ++j) {
        for (i = 1; i < 4; ++i) {
            if (slotFilled[i][j] != 0) {
                k = i;
                while (k-1 >= 0) {
                    if (!detectTile(k-1, j)) {sendNumbers(k, j, k-1, j);}
                    else if (detectTile(k-1, j) && sameNumber(k, j, k-1, j) && !merged[k-1][j]) {
                        combineNumbers(k, j, k-1, j); 
                    }
                    --k;
                }
            }
        }
    }
}

void moveRight() {
    resetMerged();
    int i, j, k = 0;
    for(i = 0; i < 4; ++i) {
        for (j = 2; j >= 0; --j) {
            if (slotFilled[i][j] != 0) {
                k = j;
                while (k+1 < 4) {
                    if (!detectTile(i, k+1)) {sendNumbers(i, k, i, k+1);}
                    else if (detectTile(i, k+1) && sameNumber(i, k, i, k+1) && !merged[i][k+1]) {
                        combineNumbers(i, k, i, k+1);
                    }
                    ++k;
                }
            }
        }
    }
}

void moveLeft() {
    resetMerged();
    int i, j, k = 0;
    for(i = 0; i < 4; ++i) {
        for (j = 1; j < 4; ++j) {
            if (slotFilled[i][j] != 0) {
                k = j;
                while (k-1 >= 0) {
                    if (!detectTile(i, k-1)) {sendNumbers(i, k, i, k-1);}
                    else if (detectTile(i, k-1) && sameNumber(i, k, i, k-1) && !merged[i][k-1]) {
                        combineNumbers(i, k, i, k-1); 
                        merged[i][k-1] = 1;
                    }
                    --k;
                }
            }
        }
    }
}

void moveDown() {
    resetMerged();
    int i, j, k = 0;
    for(j = 0; j < 4; ++j) {
        for (i = 2; i >= 0; --i) {
            if (slotFilled[i][j] != 0) {
                k = i;
                while (k+1 < 4) {
                    if (!detectTile(k+1, j)) {sendNumbers(k, j, k+1, j);}
                    else if (detectTile(k+1, j) && sameNumber(k, j, k+1, j) && !merged[k+1][j]) {
                        combineNumbers(k, j, k+1, j);
                    }
                    ++k;
                }
            }
        }
    }
}

void moveBlocks(int direction) {
    if (direction == 0) {moveUp();}
    else if (direction == 1) {moveDown();}
    else if (direction == 2) {moveLeft();}
    else if (direction == 3) {moveRight();}
}

#endif