#ifndef __MOVING_AVERAGE__
#define __MOVING_AVERAGE__

class MovingAverage {
private:
    float *dataArray;
    uint16_t sizeOfDataArray;
    uint16_t count;
public:
    MovingAverage(uint16_t num_array);
    float updateData(float value);
    void reset(void);
};

MovingAverage::MovingAverage(uint16_t num_array) {
    dataArray = new float[num_array];
    sizeOfDataArray = num_array;
    count = 0;
}

float MovingAverage::updateData(float value) {
    count++;
    if (count >= sizeOfDataArray) count = 0;
    dataArray[count] = value;
 
    float rslt = 0;
    for (int i = 0; i < sizeOfDataArray; ++i) {
        rslt += dataArray[i];
    }
    
    return rslt / sizeOfDataArray;
}

void MovingAverage::reset(void) {
    for (int i = 0; i < sizeOfDataArray; ++i) {
        dataArray[i] = 0;
    }
}

#endif
