#include "LGMotors.h"
#include "mbed.h"
#define PI 3.14159265358979323846264338327


//NOTE 11/16 ComSLPはなぜか使えません。とりあえず今はcom_slp無視してください。11/16 By 祐聖
//NOTE 12/3  ComSLP使えちゃいましたあはは
//[参照]2018/11/15 モータまわすぜい @ゆうせいのレポート(と言う名のなにか).pages

//------------------------------------------------------------------------------------
Motors::Motors(PinName _pwm1,PinName _pwm2,PinName _pwm3,PinName _pwm4,PinName _comSleep) : m1_(_pwm1),m2_(_pwm2),m3_(_pwm3),m4_(_pwm4),slp(_comSleep),m1(_pwm1),m2(_pwm2),m3(_pwm3),m4(_pwm4){
        period_us(24);//50
        m1_.mode(PullUp);
        m2_.mode(PullUp);
        m3_.mode(PullUp);
        m4_.mode(PullUp);

        stopAll();
        Deg_m1 = 135;
        Deg_m2 = -135;
        Deg_m3 = -45;
        Deg_m3 = 5;
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
        slp = 1;

        if(p1 != 0) {
                if(p1 > 0) {
                        if(p1 > 92) {
                                p1 = 92;
                        }else if(p1 < 8) {
                                p1=8;
                        }
                }else{
                        if(p1 < -92) {
                                p1 = -92;
                        }else if(p1 > -8) {
                                p1=-8;
                        }
                }
        }



        if(p2 != 0) {
                if(p2 > 0) {
                        if(p2 > 92) {
                                p2 = 92;
                        }else if(p2 < 8) {
                                p2=8;
                        }
                }else{
                        if(p2 < -92) {
                                p2 = -92;
                        }else if(p2 > -8) {
                                p2=-8;
                        }
                }
        }


        if(p3 != 0) {
                if(p3 > 0) {
                        if(p3 > 92) {
                                p3 = 92;
                        }else if(p3 < 8) {
                                p3=8;
                        }
                }else{
                        if(p3 < -92) {
                                p3 = -92;
                        }else if(p3 > -8) {
                                p3=-8;
                        }
                }
        }
        if(p4!= 0) {
                if(p4 > 0) {
                        if(p4 > 92) {
                                p4 = 92;
                        }else if(p4 < 8) {
                                p4=8;
                        }
                }else{
                        if(p4 < -92) {
                                p4 = -92;
                        }else if(p4 > -8) {
                                p4=-8;
                        }
                }
        }
        // m1=((p1/2)+50)/100;
        // m2=((p2/2)+50)/100;
        // m3=((p3/2)+50)/100;
        p1/=100;
        p2/=100;
        p3/=100;
        p4/=100;

        m1 = p1/2+0.5;
        m2 = p2/2+0.5;
        m3 = p3/2+0.5;
        m4 = p4/2+0.5;



}

//------------------------------------------------------------------------------------

void Motors::stopAll(){
        slp = 1;
        m1=0.5;
        m2=0.5;
        m3=0.5;
        m4=0.5;
        //slp=0;
        //NOTE SLP disabled. see the note at the head of this file.
}

void Motors::stop(uint8_t index){

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
        deg +=90;

        ///  p1=_sin(9);
// sin32_T(theta)
        // p1 = (sin((deg+45) * PI / 180) * power+degree);
        // p2 = (sin((deg+135) * PI / 180) * power+degree);
        // p3 = (sin((deg-135) * PI / 180) * power+degree);
        // p4 = (sin((deg-45) * PI / 180) * power+degree);//*/



        p1 = (sin32_T(deg+45) * power+degree);
        p2 = (sin32_T(deg+135) * power+degree);
        p3 = (sin32_T(deg-135) * power+degree);
        p4 = (sin32_T(deg-45) * power+degree);//*/



        run(p1,p2,p3,p4);

        return deg;
}


void Motors::driveVector(VectorXY vec,float Output){
        float M1 = vec.x* -0.7071 + vec.y* 0.7071 + Output;
        float M2 = vec.x* -0.7071 + vec.y* -0.7071 + Output;
        float M3 = -vec.x* -0.7071 + vec.y* -0.7071 + Output;
        float M4 = -vec.x* -0.7071 + vec.y* 0.7071 + Output;

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
