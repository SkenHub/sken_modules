/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Tikuwa404
  * @version V2.0
  * @date    25-October-2024
  * @brief   module_master function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include "sken_module.hpp"


void main_interrupt(void) {
	/* Main code here */

	/* -------------- */
}

int main(void)
{
	sken_system.init();
	sken_module_init();

	/* Initialize here */

	/* --------------- */

	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);

	while(true) {
		sken_module_receive();
	}
}
