#ifndef _SETUP_HPP_
#define _SETUP_HPP_



#define INLINE inline __attribute__((always_inline))
#define PI 3.14159265358979323846264338327


#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))


//------------------------------------------------------------------------------
//三角関数表
#define sin0_0    0
#define cos0_0    1
#define sin22_5   0.3827
#define cos22_5   0.9239
#define sin45_0   0.7071
#define cos45_0   0.7071
#define sin67_5   0.9239
#define cos67_5   0.3827
#define sin90_0   1
#define cos90_0   0
#define sin112_5  0.9239
#define cos112_5 -0.3827
#define sin135_0  0.7071
#define cos135_0  -0.7071
#define sin157_5  0.3827
#define cos157_5 -0.9239
#define sin180_0  0
#define cos180_0  -1

#define sin_22_5  -0.3827
#define cos_22_5   0.9239
#define sin_45_0  -0.7071
#define cos_45_0   0.7071
#define sin_67_5  -0.9239
#define cos_67_5   0.3827
#define sin_90_0  -1
#define cos_90_0   0
#define sin_112_5 -0.9239
#define cos_112_5 -0.3827
#define sin_135_0 -0.7071
#define cos_135_0 -0.7071
#define sin_157_5 -0.3827
#define cos_157_5 -0.9239
//------------------------------------------------------------------------------
//めちゃくちゃにします(XY反転)
//XにsinかけてYにcosかける脳筋法
#define X00   IR_PulseWidth[0]*sin0_0
#define Y00   IR_PulseWidth[0]*cos0_0
#define X_225  IR_PulseWidth[1]*sin_22_5
#define Y_225  IR_PulseWidth[1]*cos_22_5
#define X_450  IR_PulseWidth[2]*sin_45_0
#define Y_450  IR_PulseWidth[2]*cos_45_0
#define X_675  IR_PulseWidth[3]*sin_67_5
#define Y_675  IR_PulseWidth[3]*cos_67_5
#define X_900  IR_PulseWidth[4]*sin_90_0
#define Y_900  IR_PulseWidth[4]*cos_90_0
#define X_1125 IR_PulseWidth[5]*sin_112_5
#define Y_1125 IR_PulseWidth[5]*cos_112_5
#define X_1350 IR_PulseWidth[6]*sin_135_0
#define Y_1350 IR_PulseWidth[6]*cos_135_0
#define X_1575 IR_PulseWidth[7]*sin_157_5
#define Y_1575 IR_PulseWidth[7]*cos_157_5
#define X1800 IR_PulseWidth[8]*sin180_0
#define Y1800 IR_PulseWidth[8]*cos180_0

#define X225  IR_PulseWidth[15]*sin22_5
#define Y225  IR_PulseWidth[15]*cos22_5
#define X450  IR_PulseWidth[14]*sin45_0
#define Y450  IR_PulseWidth[14]*cos45_0
#define X675  IR_PulseWidth[13]*sin67_5
#define Y675  IR_PulseWidth[13]*cos67_5
#define X900  IR_PulseWidth[12]*sin90_0
#define Y900  IR_PulseWidth[12]*cos90_0
#define X1125 IR_PulseWidth[11]*sin112_5
#define Y1125 IR_PulseWidth[11]*cos112_5
#define X1350 IR_PulseWidth[10]*sin135_0
#define Y1350 IR_PulseWidth[10]*cos135_0
#define X1575 IR_PulseWidth[9]*sin157_5
#define Y1575 IR_PulseWidth[9]*cos157_5




#endif
