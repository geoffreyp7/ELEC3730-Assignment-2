//     $Date: 2018-03-26 08:32:18 +1100 (Mon, 26 Mar 2018) $
// $Revision: 1217 $
//   $Author: Peter $

#include "Ass-02.h"
#include "Calculator.h"

ButtonGrid grid;

void CalculatorInit(void)
{
  // STEPIEN: Assume horizontal display

  // Initialize and turn on LCD and calibrate the touch panel
  BSP_LCD_Init();
  BSP_LCD_DisplayOn();
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  BSP_TP_Init();

  BSP_LCD_Clear(LCD_COLOR_WHITE);
  initGrid(&grid);
  drawGrid(grid);

}

void CalculatorProcess(void)
{
	 updateCalculatorDisplay();
	 processTouchGrid(&grid);
}
