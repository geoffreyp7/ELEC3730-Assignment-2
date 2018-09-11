/*
 * StringMathEvaluation.h
 *
 *  Created on: 16Apr.,2018
 *      Author: Geoffrey Parry-Grass c3232572
 *
 *      Allows the computation of the numerical value of a string containing mathematical operations and operands.
 *      Supported operations are:
 *
 *      Brackets ()
 *      Powers ^
 *      Division /
 *      Multiplication *
 *      Addition +
 *      Subtraction /
 *
 *      Processes floating point numbers, follows order of operations.
 *
 *      Use only evalString function and strMathValid, all others are internal.
 */

#ifndef STRINGMATHEVALUATION_H
#define STRINGMATHEVALUATION_H

#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>

#include "Ass-02.h"

//The possible types of nodes that can exist
typedef enum {

	numeric = 0,
	openBracket = 40,
	closeBracket = 41,
	power = 94,
	division = 47,
	multiplication = 42,
	addition = 43,
	subtraction = 45

} CalcNodeType;

//A node which represents a mathematical operation or operand
typedef struct CalcNode {

    CalcNodeType nodeType;
    double value;
    struct CalcNode* next;
    struct CalcNode* prev;

} CalcNode;

//A doubly linked list storing the mathematical operations and operands
typedef struct CalcList {

	CalcNode* head;
	CalcNode* tail;

} CalcList;

/*
 * USER FUNCTIONS
 */
//Given a mathematical string, the function evaluates it's numerical value.
extern double evalString(char * str, double ans);

//Checks whether a string is mathematically valid and capable of being processed by evalString.
extern int strMathValid(char * str);


/*
 * INTERNAL FUNCTIONS
 */
CalcList* strToCalcList(char * str, double ans);
CalcNode* makeCalcNode(CalcNodeType type, double value, CalcNode* next, CalcNode* prev);
int deleteCalcList(CalcList* calcList);
CalcNode* removeAndReplaceOperator(CalcNode* operatorNode, double result, CalcList* calcList);
int addToCalcList(CalcList* calcList, CalcNodeType nodeType, double value);
void printCalcList(CalcList* calcList);
int isOperator(char ch);
int isMinus(char ch);
int isBracket(char ch);

#endif /* STRINGMATHEVALUATION_H */
