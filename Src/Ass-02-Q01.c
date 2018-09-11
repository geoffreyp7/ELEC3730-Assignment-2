//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#ifdef STM32F407xx
#include "usart.h"
#endif

uint8_t GLOBAL_DEBUG=0;

char command_line[MAX_CHARS];
int num_chars = 0;

void CommandLineParserInit(void)
{
  // Print welcome message
  printf("\014");
  printf("ELEC3730 Assignment 2\n");
  printf("Command Line Parser Example\n");
}

void CommandLineParserProcess(void)
{

  char c;

  // Check for input and echo back

  if (HAL_UART_Receive(&huart2, &c, 1, 0x0) == HAL_OK)
  {

  HAL_GPIO_TogglePin(GPIOD, LD4_Pin); // Toggle LED4
    if (c == 13) { //enter detected

      command_line[num_chars] = '\0'; //terminate the input

      char **array_of_words; //command line words
      int count; //number of words

      count = string_parser(command_line, &array_of_words); //parse the command line

      if (count >= 3) { //3 or more words

        opType operator = add; 
        int valid_command = 1;

        for (int i=3; i < num_chars; i++) { //check for non numbers

          if (!isdigit(command_line[i]) && command_line[i] != '.' && command_line[i] != ' ' && command_line[i] != '-') {
            valid_command = 0;
          }

        }
        
        num_chars = 0; //reset char count

        if (strcmp(array_of_words[0], "add") == 0) { //operation is add
          operator = add;
        } else if (strcmp(array_of_words[0], "sub") == 0) { //operation is sub
          operator = subtract;
          if (count > 3) { //if too many operands entered
              printf("Note: Subtract only accepts 2 operands\n");
              valid_command = 0;
          }
        } else if (strcmp(array_of_words[0], "mul") == 0) { //operation is mul
          operator = multiply;
        } else if (strcmp(array_of_words[0], "div") == 0) { //operation is div
          operator = divide;
          if (count > 3) { //if too many operands entered
            printf("Note: Divide only accepts 2 operands\n");
            valid_command = 0;
          }
        } else {
          valid_command = 0; //not any of the commands
        }

        if (valid_command) { 

          double ans = strtod(array_of_words[1], NULL); //first value
          for (int i=2; i<count; i++) { //extract values
            double val = strtod(array_of_words[i], NULL);

            switch (operator) { //perform the correct operation
            case add :
              ans += val;
              break;
            case subtract :
              ans -= val;
              break;
            case multiply :
              ans *= val;
              break;
            case divide :
              ans /= val;
              break;
            }
          }
          printf("\nResult: %f\n", ans);
        } else {
          printf("ERROR: Could not process command. Please check the input and try again.\n");
        }

        for (int k = 0; k < count; k++) { //free memory
          free(array_of_words[k]);
        }
        free(array_of_words);

      } else if (count > 0 && count <= 2) { //not a math command

        if (strcmp(array_of_words[0], "debug") == 0) { //debug command

          if (count == 1) { //only "debug" entered
            printf("Debug setting is: %d\n", GLOBAL_DEBUG);
          } else if (strcmp(array_of_words[1], "on") == 0) {
            GLOBAL_DEBUG = 1;
            printf("Debug messages on\n");
          } else if (strcmp(array_of_words[1], "off") == 0) {
            GLOBAL_DEBUG = 0;
            printf("Debug messages off\n");
          } else {
            printf("ERROR: Please enter on or off.\n");
          }

        } else if (strcmp(array_of_words[0], "help") == 0) {

          int printAll = 0;

          if (count == 1) {
            printAll = 1;
          }

          if (printAll || strcmp(array_of_words[1], "add") == 0) {
            printf("add <num1> .. <num N> : Sums one or more numbers.\n");
          }

          if (printAll || strcmp(array_of_words[1], "sub") == 0) {
            printf("sub <num1> <num 2> : Subtract two numbers.\n");
          }

          if (printAll || strcmp(array_of_words[1], "mul") == 0) {
            printf("mul <num1> .. <num N> : Multiply one or more numbers.\n");
          }

          if (printAll || strcmp(array_of_words[1], "div") == 0) {
            printf("div <num1> <num N> : Divide two numbers.\n");
          }

          if (printAll || strcmp(array_of_words[1], "debug") == 0) {
            printf("debug [<on|off>] Turn debug messages on or off or display status.\n");
          }

          if (printAll || strcmp(array_of_words[1], "help") == 0) {
            printf("help [<command>] Display help messages.\n");
          }

        } else {
        	printf("Invalid Command.\n");
        }


      }

      num_chars = 0;

    } else if (c == 127) { //backspace
        if (num_chars > 0) { //remove a from end if string is not empty
          num_chars--;
        }

    } else { //input added to string

        if (num_chars < MAX_CHARS-1) { //if there is room
          command_line[num_chars] = c;
        }
        num_chars++;

    }

  }

}

/*
 * Takes an input string and pointer to an array of words.
 * Breaks the input string into words and fills the array of words with the words.
 * Allocates heap memory to store the char * and each word is given a separate
 * allocation of heap memory to store the chars
 */
int string_parser(char *inp, char **array_of_words_p[])
{
    int string_length;
    int num_words = 0;

    char ** arr = malloc(sizeof(char*)*INIT_NUM_WORDS);
    if (!arr) {
      printf("\nERROR: Memory allocation error!\n\n");
      exit(1);
    }
    int word_mem = INIT_NUM_WORDS;

    //Check if input string is null and then if not,
    //store length of string and check if string is empty
    if (inp && (string_length = strlen(inp))) {

      //true when a space has been encountered,
      //initially set to true to capture first word,
      //reset at beginning of new word
      int space_found = 1;

      //Step through the entire string
      for (int i=0; i<string_length; i++) {

        if (inp[i] == ' ') {
          space_found = 1;
        }

              //A new word has been found
        if ((inp[i] != ' ' && inp[i]) && space_found) {

          if (num_words >= word_mem) {
            arr = realloc(arr, sizeof(char*)*(word_mem += INIT_NUM_WORDS));
            if (!arr) {
              printf("\nERROR: Memory allocation error!\n\n");
              exit(1);
            }
          }


          //Malloc an inital piece of memory
          arr[num_words] = malloc(sizeof(char)*INIT_WORD_LENGTH);
          if (!arr[num_words]) {
            printf("\nERROR: Memory allocation error!\n\n");
            exit(1);
          }

          int mem_size = INIT_WORD_LENGTH; //Size of current memory allocation
          int num_chars; //Number of characters currently in word

          //Step through the word and copy it into char * word
          for (num_chars = 0; inp[i] != ' ' && inp[i] != '\0'; i++, num_chars++) {

                      if (num_chars < mem_size) {
                        //If there is enough memory simply store the char
                        arr[num_words][num_chars] = inp[i];
                      } else {
                        //If there is not enough memory increase the size
                        mem_size += INIT_WORD_LENGTH;
                        arr[num_words] = realloc(arr[num_words], sizeof(char)*(mem_size));
                  if (!arr[num_words]) {
                    printf("\nERROR: Memory allocation error!\n\n");
                    exit(1);
                  }
                  arr[num_words][num_chars] = inp[i];
                      }
          }

          //Make the memory allocation only as big as the current word length
          //plus enough for the NULL character
          arr[num_words] = realloc(arr[num_words], sizeof(char) * (num_chars+1));
          if (!arr[num_words]) {
            printf("\nERROR: Memory allocation error!\n\n");
            exit(1);
          }

          //Terminate the string with the NULL character
          arr[num_words][num_chars] = '\0';
          num_chars++;

          space_found = 0; //reset for next word
          i--; //Step the index back to the end of the last word
          num_words++; //Increase the word count

        }
      }

      arr = realloc(arr, sizeof(char*)*(num_words));
      if (!arr) {
        printf("\nERROR: Memory allocation error!\n\n");
        exit(1);
      }
      *(array_of_words_p) = arr;

    }

    return num_words;
}
