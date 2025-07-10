#include "timerISR.h"
//#include "helper.h"
//#include "periph.h"
#include "irAVR.h"
#include "spiAVR.h"
#include "ST7735_INIT.h"
#include "st7735.h"
#include "gameHelper.h"
#include <string.h>

#define NUM_TASKS 2

decode_results signalIR;
unsigned long powerSignal = 0xFFA25D;
unsigned long pauseSignal = 0xFF02FD;
int moveDirection = 4;

typedef struct _task{//Task struct for concurrent synchSMs implmentations
	signed 	 char state; 		//Task's current state
	unsigned long period; 		//Task period
	unsigned long elapsedTime; 	//Time elapsed since last task tick
	int (*TickFct)(int); 		//Task tick function
} task;


const unsigned long GCD_PERIOD = 1;
const unsigned long SYSTEM_STATES_PERIOD = 1;
const unsigned long GAME_STATES_PERIOD = 200;

task tasks[NUM_TASKS]; 

enum SYSStates {SYS_Off, SYS_Pause, SYS_On} SYSState;
enum gameStates {game_Lose, game_Wait, game_Move, game_Score, game_Lose_Win_Screen} gameState;

int TickFct_System_State(int states);
int TickFct_Game_State(int states);

void TimerISR() {
	for ( unsigned int i = 0; i < NUM_TASKS; i++ ) {                   // Iterate through each task in the task array
		if ( tasks[i].elapsedTime == tasks[i].period ) {           // Check if the task is ready to tick
			tasks[i].state = tasks[i].TickFct(tasks[i].state); // Tick and set the next state for this task
			tasks[i].elapsedTime = 0;                          // Reset the elapsed time for the next tick
		}
		tasks[i].elapsedTime += GCD_PERIOD;                        // Increment the elapsed time by GCD_PERIOD
	}
}

int main(void) {
    DDRB = 0b10000111; PORTB = 0b01111000;
    DDRC = 0b00000000; PORTC = 0b11111111;
    DDRD = 0b11111111; PORTD = 0b00000000;

    ADC_init();
    IRinit(&DDRD, &PIND, 7);
    SPI_INIT();
    ST7735_init();
    srand(time(nullptr));

    unsigned int i = 0;
    tasks[i].state = SYS_Off;
    tasks[i].period = SYSTEM_STATES_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_System_State;
    ++i;
    tasks[i].state = game_Lose;
    tasks[i].period = GAME_STATES_PERIOD;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &TickFct_Game_State;

    TimerSet(GCD_PERIOD);
    TimerOn();

    while (1) {}

    return 0;
}

//Right before sending an 8 bit value with the SPI_SEND() funtion, you will need to set your SS pin to low.

int TickFct_System_State(int states) {
    switch (SYSState) {//transition state
        default: SYSState = SYS_Off; blackOutDisplay(); break;
        case SYS_Off: 
            if (IRdecode(&signalIR)) {
                if (signalIR.value == powerSignal) {SYSState = SYS_On; startDisplay(); drawGame(); updateGame();}
                else {SYSState = SYS_Off;}
                IRresume();
            }
            break;

        case SYS_Pause:
            if (IRdecode(&signalIR)) {
                if (signalIR.value == powerSignal) {SYSState = SYS_Off; sendCommand(0x28); blackOutDisplay();}
                else if (signalIR.value == pauseSignal) {SYSState = SYS_On; sendCommand(0x29);}
                else {SYSState = SYS_Pause;}
                IRresume();
            }
            break;

        case SYS_On:
            if (IRdecode(&signalIR)) {
                if (signalIR.value == powerSignal) {SYSState = SYS_Off; blackOutDisplay(); resetSlotFilled();}
                else if (signalIR.value == pauseSignal) {SYSState = SYS_Pause; sendCommand(0x28);}
                else {SYSState = SYS_On;}
                IRresume();
            }
            break;
    }

    switch (SYSState) {//action state
        default: break;
        case SYS_Off: PORTD = SetBit(PORTD, 4, 0); PORTD = SetBit(PORTD, 3, 0); break;
        case SYS_Pause: PORTD = SetBit(PORTD, 4, 1); PORTD = SetBit(PORTD, 3, 0); break;
        case SYS_On: PORTD = SetBit(PORTD, 4, 0); PORTD = SetBit(PORTD, 3, 1); break;
    }
    return states;
}

int TickFct_Game_State(int states) {
    switch(gameState) {
        default: gameState = game_Lose; break;

        case game_Lose:
            startDisplay();
            drawGame();
            gameState = game_Wait;
            break;

        case game_Wait: 
            if (!checkValidMove() || checkWin()) {gameState = game_Lose_Win_Screen; resetSlotFilled();} 
            else if (!GetBit(PINC, 2)) {
                gameState = game_Score; 
                showScore();
            }
            else if (getDirections() != 4 && checkValidMove()) {
                gameState = game_Move; 
                moveDirection = getDirections(); 
                moveBlocks(moveDirection); 
                placeRandomTile();
            } 
            else {gameState = game_Wait;}
        break;

        case game_Move: 
            if (getDirections() == 4) {gameState = game_Wait; moveDirection = 4; updateGame();} 
            else {gameState = game_Move;}
        break;

        case game_Score:
            if (GetBit(PINC, 2)) {gameState = game_Score;}
            else if (!GetBit(PINC, 2)) {gameState = game_Wait; startDisplay(); drawGame(); updateGame();}
        break;
        
        case game_Lose_Win_Screen:
            if (GetBit(PINC, 2)) {gameState = game_Lose_Win_Screen; showScore();}
            else {gameState = game_Lose;}
        break;
    }

    switch(gameState) {
        default: break;
        case game_Wait: break;
        case game_Move: break;
        case game_Lose: break;
        case game_Score: break;
        case game_Lose_Win_Screen: break; 
    }
    return states;
}