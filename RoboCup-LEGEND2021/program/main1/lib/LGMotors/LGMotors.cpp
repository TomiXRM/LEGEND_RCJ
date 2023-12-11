#include "LGMotors.h"
#include "mbed.h"


//------------------------------------------------------------------------------------
LGMotors::LGMotors(PinName _pha1,PinName _ena1,PinName _pha2,PinName _ena2,PinName _phb1,PinName _enb1,PinName _phb2,PinName _enb2,PinName _comSleep) : PHA1(_pha1),ENA1(_ena1),PHA2(_pha2),ENA2(_ena2),PHB1(_phb1),ENB1(_enb1),PHB2(_phb2),ENB2(_enb2),slp(_comSleep){
        period_us(50);//50

        stopAll();
};

//------------------------------------------------------------------------------------

void LGMotors::run(uint8_t index,float value){
        switch (index) {
        case 1: runMotorIndex(value,PHA1,ENA1); break;
        case 2: runMotorIndex(value,PHA2,ENA2); break;
        case 3: runMotorIndex(value,PHB1,ENB1); break;
        case 4: runMotorIndex(value,PHB2,ENB2); break;
        default: break;
        }
}

void LGMotors::runMotorIndex(float M_speed,PwmOut& PH,PwmOut& EN){
        slp = 1;
        if(abs(M_speed) > 100) {
                if(M_speed > 0) {
                        M_speed = 100;
                }else{
                        M_speed = -100;
                }
        }
        if(abs(M_speed) <= 100) {

                if(M_speed == 0) {
                #ifdef MD_BRAKE
                        PH.write(1); EN.write(1);//ブレーキ
                #else
                        PH.write(0); EN.write(0);//フリー
                #endif
                }else{
                        if(M_speed > 0) {
                                //正
                                PH.write(0);
                                EN.write(abs(M_speed)/100);
                        }else{
                                //負
                                PH.write(abs(M_speed)/100);
                                EN.write(0);
                        }
                }
        }
}

void LGMotors::run(float p1,float p2,float p3,float p4){
        runMotorIndex(p1,PHA1,ENA1);
        runMotorIndex(p2,PHA2,ENA2);
        runMotorIndex(p3,PHB1,ENB1);
        runMotorIndex(p4,PHB2,ENB2);
}

//------------------------------------------------------------------------------------

void LGMotors::stopAll(){
        PHA1 = 0; ENA1 = 0;
        PHA2 = 0; ENA2 = 0;
        PHB1 = 0; ENB1 = 0;
        PHB2 = 0; ENB2 = 0;
        slp = 0;
        // run(0,0,0,0);
}

void LGMotors::stop(uint8_t index){

        switch (index) {
        case 1: PHA1 = 1; ENA1 = 1; break;
        case 2: PHA2 = 1; ENA2 = 1; break;
        case 3: PHB1 = 1; ENB1 = 1; break;
        case 4: PHB2 = 1; ENB2 = 1; break;
        default: break;
        }

}

//------------------------------------------------------------------------------------
int LGMotors::drive(int deg,float power,float degree){
        float p1,p2,p3,p4;
        slp = 1;
        deg = -deg;
        deg +=90;

        p1 = (sin32_T(deg+DEG_M1) * power+degree);
        p2 = (sin32_T(deg+DEG_M2) * power+degree);
        p3 = (sin32_T(deg+DEG_M3) * power+degree);
        p4 = (sin32_T(deg+DEG_M4) * power+degree);

        run(p1,p2,p3,p4);
        return deg;
}


void LGMotors::driveVector(VectorXY vec,float Output){
        // float M1 = vec.x* sin32_T(DEG_M1) + vec.y* cos32_T(DEG_M1) + Output;
        // float M2 = vec.x* sin32_T(DEG_M2) - vec.y* cos32_T(DEG_M2) + Output;
        // float M3 = vec.x* sin32_T(DEG_M3) + vec.y* cos32_T(DEG_M3) + Output;
        // float M4 = vec.x* sin32_T(DEG_M4) - vec.y* cos32_T(DEG_M4) + Output;
        float M1 = vec.x* -0.7071 + vec.y* -0.7071 + Output;
        float M2 = vec.x* 0.7071 + vec.y* -0.7071 + Output;
        float M3 = vec.x* 0.7071 + vec.y* 0.7071 + Output;
        float M4 = vec.x* -0.7071 + vec.y* 0.7071 + Output;
        run(M1, M2, M3, M4);
}


//-----------------------------------------------------------------------------------

//Period Wrapper functions
void LGMotors::period(float seconds){

        PHA1.period(seconds);
        ENA1.period(seconds);

        PHA2.period(seconds);
        ENA2.period(seconds);

        PHB1.period(seconds);
        ENB1.period(seconds);

        PHB2.period(seconds);
        ENB2.period(seconds);
}


void LGMotors::period_us(int us){

        PHA1.period_us(us);
        ENA1.period_us(us);

        PHA2.period_us(us);
        ENA2.period_us(us);

        PHB1.period_us(us);
        ENB1.period_us(us);

        PHB2.period_us(us);
        ENB2.period_us(us);
}



void LGMotors::period_ms(int ms){

        PHA1.period_ms(ms);
        ENA1.period_ms(ms);

        PHA2.period_ms(ms);
        ENA2.period_ms(ms);

        PHB1.period_ms(ms);
        ENB1.period_ms(ms);

        PHB2.period_ms(ms);
        ENB2.period_ms(ms);
}
