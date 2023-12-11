


LGHCSR04::LGHCSR04(PinName PIN)
        : _event(Echo_PIN),_pin(PIN), _timer()
{
        timeout = 1000;
        _SPEED_OF_SOUND_CM = 33;
}


int LGHCSR04::read_cm(){
        _pin.output();
        _pin.write(0);
        wait_us(3);
        _pin.write(1);
        wait_us(3);
        _pin.write(0);
        _Time = LGHCSR04::read_high_us();
        return ((_Time*_SPEED_OF_SOUND_CM)/1000/2);
}

void LGHCSR04::SetTimeout(){
        timeout = _timeout;
}

int LGHCSR04::read_high_us(){
        _timer.reset();
        _timer.start();
        pulsetime.reset();
        _pin.input();
        while (_pin == 1) {
                if (_timer.read_us() > timeout) return -1;
        }
        while (_pin == 0) {
                if (_timer.read_us() > timeout) return -1;
        }
        pulsetime.start();
        while (_pin == 1) {
                if (_timer.read_us() > timeout) return -1;
        }
        pulsetime.stop();
        return pulsetime.read_us();
}
