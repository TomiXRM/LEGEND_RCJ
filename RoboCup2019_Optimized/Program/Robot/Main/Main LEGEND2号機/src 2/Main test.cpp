#include "mbed.h"
#include "setup.h"


LGUI ui(&pc,PC_10,PC_11);

//プロトタイプ宣言
void Body_Main();
void After_Main();

void Body_Ball();

void Body_Line();

void Body_Commpus();

void Body_US();

void Body_Cam();

void Body_Debug();

void Before_Speed();
void Body_Speed();

void ReceiveSpeed();
void SetKICKER(bool i);


// References

void Body_Main(){
        ui.SetBZ(7);
        ui.putc(ui.state);
        pc.printf("Main Mode!!\n");
}


void After_Main(){
        ui.SetBZ(3);
        ui.putc(ui.state);
}


void Body_Ball(){
        IR.get();
        wait_ms(10);
        ui.putc('H');
        ui.putc(IR.data % 256);
        ui.putc(IR.data / 256);
        pc.printf("%d\t %d\t %d\n",IR.data,IR.dir,IR.dist);
}



void Body_Line(){
        pc.printf("Line Mode!!\n");
}



void Body_Compus(){
        pc.printf("Compass Mode!!\n");
}


void Body_US(){

        pc.printf("Us Mode!!\n");

}

void Body_Cam(){
        pc.printf("Cam Mode!!\n");
}

void Body_Debug(){
        pc.printf("Debug Mode!!\n");
}

void Before_Speed(){
        //Ready for speed mode

        ui.data_length = 0;
        ui.attachRx(&ReceiveSpeed);
        ui.putc(ui.speed);
        pc.printf("Mode detected");//DEBUG

}

void Body_Speed(){

        pc.printf("Speed Mode!!...%d\n",ui.speed);
}



void SetKICKER(bool i){
        bitWrite(ui.state, 6, i);
}





void ReceiveSpeed(){
        if(ui.data_length == 0)       {//モードの受信
                ui.speed =  ui.getc();
                ui.data_length += 1;
        }else if(ui.data_length == 1) {//スピードの受信
                ui.mode =  ui.getc();
                ui.data_length = 0; //リセット
        }
        pc.printf("Speed:%d\n",ui.speed);
}





void setup(){
        pc.baud(115200);
        ui.baud(BAUDRATE);

        //Mode enrties.
        ui.Entry(MAIN,DONOTHING,callback(&Body_Main),callback(&After_Main));
        ui.Entry(BALL,DONOTHING,callback(&Body_Ball),DONOTHING);
        ui.Entry(LINE,DONOTHING,callback(&Body_Line),DONOTHING);
        ui.Entry(COMPASS,DONOTHING,callback(&Body_Compus),DONOTHING);
        ui.Entry(US,DONOTHING,callback(&Body_US),DONOTHING);
        ui.Entry(CAM,DONOTHING,callback(&Body_Cam),DONOTHING);
        ui.Entry(DEBUG,DONOTHING,callback(&Body_Debug),DONOTHING);
        ui.Entry(SPEED,callback(&Before_Speed),callback(&Body_Speed),DONOTHING);

}


int main() {
        setup();

        pc.printf("\n\nHello world!! LEGEND US_module\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));

        while(1) {
                ui.Run();
        }
}
