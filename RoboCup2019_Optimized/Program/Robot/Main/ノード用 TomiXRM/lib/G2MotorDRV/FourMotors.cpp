#include "FourMotors.h"
#include "mbed.h"

Motor::Motor(PinName MD1,PinName PWM1,PinName MD2,PinName PWM2,PinName MD3,PinName PWM3,PinName MD4,PinName PWM4) :
        _PWM1(PWM1),_MD1(MD1),_PWM2(PWM2),_MD2(MD2),_PWM3(PWM3),_MD3(MD3),_PWM4(PWM4),_MD4(MD4){
        _PWM1.period(0.001); pwm1 = 0; _MD1 = 0;
        _PWM2.period(0.001); pwm2 = 0; _MD2 = 0;
        _PWM3.period(0.001); pwm3 = 0; _MD3 = 0;
        _PWM4.period(0.001); pwm4 = 0; _MD4 = 0;
        M1SA=1;
        M2SA=1;
        M3SA=1;
        M4SA=1;
}
void Motor::frequency(float freq = 0.001){
        _PWM1.period(freq);
        _PWM2.period(freq);
        _PWM3.period(freq);
        _PWM4.period(freq);
}
void Motor::tweak(float par1,float par2,float par3,float par4){
        M1SA = par1/100;
        M2SA = par2/100;
        M3SA = par3/100;
        M4SA = par4/100;
}
int Motor::drive(int dig,float power,int HMCmot){
        dig =-dig;
        if (power == 0) {
                _MD1 = 1;
                _MD2 = 1;
                _MD3 = 0;
                _MD4 = 0;
                _PWM1 = 0;
                _PWM2 = 0;
                _PWM3 = 0;
                _PWM4 = 0;
        } else {
                /*すべて右回転と仮定する
                      M2    M1

                      M3    M4
                   逆時計周り
                 */
                dig += 90;
                power /= 100;
                pwm1 = (HMCmot * 0.009)+(sin((dig + 30) * PI / 180) * power * M1SA);
                pwm2 = (HMCmot * 0.009)+(sin((dig + 150) * PI / 180) * power * M2SA);
                pwm3 = (HMCmot * 0.009)+(sin((dig - 135) * PI / 180) * power * M3SA);
                pwm4 = (HMCmot * 0.009)+(sin((dig - 45) * PI / 180) * power * M4SA);
        }
        {
                if (pwm1 > 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 1;

                } else if (pwm1 < 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 0;

                } else {
                        _PWM1 = 0;
                        _MD1 = 0;
                }
        }
        {
                if (pwm2 > 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 1;

                } else if (pwm2 < 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 0;

                } else {
                        _PWM2 = 0;
                        _MD2 = 0;
                }
        }
        {
                if (pwm4 > 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 1;

                } else if (pwm4 < 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 0;

                } else {
                        _PWM4 = 0;
                        _MD4 = 0;
                }
        }

        {
                if (pwm3 > 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 1;

                } else if (pwm3 < 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 0;

                } else {
                        _PWM3 = 0;
                        _MD3 = 0;
                }
        }

        return dig;
}
int Motor::drive(int dig,float power){
        dig =-dig;
        if (power == 0) {
                _MD1 = 1;
                _MD2 = 1;
                _MD3 = 0;
                _MD4 = 0;
                _PWM1 = 0;
                _PWM2 = 0;
                _PWM3 = 0;
                _PWM4 = 0;
        } else {
                /*すべて右回転と仮定する
                      M2    M1

                      M3    M4
                   逆時計周り
                 */
                dig += 90;
                power /= 100;
                pwm1 = (sin((dig + 30) * PI / 180) * power * M1SA);
                pwm2 = (sin((dig + 150) * PI / 180) * power * M2SA);
                pwm3 = (sin((dig - 135) * PI / 180) * power * M3SA);
                pwm4 = (sin((dig - 45) * PI / 180) * power * M4SA);
        }
        {
                if (pwm1 > 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 1;

                } else if (pwm1 < 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 0;

                } else {
                        _PWM1 = 0;
                        _MD1 = 0;
                }
        }
        {
                if (pwm2 > 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 1;

                } else if (pwm2 < 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 0;

                } else {
                        _PWM2 = 0;
                        _MD2 = 0;
                }
        }

        {
                if (pwm4 > 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 1;

                } else if (pwm4 < 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 0;

                } else {
                        _PWM4 = 0;
                        _MD4 = 0;
                }
        }
        {
                if (pwm3 > 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 1;

                } else if (pwm3 < 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 0;

                } else {
                        _PWM3 = 0;
                        _MD3 = 0;
                }
        }
        return dig;
}
void Motor::move(float pwm1,float pwm2,float pwm3,float pwm4){
        pwm1/=100;
        pwm2/=100;
        pwm3/=100;
        pwm4/=100;


        {
                if (pwm1 > 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 1;

                } else if (pwm1 < 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 0;

                } else {
                        _PWM1 = 0;
                        _MD1 = 0;
                }
        }
        {
                if (pwm2 > 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 1;

                } else if (pwm2 < 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 0;

                } else {
                        _PWM2 = 0;
                        _MD2 = 0;
                }
        }
        {
                if (pwm3 > 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 1;

                } else if (pwm3 < 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 0;

                } else {
                        _PWM3 = 0;
                        _MD3 = 0;
                }
        }
        {
                if (pwm4 > 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 1;

                } else if (pwm4 < 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 0;

                } else {
                        _PWM4 = 0;
                        _MD4 = 0;
                }
        }

}
void Motor::move(float pwm1,float pwm2,float pwm3,float pwm4,int HMCmot){
        pwm1/=100;
        pwm2/=100;
        pwm3/=100;
        pwm4/=100;
        pwm1 += (HMCmot*0.009);
        pwm2 += (HMCmot*0.009);
        pwm3 += (HMCmot*0.009);
        pwm4 += (HMCmot*0.009);

        {
                if (pwm1 > 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 1;

                } else if (pwm1 < 0) {
                        _PWM1 = abs(pwm1);
                        _MD1 = 0;

                } else {
                        _PWM1 = 0;
                        _MD1 = 0;
                }
        }
        {
                if (pwm2 > 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 1;

                } else if (pwm2 < 0) {
                        _PWM2 = abs(pwm2);
                        _MD2 = 0;

                } else {
                        _PWM2 = 0;
                        _MD2 = 0;
                }
        }
        {
                if (pwm3 > 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 1;

                } else if (pwm3 < 0) {
                        _PWM3 = abs(pwm3);
                        _MD3 = 0;

                } else {
                        _PWM3 = 0;
                        _MD3 = 0;
                }
        }
        {
                if (pwm4 > 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 1;

                } else if (pwm4 < 0) {
                        _PWM4 = abs(pwm4);
                        _MD4 = 0;

                } else {
                        _PWM4 = 0;
                        _MD4 = 0;
                }
        }

}

void Motor::driveVector(VectorXY vec,float Output){
        float M1 = vec.y* 0.866 + vec.x * -0.5 + Output*2;
        float M2 = vec.y* -0.866 + vec.x * -0.5 + Output*2;
        float M3  = vec.y * -0.707 + vec.x * 0.707 + Output*2;
        float M4 = vec.y * 0.707 +vec.x * 0.707 + Output*2;
        move(M1, M2, M3, M4);
}

void Motor::stop(bool i){
        _PWM1 = 0;
        _PWM2 = 0;
        _PWM3 = 0;
        _PWM4 = 0;

}
