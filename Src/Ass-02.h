//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

// Assignment 2 include file

#ifndef ASS_02_H_
#define ASS_02_H_

// Standard includes
#ifdef STM32F407xx
#include "stm32f4xx_hal.h"
#include "openx07v_c_lcd.h"
#include "touch_panel.h"
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

// Assignment main
extern void Ass_02_Main(void);

//GLOBAL
extern uint8_t GLOBAL_DEBUG;

// Question 1
extern void CommandLineParserInit(void);
extern void CommandLineParserProcess(void);
extern int string_parser(char *inp, char **array_of_words_p[]);

#define MAX_CHARS 128

typedef enum {
	add = 0,
	subtract = 1,
	multiply = 2,
	divide = 3
} opType;

//Assignment 1 string parser
#define INIT_WORD_LENGTH 5
#define INIT_NUM_WORDS 2
//

// Question 2
extern void CalculatorInit(void);
extern void CalculatorProcess(void);

// Library functions
#ifdef STM32F407xx
extern uint8_t BSP_TP_Init(void);
extern uint8_t BSP_TP_GetDisplayPoint(Coordinate *pDisplay);
#endif

#endif /* ASS_02_H_ */
