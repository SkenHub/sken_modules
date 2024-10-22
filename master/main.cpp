/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Tikuwa404
  * @version V1.0
  * @date    21-October-2024
  * @brief   module_master function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

CanData received_raw;
uint8_t received_enc[8][8];
uint8_t received_limit[8][8];

void main_interrupt(void) {
	
}

void can_transmit(void) {
	
}

int main(void)
{
	sken_system.init();

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);
	sken_system.addTimerInterruptFunc(can_transmit, 1, 10);

	while(true) {
		for (int i = 0; i < 8; ++i) {
			switch (received_raw.rx_stdid) {
			case 0x100:
				received_enc[0][i] = received_raw.rx_data[i]; break;
			case 0x101:
				received_enc[1][i] = received_raw.rx_data[i]; break;
			case 0x102:
				received_enc[2][i] = received_raw.rx_data[i]; break;
			case 0x103:
				received_enc[3][i] = received_raw.rx_data[i]; break;
			case 0x104:
				received_enc[4][i] = received_raw.rx_data[i]; break;
			case 0x105:
				received_enc[5][i] = received_raw.rx_data[i]; break;
			case 0x106:
				received_enc[6][i] = received_raw.rx_data[i]; break;
			case 0x107:
				received_enc[7][i] = received_raw.rx_data[i]; break;
			case 0x108:
				received_limit[0][i] = received_raw.rx_data[i]; break;
			case 0x109:
				received_limit[1][i] = received_raw.rx_data[i]; break;
			case 0x10A:
				received_limit[2][i] = received_raw.rx_data[i]; break;
			case 0x10B:
				received_limit[3][i] = received_raw.rx_data[i]; break;
			case 0x10C:
				received_limit[4][i] = received_raw.rx_data[i]; break;
			case 0x10D:
				received_limit[5][i] = received_raw.rx_data[i]; break;
			case 0x10E:
				received_limit[6][i] = received_raw.rx_data[i]; break;
			case 0x10F:
				received_limit[7][i] = received_raw.rx_data[i]; break;
			}
		}
	}
}
