/*
 * StringMathEvaluation.c
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
 *
 */

#include "StringMathEvaluation.h"

//Evaluates a CalcList
//Assumes a valid CalcList, CalcList validity must be confirmed
//before using function
double evalCalcList(CalcList* calcList, CalcNode* start, CalcNode* end) {

	CalcNode* curr = start; //evaluates from start position

	double result = 0;

	//Process Brackets
	while (curr != NULL && curr != end) {

		//When an open bracket is encountered
		if (curr->nodeType == openBracket) {

			//The bracketed portion is isolated
			CalcNode* newStart = NULL;
			CalcNode* newEnd = NULL;

            curr = curr->next; //one position after bracket
            newStart = curr; //start of bracketed portion

            //Finds end of bracketed portion (there may be other brackets in between)
            int bracketNum = 1;
            while (curr != NULL && bracketNum > 0) {

            	if (curr->nodeType == closeBracket) {
            		bracketNum--;
            	} else if (curr->nodeType == openBracket) {
            		bracketNum++;
            	}

            	if (bracketNum != 0) {
            		curr = curr->next;
            	}

            }

            newEnd = curr->prev; //End is one position back from closing bracket (i.e. brackets are not included in new calculation)

            if (GLOBAL_DEBUG) {
            	printf("\nDUBUG_MSG-INFO: OPERATION STACK LEVEL INCREASED IN: evalCalcList\n");
            }
            result = evalCalcList(calcList, newStart, newEnd); //recursively call this function on bracketed portion
            if (GLOBAL_DEBUG) {
            	printf("\nDUBUG_MSG-INFO: OPERATION STACK LEVEL DECREASED IN: evalCalcList\n");
            }

            CalcNode* openBrack = curr->prev->prev; //stores the open bracket node
            CalcNode* closeBrack = curr; //stores the close bracket node
            CalcNode* newNode = curr->prev; //Node that now stores result calculated from bracketed portion

            newNode->next = closeBrack->next; //Puts in result in between the existing brackets
            newNode->prev = openBrack->prev;

            ///////////REMOVES THE BRACKETS FROM THE LIST

            //Detects if the result is now at the start of the calcList and adds node to list appropriately
            if (openBrack->prev == NULL) {
            	calcList->head = newNode;
            } else {
            	openBrack->prev->next = newNode;
            }

            //Detects if the result is now at the end of the calcList and adds node to list appropriately
            if (closeBrack->next == NULL) {
            	calcList->tail = newNode;
            } else {
            	closeBrack->next->prev = newNode;
            }

            //If the bracketed portion was the starting position then the result is the new start
            if (openBrack == start) {
            	start = newNode;
            }

            //If the bracketed portion was the ending position then the result is the new end
            if (closeBrack == end) {
            	end = newNode;
            }

            free (openBrack); //free the bracket nodes
            free (closeBrack);

            ///////////

            curr = newNode; //Continue from the new node

		}
		curr = curr->next; //Step thru the calcList
	}

	//Go from start
	curr = start;
	//Process Indices
	while (curr != NULL && curr != end) {

        if (curr->nodeType == power) { //^ symbol found

			CalcNode* tempAfterOp = curr->next; //node after ^ (a numerical value)
			CalcNode* tempBeforeOp = curr->prev; //node before ^ (a numerical value)

        	result = pow(curr->prev->value, curr->next->value); //perform the operation
			CalcNode* newCalcNode = removeAndReplaceOperator(curr, result, calcList); //replace the operand operator operand with the result
			curr = newCalcNode; //step to new node

			//If the operation was at the end
			if (tempAfterOp == end) {
				end = newCalcNode; //the new node is the new end
			}
			//likewise for start
			if (tempBeforeOp == start) {
				start = newCalcNode;
			}

        }

		curr = curr->next; //step to next node

	}

	curr = start;
	//Process Multiplication and Division
	while (curr != NULL && curr != end) {

		if (curr->nodeType == multiplication) { //same process as for indices just different operation (see above)

			CalcNode* tempAfterOp = curr->next;
			CalcNode* tempBeforeOp = curr->prev;

			result = curr->prev->value * curr->next->value;
			CalcNode* newCalcNode = removeAndReplaceOperator(curr, result, calcList);
			curr = newCalcNode;

			if (tempAfterOp == end) {
				end = newCalcNode;
			}
			if (tempBeforeOp == start) {
				start = newCalcNode;
			}

		} else if (curr->nodeType == division) { //same process as for indices just different operation (see above)

			CalcNode* tempAfterOp = curr->next;
			CalcNode* tempBeforeOp = curr->prev;

			result = curr->prev->value / curr->next->value;
			CalcNode* newCalcNode = removeAndReplaceOperator(curr, result, calcList);
			curr = newCalcNode;

			if (tempAfterOp == end) {
				end = newCalcNode;
			}
			if (tempBeforeOp == start) {
				start = newCalcNode;
			}

		}

		curr = curr->next;

	}

	curr = start;
	//Process Addition and Subtraction
	while (curr != NULL && curr != end) {

		if (curr->nodeType == addition) { //same process as for indices just different operation (see above)

			CalcNode* tempAfterOp = curr->next;
			CalcNode* tempBeforeOp = curr->prev;

			result = curr->prev->value + curr->next->value;
			CalcNode* newCalcNode = removeAndReplaceOperator(curr, result, calcList);
			curr = newCalcNode;

			if (tempAfterOp == end) {
				end = newCalcNode;
			}
			if (tempBeforeOp == start) {
				start = newCalcNode;
			}

		} else if (curr->nodeType == subtraction) { //same process as for indices just different operation (see above)

			CalcNode* tempAfterOp = curr->next;
			CalcNode* tempBeforeOp = curr->prev;

			result = curr->prev->value - curr->next->value;
			CalcNode* newCalcNode = removeAndReplaceOperator(curr, result, calcList);
			curr = newCalcNode;

			if (tempAfterOp == end) {
				end = newCalcNode;
			}
			if (tempBeforeOp == start) {
				start = newCalcNode;
			}

		}

		curr = curr->next;

	}

	//return result;
	return start->value;

}

/*
 * Removes an operator, operand, operator pattern in the list and replaces it with the result
 */
CalcNode* removeAndReplaceOperator(CalcNode* operatorNode, double result, CalcList* calcList) {

	CalcNode* newCalcNode = makeCalcNode(numeric, result, operatorNode->next->next
			, operatorNode->prev->prev); //makes a new node to store the result

	//determines if the start of the pattern is the head of the list to insert correctly into list
	if (operatorNode->prev->prev != NULL) {
		operatorNode->prev->prev->next = newCalcNode;
	} else {
		calcList->head = newCalcNode;
	}

	//determines if the start of the pattern is the tail of the list to insert correctly into list
	if (operatorNode->next->next != NULL) {
		operatorNode->next->next->prev = newCalcNode;
	} else {
		calcList->tail = newCalcNode;
	}

	//free nodes (operand 1, operator, operand 2)
	free(operatorNode->prev);
	free(operatorNode->next);
	free(operatorNode);

	return newCalcNode;

}

/*
 * Makes a new node to store an operand or operator
 */
CalcNode* makeCalcNode(CalcNodeType type, double value, CalcNode* next, CalcNode* prev) {

	CalcNode* calcNode = malloc(sizeof(CalcNode));
	calcNode->nodeType = type;
	calcNode->value = value;
	calcNode->next = next;
	calcNode->prev = prev;
	return calcNode;

}

/*
 * Adds a calcNode to a CalcList (adds to end)
 */
int addToCalcList(CalcList* calcList, CalcNodeType nodeType, double value) {

	if (!calcList) { //Checks for non NULL calclist
		return 0;
	}

	if (calcList->head == NULL) { //If there is no head (empty list)

		//Make the node and store it as the head (and tail, first node in list)
		CalcNode* newCalcNode = makeCalcNode(nodeType, value, NULL, NULL);
		if (!newCalcNode) { //memory allocation failure
			return 0;
		}

		calcList->head = newCalcNode;
		calcList->tail = newCalcNode;

	} else { //If the list already contains items

		//Make and insert node at end of list
		CalcNode* newCalcNode = makeCalcNode(nodeType, value, NULL, calcList->tail);
		if (!newCalcNode) {
			return 0;
		}

		calcList->tail->next = newCalcNode;
		calcList->tail = newCalcNode;
	}

	return 1;

}

/*
 * Given a string, checks whether it is mathematically valid, i.e. can be processed as a calclist
 */
int strMathValid(char * str) {

	//Reasons a string may not be mathematically valid
	//Mismatched Brackets
	//Consecutive Operators
	//Violation of operand operator operand pattern
	//Disallowed symbols

	int bracketNum = 0; //used to check for mismatched brackets

	for (int i=0; i<strlen(str); i++) {

		if (isOperator(str[i]) && !isBracket(str[i]) && !isMinus(str[i])) {
            //Handle case of an operator (not bracket or -)
			//Check before and after symbol for invalid operands
			if (i-1 < 0) {
				return 0;
			} else if (i+1 >= strlen(str)) {
				return 0;
			} else if (!isdigit(str[i-1]) && str[i-1] != 'A' && str[i-1] != closeBracket) {
				return 0;
			} else if (!isdigit(str[i+1]) && str[i+1] != 'A' && !isMinus(str[i+1]) && str[i+1] != openBracket) {
				return 0;
			}

		} else if (isBracket(str[i])) {

			//Handle case of bracket (check for empty brackets, )(, mismatched brackets, and other invalid brackets
			if (str[i]==openBracket) {
				bracketNum++;

				if (i+1 < strlen(str)) {

					if (str[i+1]==closeBracket) {
						return 0;
					}

					if (i-1 >= 0) {
						if (str[i-1]==closeBracket) {
							return 0;
						} else if (isdigit(str[i-1]) || str[i-1]=='A') {
							return 0;
						}
					}

				} else {
					return 0;
				}

			} else if (str[i]==closeBracket) {
				bracketNum--;
			}

		} else if (isMinus(str[i])) { //check if - is used in a valid way

			if (i+1>=strlen(str)) { //if minus used at very end of string
				return 0;
			} else if (!isdigit(str[i+1]) && str[i+1] != 'A' && !isMinus(str[i+1]) && str[i+1] != openBracket) { //if not followed by valid symbol
				return 0;
			}

		} else if (isdigit(str[i])) {

		    int numPoints=0; //step past double as check for valid decimal point usage
		    int numAfterPoint=1;
		    while ((isdigit(str[i]) || str[i]=='.') && i<strlen(str)) {

		    	if (str[i] == '.') {
		    		numPoints++;
		    		numAfterPoint=0;

		    		if (numPoints>1) {
		    			return 0;
		    		}

		    	}

	    		if (numPoints && isdigit(str[i])) {
	    	        numAfterPoint=1;
	    		}

		    	i++;

		    }
		    i--;
		    if (!numAfterPoint) {
		    	return 0;
		    }

	    } else if (str[i]=='A') {

	    	if (i-1 >= 0 && !isOperator(str[i-1])) {
	    		return 0;
	    	}

	    	if (i+1 < strlen(str) && !isOperator(str[i+1])) {
	    		return 0;
	        }


	    } else {
			return 0;
		}

	}

	if (bracketNum != 0) {
		return 0;
	}

	return 1;

}

/*
 * Returns true if char is a math operator, false otherwise
 */
int isOperator(char ch) {
	if (ch == openBracket || ch == closeBracket || ch == power
			|| ch == division || ch == multiplication || ch == addition
			|| ch == subtraction) {
		return 1;

	} else {
		return 0;
	}
}

/*
 * returns true if char is a ( or ), false otherwise
 */
int isBracket(char ch) {
	if (ch == openBracket || ch == closeBracket) {
		return 1;

	} else {
		return 0;
	}
}

//returns true if char is a -, false otherwise
int isMinus(char ch) {
	if (ch == subtraction) {
		return 1;

	} else {
		return 0;
	}
}

/*
 * Converts a null terminated math string to a calclist for further processing
 */
CalcList* strToCalcList(char * str, double ans) {

	if (!str || !strMathValid(str)) { //check for empty or invalid string
		return NULL;
	}

	CalcList* calcList = malloc(sizeof(CalcList)); //initiate a new calclist
	calcList->head = NULL;
	calcList->tail = NULL;

	int negative=0; //keep track of negative value

	for (int i=0; str[i] != '\0'; i++) { //step thru the string

		CalcNodeType nodeType = 0;
		double value = 0;

		if (isOperator(str[i]) && !isMinus(str[i])) { //handles all operators other than -
			nodeType = str[i];
			addToCalcList(calcList, nodeType, value); //simply adds them to the list
		} else if (isdigit(str[i])) { //handles numeric node
			nodeType = numeric;
			value = strtod(&str[i], NULL);

			if (negative) { //if value is to be negative
				value *= -1; // make it negative
				negative = 0;
			}

			addToCalcList(calcList, nodeType, value);

			while ((isdigit(str[i]) || str[i] == '.') && str[i] != '\0') { //step past the number
				i++;
			}
			i--;
		} else if (str[i] == 'A') { //handles the ans symbol
			nodeType = numeric;
			value = ans;
			if (negative) {
				value *= -1;
				negative = 0;
			}
			addToCalcList(calcList, nodeType, value);
		} else if (isMinus(str[i]) && i-1 >= 0 && (isdigit(str[i-1]) || str[i-1] == closeBracket || str[i-1] == 'A') ) { //handles - used as an operation
			nodeType = subtraction;
			addToCalcList(calcList, nodeType, value);
		} else if (isMinus(str[i])) { //handles - used as a negative sign
			negative = !negative;
		}

	}

	return calcList;

}

/*
 * Prints out a calclist (used for error checking)
 */
void printCalcList(CalcList* calcList) {

	CalcNode* curr = calcList->head;
	printf("|");
	while (curr != NULL) {
		if (curr->nodeType == numeric) {
			printf("%f", curr->value);
		} else {
			printf("%c", curr->nodeType);
		}
		if (curr!=calcList->tail) {
			printf("[->]");
		}
		curr = curr->next;
	}
	printf("|\n");

}

/*
 * Given a math string and prev ans (if available otherwise pass 0), the function converts the string to
 * a calclist and computes the value
 */
double evalString(char * str, double ans) {

	CalcList* temp; //temp calclist to compute value

	if (!(temp = strToCalcList(str, ans))) { //convert string to calclist
		if (GLOBAL_DEBUG) {
		    printf("\nDUBUG_MSG-WARN: CALCLIST CREATION FAILED IN FUNC: evalString\n");
		}
		return 0;
	} else {

		if (GLOBAL_DEBUG) {
			printf("\nDUBUG_MSG-INFO: CALCLIST CREATED IN FUNC: evalString\n");
			printCalcList(temp);
		}

		double result = evalCalcList(temp, temp->head, temp->tail); //caculate value

		deleteCalcList(temp); //delete the temp list
		if (GLOBAL_DEBUG) {
			printf("\nDUBUG_MSG-INFO: CALCLIST DELETED IN FUNC: evalString\n");
		}

		return result; //return calculated result

	}

}

/*
 * Frees all nodes in list and frees the list
 */
int deleteCalcList(CalcList* calcList) {
    CalcNode* curr = calcList->head;
    while(curr != NULL) {
    	CalcNode* next = curr->next;
    	free(curr);
    	curr = next;
    }
    free(calcList);
    return 1;
}

