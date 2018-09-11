/*
 * Calculator.h
 *
 *  Created on: 16Apr.,2018
 *      Author: Geoffrey
 */

#include "Ass-02.h"
#include "StringMathEvaluation.h"

#ifndef CALCULATOR_H
#define CALCULATOR_H

#define CALC_DISP_R1_X 5
#define CALC_DISP_R1_Y 15
#define CALC_DISP_R2_X 5
#define CALC_DISP_R2_Y 55
#define CALC_DISP_R1_HEIGHT 15
#define CALC_DISP_R2_HEIGHT 15
#define CALC_DISP_R1_MAX_CHARS 27
#define CALC_DISP_R2_MAX_CHARS 27
#define CALC_CONT_X 5
#define CALC_CONT_Y 0
#define CALC_CONT_HEIGHT 15

#define NUM_GRID_BUTTONS 24
#define GRID_Y_START 80
#define GRID_BUT_WIDTH 53
#define GRID_BUT_HEIGHT 40
#define GRID_HEIGHT 4
#define GRID_WIDTH 6

#define CALC_MAX_INPUT_SIZE 128

#define CALC_TOUCH_WAIT_TIME 10
#define CALC_TOUCH_RECOVER_TIME 100

//Represents the location of the button in the grid
typedef enum {

	OFF_GRID = -1,
	r1c1,
	r1c2,
	r1c3,
	r1c4,
	r1c5,
	r1c6,
	r2c1,
	r2c2,
	r2c3,
	r2c4,
	r2c5,
	r2c6,
	r3c1,
	r3c2,
	r3c3,
	r3c4,
	r3c5,
	r3c6,
	r4c1,
	r4c2,
	r4c3,
	r4c4,
	r4c5,
	r4c6,

} GridPos;

//Represents button color theme
typedef enum CALC_BUTTON_COLOR {

	white_black_txt=0,
	grey_blue_txt,
	blue_white_txt

} CALC_BUTTON_COLOR;

typedef enum TP_STATE {
	NO_TOUCH=0,
	TOUCH_WAIT,
	TOUCH_RECOVER
} TP_STATE;

//Represents a button
typedef struct GridButton {

	uint16_t x, y;
	char* text;
	CALC_BUTTON_COLOR color;

} GridButton;

//Represents a grid of buttons
typedef struct ButtonGrid {

    GridButton buttons[NUM_GRID_BUTTONS];

} ButtonGrid;


//FUNCTIONS
//User Functions
extern void updateCalculatorDisplay(void);
extern void processTouchGrid(ButtonGrid* grid);
extern void drawGrid(ButtonGrid buttonGrid);
extern void initGrid(ButtonGrid* buttonGrid);

//Internal Functions
void drawGridButton(GridButton button);
GridPos coordToGridPos(uint16_t x, uint16_t y);
void equalPress(void);
void resultToDispStr(double result, char*output, int buffSize);
void setContIndicator(int on);
void inputButtonPress(GridButton button);
void backButtonPress();
void clearButtonPress();
void handleScreenTouch(uint16_t x, uint16_t y, ButtonGrid butGrid) ;

#endif /* CALCULATOR_H */
