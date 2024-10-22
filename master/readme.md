# sken_module:master
モジュール基盤のマスター用プログラム。  
CANはCAN_1(A12,A11)を用いる

## void main_interrupt(void)
1[ms]毎に呼び出される関数。主な処理はここに記述する。（中にsken_system.canTransmit()を入れるとSTMStudioとの通信ができない。）

## void can_transmit(void)
10[ms]毎に呼び出される関数。sken_system.canTransmit()関数はここに記述する。

## uint8_t received_enc[8][8]
0x100~0x107で受信した自己位置の情報が自動で格納される配列。  
x、y、theta(度数法)が送られてくる。

[例]  
0x100で受信した自己位置が入った配列は`received_enc[0]`である。  

[サンプルコード]  
0x100の自己位置を受信し、変数`int16_t x, y, z`に展開する

``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

CanData received_raw;
uint8_t received_enc[8][8];
uint8_t received_limit[8][8];

int16_t x, y, theta;

void main_interrupt(void) {
    union{int16_t s16, uint8_t u8[2]} conv;
    conv.u8[0] = received_enc[0][0];
    conv.u8[1] = received_enc[0][1];
    x = conv.s16;
    conv.u8[0] = received_enc[0][2];
    conv.u8[1] = received_enc[0][3];
    y = conv.s16;
    conv.u8[0] = received_enc[0][4];
    conv.u8[1] = received_enc[0][5];
    theta = conv.s16;
}

void can_transmit(void) {
}

int main(void)
{
	sken_system.init();

    sw.init(C13, INPUT_PULLUP);

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);
	sken_system.addTimerInterruptFunc(can_transmit, 1, 10);
    //以下省略
```

## uint8_t received_limit[8][8]
0x108~0x10Fで受信したリミットの情報が自動で格納される配列。  
リミットが押されているなら1、押されていないなら0が格納される。  

[例]  
0x109で受信したリミットの情報が入った配列は`received_limit[1]`であり、その中の5番リミットの情報は`received_limit[1][5]`に格納されている。

# サンプルコード
[サンプルコード]  
0x108のセンサーモジュールのリミット0番が押された時にLEDを点灯する

``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

CanData received_raw;
uint8_t received_enc[8][8];
uint8_t received_limit[8][8];

Gpio led;

void main_interrupt(void) {
    led.write(received_limit[0][0] ? HIGH : LOW);
}

void can_transmit(void) {

}

int main(void)
{
	sken_system.init();

    led.init(A5, OUTPUT);

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);
	sken_system.addTimerInterruptFunc(can_transmit, 1, 10);
    //以下省略
```

[サンプルコード]  
青ボタンが押された時0x320の電磁弁モジュールの0番の電磁弁を起動する

``` c++
#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "sken_library/include.h"

CanData received_raw;
uint8_t received_enc[8][8];
uint8_t received_limit[8][8];

Gpio sw;
uint8_t send_data[8] = {};

void main_interrupt(void) {
    if (sw.read()) {
        send_data[0] = 0;
    } else {
        send_data[0] = 1;
    }
}

void can_transmit(void) {
    sken_system.canTransmit(CAN_1, 0x320, send_data, 8);
}

int main(void)
{
	sken_system.init();

    sw.init(C13, INPUT_PULLUP);

	sken_system.startCanCommunicate(A12, A11, CAN_1);
	sken_system.addCanRceiveInterruptFunc(CAN_1, &received_raw);
	sken_system.addTimerInterruptFunc(main_interrupt, 0, 1);
	sken_system.addTimerInterruptFunc(can_transmit, 1, 10);
    //以下省略
```