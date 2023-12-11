#ifndef __MOVING_AVERAGE__
#define __MOVING_AVERAGE__

class MovingAverage {
private:
float *dataArray;
uint16_t sizeOfDataArray;
uint16_t count;
public:
MovingAverage(uint16_t len_array);
float updateData(float value);
void reset(void);

float calc();
};



MovingAverage::MovingAverage(uint16_t len_array) {
        dataArray = new float[len_array];
        sizeOfDataArray = len_array;
        count = 0;
}

float MovingAverage::updateData(float value) {
        count++;
        if (count >= sizeOfDataArray) count = 0;
        dataArray[count] = value;


        return calc();
}

void MovingAverage::reset(void) {
        for (int i = 0; i < sizeOfDataArray; ++i) {
                dataArray[i] = 0;
        }
}


float MovingAverage::calc(){

        uint8_t i=0;
        int sum=0;

        for(i=0; i< sizeOfDataArray; i++) {
                sum=sum+(dataArray[i]);

                //pc.printf("Val[%d]=%d\n",i,dataArray[i]);
        }

        float avr=sum/sizeOfDataArray;

        float s2=0;//分散

        for(i=0; i<sizeOfDataArray; i++) {
                s2+=pow(avr-(dataArray[i]),2);
        }

        s2=s2/sizeOfDataArray;

        float s=sqrt(s2);//標準偏差

        uint32_t allowed_max=avr+s;//許容される最大値
        uint32_t allowed_min=avr-s;//許容される最小値

        uint32_t tmp[sizeOfDataArray];

        for(i=0; i<sizeOfDataArray; i++) {

                if(dataArray[i]<allowed_max&&dataArray[i]>allowed_min) {
                        tmp[i]=dataArray[i];
                }else{
                        tmp[i]=0xFF;
                }

        }

        sum=0;

        uint8_t skip=0;

        for(i=0; i< sizeOfDataArray; i++) {

                if(tmp[i]!=0xFF) {
                        sum+=tmp[i];
                }else{

                        skip++;
                }
        }

        return sum/(sizeOfDataArray-skip);

}

#endif
