/*
 * sken_module.hpp
 *
 *  Created on: 2024/10/24
 *      Author: Tikuwa404
 */

#ifndef SKEN_MODULE_HPP_
#define SKEN_MODULE_HPP_

#include "sken_library/include.h"

enum module_sending_name {
	SERVO_0,SERVO_1,SERVO_2,SERVO_3,SERVO_4,SERVO_5,SERVO_6,SERVO_7,SERVO_8,SERVO_9,SERVO_A,SERVO_B,SERVO_C,SERVO_D,SERVO_E,SERVO_F,
	SOLENOID_0,SOLENOID_1,SOLENOID_2,SOLENOID_3,SOLENOID_4,SOLENOID_5,SOLENOID_6,SOLENOID_7,SOLENOID_8,SOLENOID_9,SOLENOID_A,SOLENOID_B,SOLENOID_C,SOLENOID_D,SOLENOID_E,SOLENOID_F
};

constexpr uint32_t module_sending_ID[32] = {
	0x310,0x311,0x312,0x313,0x314,0x315,0x316,0x317,0x318,0x319,0x31A,0x31B,0x31C,0x31D,0x31E,0x31F,
	0x320,0x321,0x322,0x323,0x324,0x325,0x326,0x327,0x328,0x329,0x32A,0x32B,0x32C,0x32D,0x32E,0x32F
};

enum module_receiving_name {
	SENSOR_0=0,SENSOR_1,SENSOR_2,SENSOR_3,SENSOR_4,SENSOR_5,SENSOR_6,SENSOR_7
};

constexpr uint32_t module_receiving_ID[8] = {
	0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107
};

class {
public:
	uint8_t data[8];
	bool transmit_flag = false;
	void transmit(uint8_t* container) {
		for (int i = 0; i < 8; ++i) {
			data[i] = container[i];
			transmit_flag = true;
		}
	}
} module_transmitter[32];

class {
public:
	uint8_t enc_data[8];
	uint8_t limit_data[8];
	void get_enc(int16_t* x, int16_t* y, int16_t* theta) {
		union{int16_t s16; uint8_t u8[2];} conv;
		conv.u8[0] = enc_data[0];
		conv.u8[1] = enc_data[1];
		*x = conv.s16;
		conv.u8[0] = enc_data[2];
		conv.u8[1] = enc_data[3];
		*y = conv.s16;
		conv.u8[0] = enc_data[4];
		conv.u8[1] = enc_data[5];
		*theta = conv.s16;
	}
	bool get_limit(int n) {
		return limit_data[n] == 1 ? true : false;
	}
} module_receiver[8];

CanData sken_module_received_raw;

void sken_module_can_transmit(void) {
	for (int i = 0; i < 32; ++i) {
		if (module_transmitter[i].transmit_flag) {
			module_transmitter[i].transmit_flag = false;
			sken_system.canTransmit(CAN_1, module_sending_ID[i], module_transmitter[i].data, 8);
		}
	}
}

void sken_module_init(void) {
	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &sken_module_received_raw);
	sken_system.addTimerInterruptFunc(sken_module_can_transmit, 7, 10);
}

void sken_module_receive(void) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (sken_module_received_raw.rx_stdid == module_receiving_ID[j]) {
				module_receiver[j].enc_data[i] = sken_module_received_raw.rx_data[i];
			}
			if (sken_module_received_raw.rx_stdid == module_receiving_ID[j] + 8) {
				module_receiver[j].limit_data[i] = sken_module_received_raw.rx_data[i];
			}
		}
	}
}

#endif /* SKEN_MODULE_HPP_ */
