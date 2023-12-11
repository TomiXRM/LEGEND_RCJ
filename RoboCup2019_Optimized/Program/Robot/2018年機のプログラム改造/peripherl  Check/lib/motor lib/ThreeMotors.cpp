#include "ThreeMotors.h"
#include "mbed.h"

Motor::Motor(PinName INA1,PinName INB1,PinName PWM1,PinName INA2,PinName INB2,PinName PWM2,PinName INA3,PinName INB3,PinName PWM3) :
								_PWM1(PWM1),
								_INA1(INA1),
								_INB1(INB1),
								_PWM2(PWM2),
								_INA2(INA2),
								_INB2(INB2),
								_PWM3(PWM3),
								_INA3(INA3),
								_INB3(INB3){
								_PWM1.period(0.001); pwm1 = 0; _INA1 = 0; _INB1 = 0;
								_PWM2.period(0.001); pwm2 = 0; _INA2 = 0; _INB2 = 0;
								_PWM3.period(0.001); pwm3 = 0; _INA3 = 0; _INB3 = 0;
								M1SA=1;
								M2SA=1;
								M3SA=1;
}
void Motor::frequency(float freq = 0.001){
								_PWM1.period(freq);
								_PWM2.period(freq);
								_PWM3.period(freq);
}
void Motor::tweak(char par1,char par2,char par3){
								M1SA = par1/100;
								M2SA = par2/100;
								M3SA = par3/100;
}
int Motor::drive(int dig,float power,int HMCmot){
								dig *=-1;
								if (power == 0) {
																_INA1 = 1;
																_INB1 = 1;
																_PWM1 = 1;
																_INA2 = 1;
																_INB2 = 1;
																_PWM2 = 1;
																_INA3 = 1;
																_INB3 = 1;
																_PWM3 = 1;
								} else {
																dig -= 90;
																power /= 100;
																pwm1 = (HMCmot * 0.009) + (-sin((dig + 30) * PI / 180) * power * M1SA);
																pwm2 = (HMCmot * 0.009) + (sin((dig - 30) * PI / 180) * power * M2SA);
																pwm3 = (HMCmot * 0.009) + (sin((dig + 90) * PI / 180) * power * M3SA);
								}
								{
																if (pwm1 > 0) {
																								if (pwm1 < 0) {
																																pwm1 *= -1;
																								}
																								_INA1 = L;
																								_INB1 = H;

																								_PWM1 = pwm1 * M1SA;
																} else if (pwm1 < 0) {
																								if (pwm1 < 0) {
																																pwm1 *= -1;
																								}
																								_INA1 = H;
																								_INB1 = L;

																								_PWM1 = pwm1 * M1SA;
																} else {
																								_INA1 = 1;
																								_INB1 = 1;
																								_PWM1 = 1;
																}
								}
								{
																if (pwm2 > 0) {
																								if (pwm2 < 0) {
																																pwm2 *= -1;
																								}
																								_INA2 = L;
																								_INB2 = H;

																								_PWM2 = pwm2 * M2SA;
																} else if (pwm2 < 0) {
																								if (pwm2 < 0) {
																																pwm2 *= -1;
																								}
																								_INA2 = H;
																								_INB2 = L;

																								_PWM2 = pwm2 * M2SA;
																} else {
																								_INA2 = 1;
																								_INB2 = 1;
																								_PWM2 = 1;
																}
								}

								if (pwm3 > 0) {
																if (pwm3 < 0) {
																								pwm3 *= -1;
																}
																_INA3 = L;
																_INB3 = H;

																_PWM3 = pwm3 * M3SA;
								} else if (pwm3 < 0) {
																if (pwm3 < 0) {
																								pwm3 *= -1;
																}
																_INA3 = H;
																_INB3 = L;

																_PWM3 = pwm3 * M3SA;
								} else {
																_INA3 = 1;
																_INB3 = 1;
																_PWM3 = 1;
								}
								return dig;
}

void Motor::move(float pwm1,float pwm2,float pwm3){
								pwm1/=100;
								pwm2/=100;
								pwm3/=100;


								if (pwm1 > 0) {
																if (pwm1 < 0) {
																								pwm1 *= -1;
																}
																_INA1 = L;
																_INB1 = H;

																_PWM1 = pwm1 * M1SA;
								} else if (pwm1 < 0) {
																if (pwm1 < 0) {
																								pwm1 *= -1;
																}
																_INA1 = H;
																_INB1 = L;

																_PWM1 = pwm1 * M1SA;
								} else {
																_INA1 = 1;
																_INB1 = 1;
																_PWM1 = 1;
								}

								if (pwm2 > 0) {
																if (pwm2 < 0) {
																								pwm2 *= -1;
																}
																_INA2 = L;
																_INB2 = H;

																_PWM2 = pwm2 * M2SA;
								} else if (pwm2 < 0) {
																if (pwm2 < 0) {
																								pwm2 *= -1;
																}
																_INA2 = H;
																_INB2 = L;

																_PWM2 = pwm2 * M2SA;
								} else {
																_INA2 = 1;
																_INB2 = 1;
																_PWM2 = 1;
								}


								if (pwm3 > 0) {
																if (pwm3 < 0) {
																								pwm3 *= -1;
																}
																_INA3 = L;
																_INB3 = H;

																_PWM3 = pwm3 * M3SA;
								} else if (pwm3 < 0) {
																if (pwm3 < 0) {
																								pwm3 *= -1;
																}
																_INA3 = H;
																_INB3 = L;

																_PWM3 = pwm3 * M3SA;
								} else {
																_INA3 = 1;
																_INB3 = 1;
																_PWM3 = 1;
								}

}
