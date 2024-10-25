# sken_module:master
モジュール回路のマスター用プログラムの雛型とヘッダファイル  
CANはCAN_1(A12,A11)を用いる  
MDDの制御は別売りのsken_library/mddを使用すること

## enum module_sending_name
SERVO_0~SERVO_F と SOLENOID_0~SOLENOID_F の計32の列挙型

## module_transmitter[32]
モジュール回路に送信するためのクラスのインスタンス。添字の指定は`enum module_sending_name`で行う

## void module_transmitter[i].transmit(uint8_t* container)
containerの先頭8つのデータを送信し、モジュールを制御する  
[サンプルコード]  
回路`SERVO_0`に配列`uint8_t send_data[8]`の指令を送信する
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include "sken_module.hpp"

uint8_t send_data[8] = {};

void main_interrupt(void) {
	/* Main code here */
	module_transmitter[SERVO_0].transmit(send_data);
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
```

## enum module_receiving_name
SENSOR_0~SENSOR_7 の計8の列挙型

## module_receiver[8]
センサモジュール回路から受信するためのクラスのインスタンス。添字の指定は`enum module_receiving_name`で行う

## void module_receiver[i].get_enc(int16_t* x, int16_t* y, int16_t* theta)
受信した自己位置を変数x、y、thetaに格納する  
[サンプルコード]  
回路`SENSOR_0`から受信した自己位置を変数`int16_t x,y,theta`に格納する
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include "sken_module.hpp"

int16_t x, y, theta;

void main_interrupt(void) {
	/* Main code here */
	module_receiver[SENSOR_0].get_enc(&x, &y, &theta);
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
```

## bool module_receiver[i].get_limit(int n)
受信したリミットスイッチの情報の内、n番目のスイッチが押されているならtrueを、押されていないならfalseを返す。  
[サンプルコード]  
回路`SENSOR_0`から0番リミットの情報を得る
``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"
#include "sken_module.hpp"


void main_interrupt(void) {
	/* Main code here */
	if (module_receiver[SENSOR_0].get_limit(0)) {
		
	}
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

```