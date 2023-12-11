#include "mbed.h"
#include "LGSW_V2.h"

LGSW::LGSW(PinName sw1,PinName sw2,PinName sw3) : SW1(sw1),SW2(sw2),SW3(sw3),SP(NC)
{
        SW1.mode(PullUp);
        SW2.mode(PullUp);
        SW3.mode(PullUp);

        SPf = 1;
        PusshingTime1.start();
        PusshingTime2.start();
        CountCycle1.start();
        CountCycle2.start();
        sampling.attach_us(mbed::callback(this,&LGSW::sample),5000);
}

LGSW::LGSW(PinName sw1,PinName sw2,PinName sw3,PinName sp) : SW1(sw1),SW2(sw2),SW3(sw3),SP(sp)
{
        SW1.mode(PullUp);
        SW2.mode(PullUp);
        SW3.mode(PullUp);

        SPf = 1;
        PusshingTime1.start();
        PusshingTime2.start();
        CountCycle1.start();
        CountCycle2.start();
        sampling.attach_us(mbed::callback(this,&LGSW::sample),5000);
}

void LGSW::SetMode(short mode){
        ModeState = mode;
}
void LGSW::SetSw1(bool state){
        Sw1State = state;
}
short LGSW::GetMode(){
        return ModeState;
}

bool LGSW::GetSw1State(){
        return Sw1State;
}
void LGSW::SetSound(bool f){
        SPf = f;
}
bool LGSW::Sound(){
        DifferenceMode = PreviousMode - ModeState;
        PreviousMode = ModeState;
        if(abs(DifferenceMode) > 0) {
                return 1;
        }else{
                return 0;
        }

}




/*
   void LGSW::Sw1Fall(){
        Sw1State = !Sw1State;
        if(Sw1State == 0) {
                if(SPf == 1) {
                        SP.write(0);
                        SP.period(1.0/10000);
                        SP.write(0.5);
                }
                timeout.attach_us(mbed::callback(this,&LGSW::callback2),20000);
        }else{
                if(SPf == 1) {
                        SP.write(0);
                        SP.period(1.0/3000);
                        SP.write(0.5);
                }
                timeout.attach_us(mbed::callback(this,&LGSW::callback3),50000);
        }
   }
   void LGSW::Sw2Fall(){
        if(SPf == 1) {
                SP.write(0);
                SP.period(1.0/3000);
                SP.write(0.5);
        }
        timeout.attach_us(mbed::callback(this,&LGSW::callback),3000);
        DifferenceMode = PreviousMode = ModeState;
        ModeState -= 1;
   }
   void LGSW::Sw3Fall(){
        if(SPf == 1) {
                SP.write(0);
                SP.period(1.0/3000);
                SP.write(0.5);
        }
        timeout.attach_us(mbed::callback(this,&LGSW::callback),3000);
        DifferenceMode = PreviousMode = ModeState;
        ModeState += 1;
   }*/

void LGSW::callback(){
        SP.write(0); //reset
}

void LGSW::callback2(){
        SP.period(1.0/4200);
        timeout.attach_us(mbed::callback(this,&LGSW::callback),50000);
}

void LGSW::callback3(){
        SP.period(1.0/6000);
        timeout.attach_us(mbed::callback(this,&LGSW::callback),30000);
}

void LGSW::ReadLongPusshPlus(){
        if(SW2.read() == 0) {
                if(PusshingTime1.read_ms() > 500) {
                        if(CountCycle1.read_ms() > 25) {
                                CountCycle1.reset();
                                ModeState -= 1;
                                if(SPf == 1) {
                                        SP.period(1.0/3000);
                                        SP.write(0.5);
                                }
                                timeout.attach_us(mbed::callback(this,&LGSW::callback),5000);
                        }
                }else{
                        //  ModeState -= 1;
                }
        }else{
                PusshingTime1.reset();
        }
}
void LGSW::ReadLongPusshMinus(){
        if(SW3.read() == 0) {
                if(PusshingTime2.read_ms() > 500) {
                        if(CountCycle2.read_ms() > 25) {
                                CountCycle2.reset();
                                ModeState += 1;
                                if(SPf == 1) {

                                        SP.period(1.0/3000);
                                        SP.write(0.5);
                                }
                                timeout.attach_us(mbed::callback(this,&LGSW::callback),5000);
                        }
                }else{
                        //ModeState += 1;
                }
        }else{
                PusshingTime2.reset();
        }
}



void LGSW::sample(){
        states[0] = SW1.read();
        states[1] = SW2.read();
        states[2] = SW3.read();

        if(states[0] != laststates[0]&&states[0] == 0) {//押された
                Sw1State = !Sw1State;
                if(Sw1State == 0) {
                        if(SPf == 1) {
                                SP.write(0);
                                SP.period(1.0/10000);
                                SP.write(0.5);
                        }
                        timeout.attach_us(mbed::callback(this,&LGSW::callback2),20000);
                }else{
                        if(SPf == 1) {
                                SP.write(0);
                                SP.period(1.0/3000);
                                SP.write(0.5);
                        }
                        timeout.attach_us(mbed::callback(this,&LGSW::callback3),50000);
                }
        }else{
                // スイッチ離した、長押しなう

        }

        if(states[1] != laststates[1]&&states[1] == 0) {//押された
                DifferenceMode = PreviousMode = ModeState;
                ModeState -= 1;
                if(SPf == 1) {
                        SP.write(0);
                        SP.period(1.0/3000);
                        SP.write(0.5);
                } timeout.attach_us(mbed::callback(this,&LGSW::callback),3000);

        }else{
                // スイッチ離した、長押しなう
                ReadLongPusshMinus();

        }

        if(states[2] != laststates[2]&&states[2] == 0) {//押された
                DifferenceMode = PreviousMode = ModeState;
                ModeState += 1;
                if(SPf == 1) {
                        SP.write(0);
                        SP.period(1.0/3000);
                        SP.write(0.5);
                }
                timeout.attach_us(mbed::callback(this,&LGSW::callback),3000);
        }else{
                // スイッチ離した、長押しなう
                ReadLongPusshPlus();
        }


        laststates[0] = states[0];
        laststates[1] = states[1];
        laststates[2] = states[2];
}
