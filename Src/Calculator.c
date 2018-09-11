/*
 * Calculator.c
 *
 *  Created on: 16Apr.,2018
 *      Author: Geoffrey Parry-Grass
 *
 *      Implementation of functions to run the calculator.
 *
 */

#include "Calculator.h"

char inputString[CALC_MAX_INPUT_SIZE] = ""; //Stores the current input of the calculator
uint8_t numChars=0; //Stores the number of chars in the input string
uint8_t prevNumChars=0; //Used to detect a change in the input string
double ans=0; //The last stored answer of the calculator

/*
 * Function is to be called when the equal button is pressed
 * If the input string is mathematically valid then the function will
 * store the result in ans and display the result in the main calculator display area
 */
void equalPress(void) {

    //First check that the input is valid and is not empty
	if (strMathValid(inputString) && numChars > 0) {

		//Converts the input string to a double result
		//evalString is implemented in StringMathEvaluation.c
		double result = evalString(inputString, ans);

		//Converts the result to a string which can be displayed on screen
		resultToDispStr(result, inputString, CALC_DISP_R1_MAX_CHARS);

		//Resets the input string to first position
		numChars=0;

		//Clear the primary and secondary calculator display areas
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

		BSP_LCD_FillRect(CALC_DISP_R1_X, CALC_DISP_R1_Y, BSP_LCD_GetXSize()-CALC_DISP_R1_X, CALC_DISP_R1_HEIGHT);
		BSP_LCD_FillRect(CALC_DISP_R2_X, CALC_DISP_R2_Y, BSP_LCD_GetXSize()-CALC_DISP_R2_X, CALC_DISP_R2_HEIGHT);
		//

		//Write the result to the primary display area
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(CALC_DISP_R1_X, CALC_DISP_R1_Y, (uint8_t*)inputString, LEFT_MODE);
		//

		ans = result; //store the result in the answer memory

		//If the input was not valid
	} else if (!strMathValid(inputString)) {

		//Clear primary and secondary display areas
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

		BSP_LCD_FillRect(CALC_DISP_R1_X, CALC_DISP_R1_Y, BSP_LCD_GetXSize()-CALC_DISP_R1_X, CALC_DISP_R1_HEIGHT);
		BSP_LCD_FillRect(CALC_DISP_R2_X, CALC_DISP_R2_Y, BSP_LCD_GetXSize()-CALC_DISP_R2_X, CALC_DISP_R2_HEIGHT);
		//


		//Display "Syntax Error" in primary display area
		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
		BSP_LCD_DisplayStringAt(CALC_DISP_R1_X, CALC_DISP_R1_Y, (uint8_t*)"Syntax Error", LEFT_MODE);
	}

}

/*
 * Function is to be called when an input button is pressed (numbers and operators)
 * The function takes a GridButton corresponding to the pressed button and
 * will add the input to the input string
 */
void inputButtonPress(GridButton button) {

	//Check if there is room for more input
	if (numChars+1 < CALC_MAX_INPUT_SIZE) {
		//insert input symbol of button into input string, move to next position and NULL terminate
		inputString[numChars] = button.text[0];
		numChars++;
		inputString[numChars] = '\0';
	}

}

/*
 * Function is to be called when the back button is pressed
 * Function will reduce the position in the input string by 1 and delete the last input
 */
void backButtonPress() {

	//If there are chars to delete
	if (numChars>0) {
		numChars--; //Go back a position (which stores last input char)
		inputString[numChars] = '\0'; //NULL terminate string here
	}
}

/*
 * Function is to be called when the clear button is pressed
 * Function empties the input string, clearing all input.
 * Clears the calculator display area
 */
void clearButtonPress() {
	//Reset input string position to beginning
	numChars=0;
	inputString[numChars]= '\0'; //NULL terminate string
	//Called to clear the display
	updateCalculatorDisplay();
}

/*
 * Initialize the calculator button area by filling it with buttons and dimensions
 * Takes a buttonGrid pointer and initializes the grid
 */
void initGrid(ButtonGrid* buttonGrid) {

	for (int i=0; i<NUM_GRID_BUTTONS; i++) { //calculate button positions
		buttonGrid->buttons[i].x = 1+(i%GRID_WIDTH)*GRID_BUT_WIDTH;
		buttonGrid->buttons[i].y = GRID_Y_START + (i/GRID_WIDTH)*GRID_BUT_HEIGHT;
	}


    //Setup the buttons
	buttonGrid->buttons[r1c1].color = white_black_txt;
	buttonGrid->buttons[r1c1].text = "7";

	buttonGrid->buttons[r1c2].color = white_black_txt;
	buttonGrid->buttons[r1c2].text = "8";

	buttonGrid->buttons[r1c3].color = white_black_txt;
	buttonGrid->buttons[r1c3].text = "9";

	buttonGrid->buttons[r1c4].color = grey_blue_txt;
	buttonGrid->buttons[r1c4].text = "(";

	buttonGrid->buttons[r1c5].color = grey_blue_txt;
	buttonGrid->buttons[r1c5].text = ")";

	buttonGrid->buttons[r1c6].color = grey_blue_txt;
	buttonGrid->buttons[r1c6].text = "<";

	buttonGrid->buttons[r2c1].color = white_black_txt;
	buttonGrid->buttons[r2c1].text = "4";

	buttonGrid->buttons[r2c2].color = white_black_txt;
	buttonGrid->buttons[r2c2].text = "5";

	buttonGrid->buttons[r2c3].color = white_black_txt;
	buttonGrid->buttons[r2c3].text = "6";

	buttonGrid->buttons[r2c4].color = grey_blue_txt;
	buttonGrid->buttons[r2c4].text = "*";

	buttonGrid->buttons[r2c5].color = grey_blue_txt;
	buttonGrid->buttons[r2c5].text = "/";

	buttonGrid->buttons[r2c6].color = grey_blue_txt;
	buttonGrid->buttons[r2c6].text = "C";

	buttonGrid->buttons[r3c1].color = white_black_txt;
	buttonGrid->buttons[r3c1].text = "1";

	buttonGrid->buttons[r3c2].color = white_black_txt;
	buttonGrid->buttons[r3c2].text = "2";

	buttonGrid->buttons[r3c3].color = white_black_txt;
	buttonGrid->buttons[r3c3].text = "3";

	buttonGrid->buttons[r3c4].color = grey_blue_txt;
	buttonGrid->buttons[r3c4].text = "+";

	buttonGrid->buttons[r3c5].color = grey_blue_txt;
	buttonGrid->buttons[r3c5].text = "-";

	buttonGrid->buttons[r3c6].color = blue_white_txt;
	buttonGrid->buttons[r3c6].text = " ";

	buttonGrid->buttons[r4c1].color = white_black_txt;
	buttonGrid->buttons[r4c1].text = "0";

	buttonGrid->buttons[r4c2].color = grey_blue_txt;
	buttonGrid->buttons[r4c2].text = ".";

	buttonGrid->buttons[r4c3].color = grey_blue_txt;
	buttonGrid->buttons[r4c3].text = "A";

	buttonGrid->buttons[r4c4].color = grey_blue_txt;
	buttonGrid->buttons[r4c4].text = "^";

	buttonGrid->buttons[r4c5].color = blue_white_txt;
	buttonGrid->buttons[r4c5].text = " ";

	buttonGrid->buttons[r4c6].color = blue_white_txt;
	buttonGrid->buttons[r4c6].text = "=";

}

/*
 * Draws a GridButton on the screen
 */
void drawGridButton(GridButton button) {

	  BSP_LCD_SetFont(&Font20);

	  uint16_t backColor=0; //color behind text
	  uint16_t textColor=0; //color of text
	  uint16_t outlineColor=LCD_COLOR_GRAY; //outline color of button

	  //find button color scheme
	  switch (button.color) {
	  case white_black_txt:
		  backColor = LCD_COLOR_WHITE;
		  textColor = LCD_COLOR_BLACK;
		  break;
	  case grey_blue_txt:
		  backColor = LCD_COLOR_LIGHTGRAY;
		  textColor = LCD_COLOR_BLUE;
		  break;
	  case blue_white_txt:
		  backColor = LCD_COLOR_BLUE;
		  textColor = LCD_COLOR_WHITE;
		  outlineColor = LCD_COLOR_BLUE;
		  break;
	  }

	  //Draws the button
	  BSP_LCD_SetTextColor(outlineColor);
	  BSP_LCD_DrawRect(button.x, button.y, GRID_BUT_WIDTH, GRID_BUT_HEIGHT);

	  BSP_LCD_SetTextColor(backColor);
	  BSP_LCD_FillRect(button.x+1, button.y+1, GRID_BUT_WIDTH-1, GRID_BUT_HEIGHT-2);

	  BSP_LCD_SetBackColor(backColor);
	  BSP_LCD_SetTextColor(textColor);
	  //Draws text in middle of button
	  BSP_LCD_DisplayStringAt((button.x+(1/2.0)*GRID_BUT_WIDTH)-5, (button.y+(1/2.0)*GRID_BUT_HEIGHT)-5, (uint8_t*)button.text, LEFT_MODE);
	  //
	  //

}

/*
 * Draws a ButtonGrid on the screen
 */
void drawGrid(ButtonGrid buttonGrid) {

	if (GLOBAL_DEBUG) {
		printf("\nDEBUG_MSG-INFO: DRAWING BUTTON GRID\n");
    }

	//loops thru all buttons in grid and draws them
	for (int i=0; i<NUM_GRID_BUTTONS; i++) {
		drawGridButton(buttonGrid.buttons[i]);
	}
}

/*
 * Formats a double stored as a string for optimal display on the calculator
 * E.g. Given 4.00000 returns 4
 *      Given 4.3201000 returns 4.32101
 *
 * Assumes null terminated string containing only a valid double (using . decimal)
 */
void roundOffDouble(char* dblStr) {

	int lastNonZero=0; //Find last non zero char
	for (int i=0; i < strlen(dblStr); i++) {
		if (dblStr[i] !='0') {
			lastNonZero=i;
		}
	}

	if (lastNonZero+1 < strlen(dblStr)) { //terminate the string after the last non zero
		dblStr[lastNonZero+1]='\0';
	}

	if (dblStr[strlen(dblStr)-1]=='.') { //if all number after decimal point are cut, then cut the point too.
		dblStr[strlen(dblStr)-1]='\0';
	}

}

/*
 * Takes a double and converts it to a string.
 * If the number magnitude is very large (|num| >= 1000000) it is displayed in e^x notation
 * If the number magnitude is very small (|num| < 0.000001) it is displayed in e^-x notation
 */
void resultToDispStr(double result, char*output, int buffSize) {

	if (fabs((result) >= 1000000 || fabs(result) < 0.000001) && result != 0) {
		snprintf(output, buffSize, "%.10e", result);
	} else {
		snprintf(output, buffSize, "%.10f", result);
		roundOffDouble(output);
	}

}

/*
 * Updates the calculator display
 * If there has been a change in the calculator input, the new input is displayed in the primary display area
 * If the input is valid the preliminary calculation result in displayed in the secondary display area
 * If there has been no change in the calculator input then no action is taken
 */
void updateCalculatorDisplay(void) {

	//Detect if calculator input has changed
	if (numChars != prevNumChars) {

		if (GLOBAL_DEBUG) {
			printf("\nDEBUG_MSG-INFO: UPDATING DISPLAY\n");
	    }

		prevNumChars = numChars; //reset change detection

		//CLEAR THE PRIMARY AND SECONDARY DISPLAY AREAS
		BSP_LCD_SetFont(&Font16);
		BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
		BSP_LCD_SetBackColor(LCD_COLOR_WHITE);

		BSP_LCD_FillRect(CALC_DISP_R1_X, CALC_DISP_R1_Y, 320-CALC_DISP_R1_X, CALC_DISP_R1_HEIGHT);
		BSP_LCD_FillRect(CALC_DISP_R2_X, CALC_DISP_R2_Y, 320-CALC_DISP_R2_X, CALC_DISP_R2_HEIGHT);
		//

		BSP_LCD_SetTextColor(LCD_COLOR_BLACK);


		//Displays only the last part of input string that will fit on display
		if (numChars < CALC_DISP_R1_MAX_CHARS) {
			BSP_LCD_DisplayStringAt(CALC_DISP_R1_X, CALC_DISP_R1_Y, (uint8_t*)inputString, LEFT_MODE);
			setContIndicator(0);
		} else if (numChars >= CALC_DISP_R1_MAX_CHARS) {
			BSP_LCD_DisplayStringAt(CALC_DISP_R1_X, CALC_DISP_R1_Y, (uint8_t*)inputString+(numChars-CALC_DISP_R1_MAX_CHARS), LEFT_MODE);
			setContIndicator(1);
		}
		//

        //If the input string is currently valid display preliminary result in secondary display area
		if (strMathValid(inputString) && numChars > 0) {

			double result = evalString(inputString, ans);
			char output[CALC_DISP_R2_MAX_CHARS];

			resultToDispStr(result, output, CALC_DISP_R2_MAX_CHARS);

			BSP_LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);
			BSP_LCD_DisplayStringAt(CALC_DISP_R2_X, CALC_DISP_R2_Y, (uint8_t*)output, LEFT_MODE);

		}

	}

}

/*
 * Given a screen coordinate the function will return a position of the button grid (enum GridPos)
 */
GridPos coordToGridPos(uint16_t x, uint16_t y) {

	uint8_t posNum=0;

	posNum = ((y-GRID_Y_START)/GRID_BUT_HEIGHT)*(GRID_WIDTH); //determines the button row (base number, E.g. 0*GRID_WIDTH for row 1, 2*GRID_WIDTH for row 3)
	posNum += x/GRID_BUT_WIDTH; //determines the button column (additional number, added to base number, E.g. 0+0 for r1c1, 0+1 for r1c2, 2*GRID_WIDTH+1 for r3c2)

	if (y<GRID_Y_START) {
		posNum=-1; //Indicates that coordinate is not on button grid
	}

	return (GridPos) posNum;

}

/*
 * Handles a screen touch even, given the coordinate of the touch and the ButtonGrid on which it occurred
 * The function will call the appropriate method the handle the event.
 */
void handleScreenTouch(uint16_t x, uint16_t y, ButtonGrid butGrid) {

	GridPos touchButton = coordToGridPos(x, y); //gets the position in button grid of touch

	//Chooses the appropriate method
	if (touchButton != r1c6 && touchButton != r2c6 && touchButton != r3c6 && touchButton != r4c6 && touchButton != r4c5 && touchButton != OFF_GRID) {
		inputButtonPress(butGrid.buttons[touchButton]);
	} else if (touchButton == r1c6) {
		backButtonPress();
	} else if (touchButton == r2c6) {
		clearButtonPress();
	} else if (touchButton == r3c6 || touchButton == r4c6 || touchButton == r4c5) {
		equalPress();
	}

}

/*
 * Processes the calculator touch panel, if a valid touch is detected the function then handles the touch.
 */
void processTouchGrid(ButtonGrid* grid) {

	static TP_STATE state = NO_TOUCH; //current state of the touch panel
	static uint32_t wait_time_start=0; //stores when the wait state starts
	static uint32_t recover_time_start=0; //stores when the recovery state starts
	static int lastx=0, lasty=0; //stores the position of the last touch received

	int gotTouch = 0; //indicates whether a valid touch was received

	BSP_TP_GetDisplayPoint(&display); //reads the touch screen

	gotTouch = (display.x <318 && display.x >= 2); //determines whether touch was valid

	if (state == NO_TOUCH) { //If the screen is in the ready state

		if (gotTouch && (display.x != lastx || display.y != lasty)) { //Check that the touch point has changed

			state = TOUCH_WAIT; //enter the wait state
			wait_time_start = HAL_GetTick();
			lastx = display.x;
			lasty = display.y;

			if (GLOBAL_DEBUG) {
				printf("\nDUBUG_MSG-INFO: Touch Panel entered state: TOUCH_WAIT\n");
			}

		}

	} else if (state == TOUCH_WAIT) { //If the display is in the wait state

		if (HAL_GetTick() - wait_time_start > CALC_TOUCH_WAIT_TIME) { //and the wait time has elapsed

			if (gotTouch) { //if the screen in still touched

				handleScreenTouch(display.x, display.y, *grid); //handle the touch
				lastx = display.x;
				lasty = display.y;
				if (GLOBAL_DEBUG) {
					printf("\nDUBUG_MSG-INFO: Touch Panel got: x=%d, y=%d\n", display.x, display.y);
				}
				state = TOUCH_RECOVER; //enter the recover state
				recover_time_start = HAL_GetTick();

				if (GLOBAL_DEBUG) {
					printf("\nDUBUG_MSG-INFO: Touch Panel entered state: TOUCH_RECOVER\n");
				}

			} else {
				state = NO_TOUCH;
				if (GLOBAL_DEBUG) {
					printf("\nDUBUG_MSG-INFO: Touch Panel entered state: NO_TOUCH\n");
				}
			}

		}

	} else if (state == TOUCH_RECOVER) { //when recovery time has elapsed, return to the no touch state

		if (HAL_GetTick() - recover_time_start > CALC_TOUCH_RECOVER_TIME) {
			state = NO_TOUCH;
			lastx = display.x;
			lasty = display.y;
			if (GLOBAL_DEBUG) {
				printf("\nDUBUG_MSG-INFO: Touch Panel entered state: NO_TOUCH\n");
			}
		}

	}

}

/*
 * Turns on or off the indicator that shows when not all text is shown on screen
 */
void setContIndicator(int on) {

	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_FillRect(CALC_CONT_X, CALC_CONT_Y, 100, CALC_CONT_HEIGHT);

	BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
	BSP_LCD_SetFont(&Font16);

	if (on) {
		BSP_LCD_DisplayStringAt(CALC_CONT_X, CALC_CONT_Y, (uint8_t*)"<", LEFT_MODE);
	}

}
