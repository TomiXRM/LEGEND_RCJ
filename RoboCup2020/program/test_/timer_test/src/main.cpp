#include "Arduino.h"
#include "timer.h"//ライブラリ読み込み


timer TIMER1;//宣言
int time;

void setup() {
        TIMER1.reset();//初期化
        delay(1000);//1秒間
        time = TIMER1.read_ms();
        Serial.begin(230400);

}

void loop() {
        Serial.print(time);
        Serial.print("\r\n");
        delay(10);
}
