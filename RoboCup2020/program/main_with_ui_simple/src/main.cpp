#include <mbed.h>
#include "config.h"


INLINE void setup(){
        pc.baud(230400);
        pc.printf("Boot!\r\n");
        pc.printf("\r\nHello world!! LEGEND\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        LCD.splash_lg();
        for (size_t i = 0; i < 8; i++) {rgb.on(i); wait_ms(30);}//チカチカさせる
        rgb.on(WHITE);
        btn.soundIndex(0); btn.soundIndex(1); btn.soundIndex(2);
        wait_ms(200); btn.soundIndex(3);
        getDx.attach_us(&getDxFromIMU,5000);
        //timer start
        pidDt.start();
        moveBackTimer.start();
        HAL_RCC_DeInit();   // If this is not implemented, HAL_RCC_OscConfig returns HAL_ERROR

        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_OscInitTypeDef RCC_OscInitStruct;

        __PWR_CLK_ENABLE();
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 8;
        RCC_OscInitStruct.PLL.PLLN = 336;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
        RCC_OscInitStruct.PLL.PLLQ = 7;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);

//         pidDirection.SetMode(AUTOMATIC);
//         pidDirection.SetOutputLimits(-70,70);//ちょうどいい
//         pidDirection.SetSampleTime(5);  //5
}

int main() {
        // put your setup code here, to run once:
        setup();
        while(1) {//メインループx
                modeRun();
                pc.printf("state:%d\r\n");
        }
}
