#include "QEI.h"
QEI::QEI(const QEI& q) : channelA_(q.Pin[0]), channelB_(q.Pin[1]),index_(q.Pin[2])
{
        pulses_       = 0;
        revolutions_  = 0;
        pulsesPerRev_ = q.pulsesPerRev_;
        encoding_     = q.encoding_;
        timer=q.timer;
        //Workout what the current state is.
        int chanA = channelA_.read();
        int chanB = channelB_.read();
        /////////////////////////////////////////////
        channelA_.mode(PullUp);
        channelB_.mode(PullUp);
        /////////////////////////////////////////////

        //2-bit state.
        currState_ = (chanA << 1) | (chanB);
        prevState_ = currState_;

        channelA_.rise(this, &QEI::encode);
        channelA_.fall(this, &QEI::encode);


        if (q.encoding_ == X4_ENCODING) {
                channelB_.rise(this, &QEI::encode);
                channelB_.fall(this, &QEI::encode);
        }
        if (Pin[2] !=  NC) {
                index_.rise(this, &QEI::index);
        }
}
QEI::QEI(PinName channelA,
         PinName channelB,
         PinName index,
         int pulsesPerRev,
         Timer *T,
         Encoding encoding
         ) : channelA_(channelA), channelB_(channelB),
        index_(index)
{
        timer=T;
        Pin[0] = channelA;
        Pin[1] = channelB;
        Pin[2] = index;
        pulses_       = 0;
        revolutions_  = 0;
        pulsesPerRev_ = pulsesPerRev;
        encoding_     = encoding;

        //Workout what the current state is.
        int chanA = channelA_.read();
        int chanB = channelB_.read();
/////////////////////////////////////////////
        channelA_.mode(PullUp);
        channelB_.mode(PullUp);
/////////////////////////////////////////////

        //2-bit state.
        currState_ = (chanA << 1) | (chanB);
        prevState_ = currState_;

        channelA_.rise(this, &QEI::encode);
        channelA_.fall(this, &QEI::encode);


        if (encoding == X4_ENCODING) {
                channelB_.rise(this, &QEI::encode);
                channelB_.fall(this, &QEI::encode);
        }
        if (index !=  NC) {
                index_.rise(this, &QEI::index);
        }

}
void QEI::state(int i)
{
        if(i==1) {
                channelA_.disable_irq();
                channelB_.disable_irq();
        } else if(i==0) {
                channelA_.enable_irq();
                channelB_.enable_irq();
        }
}
void QEI::qei_reset(void)
{

        pulses_      = 0;
        revolutions_ = 0;
        round_rev = 0;
        sumangle = angle_ =0;
}
void QEI::set(int pul, int rev)
{

        pulses_      = pul;
        revolutions_ = rev;

}
int QEI::getCurrentState(void)
{

        return currState_;

}

int QEI::getPulses(void)
{

        return pulses_;

}

int QEI::getRevolutions(void)
{

        return revolutions_;

}
double QEI::getAngle()
{
        return angle_;
}
int QEI::getAng_rev()
{
        return round_rev;
}
double QEI::getSumangle()
{
        return sumangle;
}

double QEI::getRPM()
{
        static double prev_angle;

        RPM = (sumangle - prev_angle) / gettime() * 60.0 / 360;

        prev_angle = sumangle;
        return RPM;
}
double QEI::getRPS()
{
        static double prev_angle;

        RPS = (sumangle - prev_angle) / gettime() / 360;
        prev_angle = sumangle;
        return RPS;
}
double QEI::getRPMS()
{
        static double prev_angle;

        RPMS = (sumangle - prev_angle) / gettime()*1000/ 360;
        prev_angle = sumangle;
        return RPMS;
}
double QEI::getRPUS()
{
        static double prev_angle;
        RPUS =(sumangle - prev_angle) / gettime()*1000*1000/ 360;
        prev_angle = sumangle;
        prev_angle = sumangle;
        return RPUS;
}
void QEI::encode(void)
{
        int change = 0;
        int chanA  = channelA_.read();
        int chanB  = channelB_.read();
        //printf("QEI\n");
        currState_ = (chanA << 1) | (chanB);

        if (encoding_ == X2_ENCODING) {

                if ((prevState_ == 0x3 && currState_ == 0x0) ||
                    (prevState_ == 0x0 && currState_ == 0x3)) {

                        pulses_++;
                        angle_pulses++;

                } else if ((prevState_ == 0x2 && currState_ == 0x1) ||
                           (prevState_ == 0x1 && currState_ == 0x2)) {

                        pulses_--;
                        angle_pulses--;

                }

        } else if (encoding_ == X4_ENCODING) {

                if (((currState_ ^ prevState_) != INVALID) && (currState_ != prevState_)) {
                        change = (prevState_ & PREV_MASK) ^ ((currState_ & CURR_MASK) >> 1);

                        if (change == 0) {
                                change = -1;
                        }

                        pulses_ -= change;
                        angle_pulses -= change;
                }

        }
        angle_ = angle_pulses*360/((double)pulsesPerRev_*4);
        sumangle = pulses_*360/((double)pulsesPerRev_*4);
        if(angle_>=360) {
                angle_pulses = angle_ = 0;
                round_rev++;
        } else if(angle_<=-360) {
                angle_pulses = angle_ = 0;
                round_rev--;
        }
        prevState_ = currState_;
}

void QEI::index(void)
{

        revolutions_++;

}
