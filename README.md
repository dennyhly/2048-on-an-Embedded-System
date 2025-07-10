# 2048-on-an-Embedded-System
Microcontroller-based implementation of the classic puzzle game using C, ST7735 display, joystick input, and IR remote control.

Overview
This project implements a fully functional version of the game 2048 on an Elegoo Uno R3 (ATmega328P) microcontroller. The game logic, UI rendering, and input handling are written in C, with output rendered to a ST7735 LCD. The system uses a joystick module for directional input and an IR remote for system control. Designed to demonstrate embedded systems programming, real-time input handling, and state machine design.

Features
Joystick-based Input - Real-time directional control using analog joystick with press-to-select functionality.

Real-time LCD Display - Visualizing the 4×4 game grid and animated block movements using ST7735. Score is also shown on demand with by clicking the joystick.

IR Remote Integration - Supports pause/resume and power toggle using an IR remote interface.

Technical Highlights
Language: C (AVR-GCC)
Platform: Elegoo Uno R3 (ATmega328P)
State Management: Event-driven finite state machines (FSMs) using synchronous task scheduling
Memory Optimization: Fonts and static arrays stored in flash memory using PROGMEM
Randomization: Simulated randomness for tile spawning via seeded rand() (limited by MCU constraints)

Components
Microcontroller: Elegoo Uno R3
Display: ST7735 LCD
User Input: Joystick Module and IR Remote + Receiver

Wiring Diagram  
<img height="500" src="https://github.com/dennyhly/2048-on-an-Embedded-System/blob/main/images/wiringDiagram.png"/>
<img height="500" src="https://github.com/dennyhly/2048-on-an-Embedded-System/blob/main/images/wires1.png"/>
<img height="500" src="https://github.com/dennyhly/2048-on-an-Embedded-System/blob/main/images/wires2.png"/>

User Guide  
The player interacts with a joystick to control the game. The joystick should have the side with the wires pointing out as their north or up direction. As such with the wires being north/up the opposing side would be south or the down direction, East would be the right direction, and West would be the left direction. There are no sound mechanisms of the project, however there are visual outputs. The player moving the blocks, blocks combining, and random blocks spawning are all displayed visually through the ST7735 display. Additionally, the player can turn on, turn off, and pause/resume the game using the IR remote. The off function would print a black screen to the ST7735, the on function would print the game, and the pause/resume function turns the LCD backlight off and back on. Pressing down on the joystick button also shows the score of the game. The score is displayed until another click or movement key is pressed. When the game is won or lost, the screen shows the final score until the joystick button is clicked and a new game is started.
