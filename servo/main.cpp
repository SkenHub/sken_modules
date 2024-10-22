/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Tikuwa404
  * @version V1.0
  * @date    19-October-2024
  * @brief   module_servo function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

/* Setting */
/* PWM width
 *  The number can be negative  */
constexpr int SERVO_MIN_PWM[8] = {   0,   0,   0,   0,   0,   0,   0,   0};
constexpr int SERVO_MAX_PWM[8] = { 100, 100, 100, 100, 100, 100, 100, 100};

/* Receiving can StdID
 *  0x310 ~ 0x31F  */
constexpr uint32_t CAN_STDID = 0x310;

/* Pin is from the left */

/* ------- */


CanData received_raw;
uint8_t received_data[8];
RcPwm servo[8];

void main_interrupt(void) {
	for (int i = 0; i < 8; ++i) {
		servo[i].turn(SERVO_MIN_PWM[i] + (SERVO_MAX_PWM[i]-SERVO_MIN_PWM[i])/100*received_data[i]);
	}
}

int main(void)
{
	sken_system.init();

	servo[0].init(B6, TIMER4, CH1);
	servo[1].init(B7, TIMER4, CH2);
	servo[2].init(B8, TIMER4, CH3);
	servo[3].init(B9, TIMER4, CH4);
	servo[4].init(C6, TIMER3, CH1);
	servo[5].init(C7, TIMER3, CH2);
	servo[6].init(C8, TIMER3, CH3);
	servo[7].init(C9, TIMER3, CH4);

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);

	while(true) {
		for (int i = 0; i < 8; ++i) {
			if (received_raw.rx_stdid == CAN_STDID) {
				received_data[i] = received_raw.rx_data[i];
			}
		}
	}
}
