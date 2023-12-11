#include "LGMotors_DRV8432.h"
#include "mbed.h"

//NOTE 11/16 ComSLPはなぜか使えません。とりあえず今はcom_slp無視してください。11/16 By 祐聖
//NOTE 12/3  ComSLP使えちゃいましたあはは
//[参照]20110/11/15 モータまわすぜい @ゆうせいのレポート(と言う名のなにか).pages

//------------------------------------------------------------------------------------
Motors::Motors(PinName _pwm1,PinName _pwm2,PinName _pwm3,PinName _pwm4,PinName _comSleep) : m1_(_pwm1),m2_(_pwm2),m3_(_pwm3),m4_(_pwm4),slp(_comSleep),m1(_pwm1),m2(_pwm2),m3(_pwm3),m4(_pwm4){
        period_us(100);//50
        m1_.mode(PullUp);
        m2_.mode(PullUp);
        m3_.mode(PullUp);
        m4_.mode(PullUp);
        timer.start();
        stopAll();
};

//------------------------------------------------------------------------------------

void Motors::run(uint8_t index,float value){

        //    slp=1;
        //NOTE SLP disabled. see the note at the head of this file.
        slp = 1;
        if(index==1) {
                m1=value;
        }else if(index==2) {
                m2=value;
        }else if(index==3) {
                m3=value;
        }else if(index==4) {
                m4=value;
        }
}

void Motors::run(float p1,float p2,float p3,float p4){
        //p1,p2,p3,p4 are using -100 to 100 numbers.
        if(p1 == p2 && p3 == p4 && p1 == p3 && p1 == 0) {
                if(timer.read_ms() > 300) {
                        stopAll();
                }else{
                        stop(1); stop(2); stop(3); stop(4);
                }
        }else{
                slp = 1;
                timer.reset();
                p1*=DIR_M1; p2*=DIR_M2; p3*=DIR_M3; p4*=DIR_M4;
                if(p1 != 0) {
                        if(p1 > 0) {
                                if(p1 > 100) {
                                        p1 = 100;
                                }else if(p1 < 10) {
                                        p1=10;
                                }
                        }else{
                                if(p1 < -100) {
                                        p1 = -100;
                                }else if(p1 > -10) {
                                        p1=-10;
                                }
                        }
                }



                if(p2 != 0) {
                        if(p2 > 0) {
                                if(p2 > 100) {
                                        p2 = 100;
                                }else if(p2 < 10) {
                                        p2=10;
                                }
                        }else{
                                if(p2 < -100) {
                                        p2 = -100;
                                }else if(p2 > -10) {
                                        p2=-10;
                                }
                        }
                }


                if(p3 != 0) {
                        if(p3 > 0) {
                                if(p3 > 100) {
                                        p3 = 100;
                                }else if(p3 < 10) {
                                        p3=10;
                                }
                        }else{
                                if(p3 < -100) {
                                        p3 = -100;
                                }else if(p3 > -10) {
                                        p3=-10;
                                }
                        }
                }
                if(p4!= 0) {
                        if(p4 > 0) {
                                if(p4 > 100) {
                                        p4 = 100;
                                }else if(p4 < 10) {
                                        p4=10;
                                }
                        }else{
                                if(p4 < -100) {
                                        p4 = -100;
                                }else if(p4 > -10) {
                                        p4=-10;
                                }
                        }
                }
                p1/=100;
                p2/=100;
                p3/=100;
                p4/=100;

                m1 = p1/2+0.5;
                m2 = p2/2+0.5;
                m3 = p3/2+0.5;
                m4 = p4/2+0.5;
        }
}

//------------------------------------------------------------------------------------

void Motors::stopAll(){
        slp = 0;
        m1=0.5;
        m2=0.5;
        m3=0.5;
        m4=0.5;
        //slp=0;
        //NOTE SLP disabled. see the note at the head of this file.
}

void Motors::stop(uint8_t index){
        slp = 1;
        if(index==1) {
                m1=0.5;
        }else if(index==2) {
                m2=0.5;
        }else if(index==3) {
                m3=0.5;
        }else if(index==4) {
                m4=0.5;
        }

}

//------------------------------------------------------------------------------------
int Motors::drive(int deg,float power,float degree){
        float p1,p2,p3,p4;
        slp = 1;
        deg = -deg;
        deg +=100;

        p1 = (sin32_T(deg+DEG_M1) * power+degree);
        p2 = (sin32_T(deg+DEG_M2) * power+degree);
        p3 = (sin32_T(deg+DEG_M3) * power+degree);
        p4 = (sin32_T(deg+DEG_M4) * power+degree);

        run(p1,p2,p3,p4);
        return deg;
}


void Motors::driveVector(VectorXY vec,float Output){
        float M1 = vec.x* -0.7071 + vec.y* -0.7071 + Output;
        float M2 = vec.x* 0.7071 + vec.y* -0.7071 + Output;
        float M3 = vec.x* 0.7071 + vec.y* 0.7071 + Output;
        float M4 = vec.x* -0.7071 + vec.y* 0.7071 + Output;

        run(M1, M2, M3, M4);
}


//-----------------------------------------------------------------------------------

//Period Wrapper functions
void Motors::period(float seconds){
        m1.period(seconds);
        m2.period(seconds);
        m3.period(seconds);
        m4.period(seconds);
}


void Motors::period_us(int us){
        m1.period_us(us);
        m2.period_us(us);
        m3.period_us(us);
        m4.period_us(us);
}



void Motors::period_ms(int ms){
        m1.period_ms(ms);
        m2.period_ms(ms);
        m3.period_ms(ms);
        m4.period_ms(ms);
}
