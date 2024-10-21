/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Tikuwa404
  * @version V1.0
  * @date    21-October-2024
  * @brief   module_sensor function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include <cmath>
#include "sken_library/include.h"
using namespace std;

/* Setting */
/* Measuring tire diameter  */
constexpr double TIRE_DIAMETER = 100.0;

/* tire-tire diameter  */
constexpr double BODY_DIAMETER = 500.0;

/* Encoder data sending can StdID
 *  0x100 ~ 0x107  */
constexpr uint32_t CAN_ENC_STDID = 0x100;

/* Limit switch data sending can StdID
 *  0x108 ~ 0x10F  */
constexpr uint32_t CAN_LIMIT_STDID = 0x108;

/* Tire 0 is front
 * Tire 1 is left
 * Tire 2 is back
 * Tire 3 is right */

/* Pin is from the left */

/* ------- */


class Point {
public:
	double x;
	double y;
	Point(double i=0, double j=0) : x(i),y(j) {}
	void rotate(double theta) {
		*this = rotated(*this, theta);
	}
	static Point rotated(Point i, double theta) {
		double rad = theta / 180 * M_PI;
		return Point(i.x*cos(rad)-i.y*sin(rad), i.x*sin(rad)+i.y*cos(rad));
	}
	Point operator=(Point i) {
		this->x = i.x;
		this->y = i.y;
		return *this;
	}
};

CanData received_raw;
uint8_t send_enc_data[8], send_limit_data[8];
Encoder enc[4];
Encoder_data enc_data[4];
double x=0, y=0, theta=0;

Gpio limit[8];

void main_interrupt(void) {
	for (int i = 0; i < 4; ++i) {
		enc[i].interrupt(&enc_data[i]);
	}
	enc_data[1].rps = -enc_data[1].rps;

	Point tmp((enc_data[0].volcity-enc_data[2].volcity)/2000, (enc_data[1].volcity-enc_data[3].volcity)/2000);
	tmp.rotate(-theta);
	x += tmp.x;
	y += tmp.y;
	theta += -(enc_data[0].volcity+enc_data[1].volcity+enc_data[2].volcity+enc_data[3].volcity)/4000/BODY_DIAMETER/PI*360;

	union {int16_t s16; uint8_t u8[2];} conv;
	conv.s16 = static_cast<int16_t>(x);
	send_enc_data[0] = conv.u8[0];
	send_enc_data[1] = conv.u8[1];
	conv.s16 = static_cast<int16_t>(y);
	send_enc_data[2] = conv.u8[0];
	send_enc_data[3] = conv.u8[1];
	conv.s16 = static_cast<int16_t>(theta);
	send_enc_data[4] = conv.u8[0];
	send_enc_data[5] = conv.u8[1];
	send_enc_data[6] = 0;
	send_enc_data[7] = 0;

	for (int i = 0; i < 8; ++i) {
		 send_limit_data[i] = !limit[i].read();
	}
}

void can_transmit(void) {
	sken_system.canTransmit(CAN_1, CAN_ENC_STDID, send_enc_data, 8);
	sken_system.canTransmit(CAN_1, CAN_LIMIT_STDID, send_limit_data, 8);
}


int main(void)
{
	sken_system.init();

	enc[0].init(A0, A1, TIMER5, TIRE_DIAMETER);
	enc[1].init(B3, A5, TIMER2, TIRE_DIAMETER); //invert
	enc[2].init(B6, B7, TIMER4, TIRE_DIAMETER);
	enc[3].init(C6, C7, TIMER8, TIRE_DIAMETER);
	limit[0].init(B14, INPUT_PULLUP);
	limit[1].init(B15, INPUT_PULLUP);
	limit[2].init(A8, INPUT_PULLUP);
	limit[3].init(A11, INPUT_PULLUP);
	limit[4].init(A6, INPUT_PULLUP);
	limit[5].init(A7, INPUT_PULLUP);
	limit[6].init(B9, INPUT_PULLUP);
	limit[7].init(B8, INPUT_PULLUP);

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);
	sken_system.addTimerInterruptFunc(can_transmit, 1, 10);

	while(true) {}
}
