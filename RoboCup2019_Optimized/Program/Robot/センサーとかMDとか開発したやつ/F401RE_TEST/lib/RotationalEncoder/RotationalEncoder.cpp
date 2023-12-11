/**
 * rotational encoder Library
 */

#include "RotationalEncoder.h"

/**
 *
 */
RotationalEncoder::RotationalEncoder(PinName channelA,
                                     PinName channelB
                                    ) : _channelA(channelA), _channelB(channelB)
{

    _absolutePulse = 0;
    _relativePulse = 0;

    _channelA.mode(PullUp);
    _channelB.mode(PullUp);

    _channelBLevelOfchannelALow = INITAL;

    _channelA.rise(this, &RotationalEncoder::encoderAUp);
    _channelA.fall(this, &RotationalEncoder::encoderADwon);


}

void RotationalEncoder::reset(void)
{
    _absolutePulse = 0;
    _relativePulse = 0;
    getRelativePulses();
}

void RotationalEncoder::chenge(int16_t _chenge){
  _absolutePulse = _chenge;
}


int16_t RotationalEncoder::getAbsolutePulses(void)
{

    return _absolutePulse;

}

int16_t RotationalEncoder::getRelativePulses(void)
{
    static int16_t beforeAbsolutePulse = 0;
    int16_t temp = _absolutePulse;

    _relativePulse = temp - beforeAbsolutePulse;
    beforeAbsolutePulse = temp;

    return _relativePulse;

}


void RotationalEncoder::encoderAUp(void)
{
    if((_channelBLevelOfchannelALow == INITAL) ||
       (_channelBLevelOfchannelALow == _channelB.read())
       ) {
        return;
    }

    if(_channelB.read() == 0) {
        _absolutePulse++;

    } else {
        _absolutePulse--;

    }

}

void RotationalEncoder::encoderADwon(void)
{
    _channelBLevelOfchannelALow = _channelB.read();
}
