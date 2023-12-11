#include "mbed.h"
#include "LGBZ.h"

LGBZ::LGBZ(PinName pin) : Buzzer(pin){
        Buzzer.period(1.0/70000);
        Buzzer.write(volume);
        wait_ms(10);
        Buzzer.write(0);
        Bstate = 1;
        Lastmode = 255;
}


void LGBZ::sound(short num){
        if(Bstate == 1) {
                switch(num) {
                case 0:
                        if(Lastmode != num) {Buzzer.period(1.0/(3920));//3220
                                             Buzzer.write(volume);}
                        //wait_ms(3);
                        //Buzzer.write(0); //reset
                        break;
                case 1:
                        Buzzer.period(1.0/3000);
                        Buzzer.write(volume);
                        wait_ms(50);
                        Buzzer.period(1.0/6000);
                        wait_ms(30);
                        Buzzer.write(0);
                        break;
                case 2:
                        Buzzer.period(1.0/10000);
                        Buzzer.write(volume);
                        wait_ms(20);
                        Buzzer.period(1.0/4200);
                        wait_ms(50);
                        Buzzer.write(0);
                        break;
                case 3:
                        //login
                        Buzzer.period(1.0/9000);
                        Buzzer.write(volume);
                        wait_ms(100);
                        Buzzer.period(1.0/7200);
                        wait_ms(100);
                        Buzzer.period(1.0/1200);
                        wait_ms(100);
                        Buzzer.period(1.0/8000);
                        wait_ms(100);
                        Buzzer.write(0);
                        break;
                case 4:
                        //sublogin
                        Buzzer.write(0);
                        Buzzer.period(1.0/(tone_H*4));
                        Buzzer.write(volume);
                        wait_ms(70);
                        Buzzer.period(1.0/(tone_F*4));
                        wait_ms(70);
                        Buzzer.period(1.0/(tone_H*4));
                        wait_ms(70);
                        Buzzer.period(1.0/(tone_F*4));
                        wait_ms(70);
                        Buzzer.period(1.0/(tone_H*4));
                        wait_ms(70);
                        Buzzer.period(1.0/(tone_F*4));
                        wait_ms(300);
                        Buzzer.write(0);
                        break;
                case 5: /*おすきな音を入力してください*/
                        Buzzer.period(1.0/(tone_G*5));
                        Buzzer.write(volume);
                        wait_ms(80);
                        Buzzer.period(1.0/(tone_J*5));
                        wait_ms(60);
                        Buzzer.period(1.0/10000);
                        wait_ms(50);
                        Buzzer.write(0);
                        break;
                case 6: /*おすきな音を入力してください*/
                        Buzzer.write(volume);
                        Buzzer.write(0);
                        break;
                case 7: Buzzer.write(0); break;
                default:
                        Buzzer.period(1.0/3000);
                        Buzzer.write(volume);
                        wait_ms(10);
                        Buzzer.write(0); //reset
                        break;
                }
                Lastmode = num;//前回のnumを記憶
        }else{
                Buzzer.write(0);//off
        }
}
void LGBZ::State(bool ONOFF){
        Bstate = ONOFF;
}

bool LGBZ::GetState(){
        return Bstate;
}
