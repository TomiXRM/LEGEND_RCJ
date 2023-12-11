#ifndef LGIMU_H
#define LGIMU_H


class {

public:
        LGIMU(PinName rx,PinName inter);
        void frequency(int freq);
        void Setinterval(uint16_t us);
        short read();
private:
        Ticker ticker;
        DigitalOut device_call;
        RawSerial device;
        Timer timer;

        char highbyte;
        char lowbyte;
        short intdat;
        char c;
        uint8_t interval;
        bool isok;

        void callback();
        void blink();
};
#endif
