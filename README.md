# sken_modules
モジュール基盤用プログラム置き場  
sken_library必須

# モジュールプログラム仕様
## 1. MDDモジュール
### 機能
- 4つのエンコーダーと4つのモータードライバーを制御。
- メカナムホイール、オムニホイール3輪・4輪の対応。
- RSP、PWMモードの切り替え機能。
- CAN通信でモータードライバーへデータを受け取る（ID：300～30F）

### 動作概要
- CAN通信でベクトル受け取り，足回り制御
- PWM：パルス幅変調（PWM）信号を用いてモーターの速度を調整
- RSPモード：各モーターをRPSで制御。
- 
## 2. サーボモジュール
### 機能
- 8つのサーボモーターを制御
- CAN通信を受信（ID：310～31F）
- 0～100の値を受け取り、これに基づいてサーボモータの角度を制御

### 動作概要
- 各サーボモータにはCAN通信で受け取った0～100の値が送られ、サーボモータの角度を決定


## 3. センサーモジュール
### 機能
- 4つのエンコーダーと8つのリミットスイッチを使用
- 自己位置とリミットスイッチの状態をCAN通信で送信
  - 自己位置データ送信：ID 310～317
  - リミットスイッチデータ送信：ID 318～31F

### 動作概要
- エンコーダーの値から自己位置を計算し、CAN通信で送信
- リミットスイッチの状態（オン/オフ）をCAN通信で送信

## 4.電磁弁ドモジュール
### 機能
- 6つの電磁弁を制御。
- CAN通信を受信（ID：320～32F）
- 1か0の値を受け取り、それに基づいてソレノイド（電磁弁）の開閉を制御

### 動作概要
- それぞれのソレノイドはCAN通信で受け取った信号（1または0）に従って作動
- 信号1でソレノイドを開き、信号0でソレノイドを閉じる


## 5. マスターモジュール
### 機能
- PCとのシリアル通信を行い、各モジュールとのデータをCAN経由で送受信
- 全てのモジュール（エンコーダー、サーボ、センサー、ソレノイド）に対して制御指令を送信
  
### 動作概要
- PCからの指令を受け取り、それに基づいて各モジュールへCAN通信で命令を送信
- 各モジュールから受け取ったデータをPCに転送
