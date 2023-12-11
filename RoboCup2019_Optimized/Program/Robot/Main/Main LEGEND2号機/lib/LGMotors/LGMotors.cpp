#include "LGMotors.h"
#include "mbed.h"
#define PI 3.14159265358979323846264338327


//NOTE 11/16 ComSLPはなぜか使えません。とりあえず今はcom_slp無視してください。11/16 By 祐聖
//NOTE 12/3  ComSLP使えちゃいましたあはは
//[参照]2018/11/15 モータまわすぜい @ゆうせいのレポート(と言う名のなにか).pages

//------------------------------------------------------------------------------------
Motors::Motors(PinName _pwm1,PinName _pwm2,PinName _pwm3,PinName _pwm4,PinName _comSleep) : m1(_pwm1),m2(_pwm2),m3(_pwm3),m4(_pwm4),slp(_comSleep){
        period_us(10);//50
        stopAll();
        Deg_m1 = 45;
        Deg_m2 = 135;
        Deg_m3 = -135;
        Deg_m4 = -45;
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
        m1=((p1/2)+50)/100;
        m2=((p2/2)+50)/100;
        m3=((p3/2)+50)/100;
        m4=((p4/2)+50)/100;
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
        deg =-deg;
        /*すべて右回転と仮定する
              m2    m1

              m3    m4
           反時計周り
         */
        deg += 90;
        // power /= 100;

        /*p1 =((sin((deg + Deg_m1) * PI / 180) * power)/2)+0.5+((degree/2)+50)/100;
           p2 =((sin((deg + Deg_m2) * PI / 180) * power)/2)+0.5+((degree/2)+50)/100;
           p3 =((sin((deg + Deg_m3) * PI / 180) * power)/2)+0.5+((degree/2)+50)/100;
           p4 =((sin((deg + Deg_m4) * PI / 180) * power)/2)+0.5+((degree/2)+50)/100;*/

        p1 = (sin((deg + Deg_m1) * PI / 180) * power)+degree;
        p2 = (sin((deg + Deg_m2) * PI / 180) * power)+degree;
        p3 = (sin((deg + Deg_m3) * PI / 180) * power)+degree;
        p4 = (sin((deg + Deg_m4) * PI / 180) * power)+degree;
        run(p1,p2,p3,p4);

        return deg;
}


void Motors::driveVector(VectorXY vec,float Output){
        float M1 = vec.y* 0.866 + vec.x * -0.5 + Output;
        float M2 = vec.y* -0.866 + vec.x * -0.5 + Output;
        float M3  = vec.y * -0.707 + vec.x * 0.707 + Output;
        float M4 = vec.y * 0.707 +vec.x * 0.707 + Output;
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
