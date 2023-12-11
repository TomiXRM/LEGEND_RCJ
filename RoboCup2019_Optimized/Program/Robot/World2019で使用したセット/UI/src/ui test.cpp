#include "mbed.h"
#include "setup.h"
#define  BAUDRATE 1000000
#define BUZZERSTATE 1

//CamMode
#define SETPOSITION     0//中立点決め
#define GOTOHOME        1//ホームポジションに戻れるかのチェック
#define FACINGOPSGOAL   2//相手ゴールを向くことができるかのチェック
#define POSITIONMONITOR 3//ポジションモニター(座標)

// Timer timer;

uint8_t mainstate;//ledとかキッカーとかブザーとか
bool r,g,b;
uint8_t buzzer;
int speed;
bool KickFlag;

int8_t camSetupMode;//カメラモードの中のモード
int8_t motorModeMode;
int8_t EachMotorModeSelect;
int8_t m_1,m_2,m_3,m_4;
int8_t SetPosiMenu;
uint8_t balldist;
short balldir;
char ballhead;
uint8_t data_length_ball;
uint8_t ballhighbyte;
uint8_t balllowbyte;
short balldata;
uint8_t ballx;
uint8_t bally;
uint8_t ballr;




INLINE void MainModeProgram();
INLINE void BallModeProgram();
INLINE void LineModeProgram();
INLINE void CompassModeProgram();
INLINE void USModeProgram();
INLINE void CamModeProgram();
INLINE void DebugModeProgram();
INLINE void SpeedModeProgram();
INLINE void MotorModeProgram();

void ReadMainState();
void ReadBallState();
void ReceiveSpeed();
void bargraph(int r);

INLINE void MainModeProgram(){
        if (UI.Start) {
                LED.Off();
                Main.attach(&ReadMainState,Serial::RxIrq);//割り込み設定をしてから
                Main.putc(MAIN);                          //モードデータを送信
                LCD.clearDisplay(); LCD.display();
                wait_ms(200); SW.SetSw1(1);//チャタリング対策
                while(SW.GetSw1State() == 1) {
                        LED.On(r,g,b);
                        SP.sound(buzzer);//0〜6で音がなる。7はブザーオフ
                        if(KickFlag == 1) {
                                Kicker.Kick();
                                //SP.sound(3);
                                led1 = 1;
                        }else{
                                led1 = 0;
                        }
                        pc.printf("MainMode MOVING...State:%d\ttimer:%d\r\n",mainstate,Kicker.timer.read_ms());
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(START_MODE);
        }else{
                LED.On(WHITE);
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,4,64),10);
                LCD.printf("Main"); LCD.display();
        }
}

INLINE void BallModeProgram(){
        LED.On(RED);
        if (UI.Start) {
                Main.attach(&ReadBallState,Serial::RxIrq);//割り込み設定をしてからメインにモードを送る
                Main.putc(BALL);//メインにモードを送信
                while(SW.GetSw1State() == 1) {

                        LCD.clearDisplay();
                        LCD.drawCircle(32, 32, 31, WHITE);
                        LCD.drawCircle(32, 32, 24, WHITE);
                        LCD.drawCircle(32, 32, 16, WHITE);
                        LCD.drawLine(0, 32, 64, 32, WHITE);
                        LCD.drawLine(32, 0, 32, 64, WHITE);
                        LCD.fillCircle(ballx,bally,4,WHITE);
                        LCD.display();
                        pc.printf("BallMode MOVING...%d\r\n",balldata);
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(BALL_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,4,64),10);
                LCD.printf("Ball"); LCD.display();
        }

}
INLINE void LineModeProgram(){
        LED.On(GREEN);
        if (UI.Start) {
                Main.putc(LINE);
                while(SW.GetSw1State() == 1) {
                        LCD.clearDisplay();
                        LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,8,64),10);
                        LCD.printf("LineMoni"); LCD.display();
                        pc.printf("LineMode MOVING...\r\n");
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(LINE_MODE);

        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,4,64),10);
                LCD.printf("Line"); LCD.display();
        }

}
INLINE void CompassModeProgram(){
        LED.On(BLUE);
        if (UI.Start) {
                Main.putc(COMPASS);
                wait_ms(10);
                SP.sound(4);                //0〜6で音がなる。7はブザーオフ
                while(SW.GetSw1State() == 1) {
                        pc.printf("CompassMode MOVING...\r\n");
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(COMPASS_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,7,64),10);
                LCD.printf("Compass"); LCD.display();
        }

}
INLINE void USModeProgram(){
        LED.On(MAGENTA);
        if (UI.Start) {
                Main.putc(US);
                while(SW.GetSw1State() == 1) {
                        pc.printf("UsMode MOVING...\r\n");
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(US_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,2,64),10);
                LCD.printf("US"); LCD.display();
        }

}
INLINE void CamModeProgram(){
        LED.On(YELLOW);
        if (UI.Start) {
                Main.putc(CAM);
                /*while(SW.GetSw1State() == 1) {
                        LCD.clearDisplay();
                        LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,9,64),10);
                        LCD.printf("Cam setup"); LCD.display();
                        pc.printf("CamMode MOVING...\r\n");
                   }*/
                SW.SetSw1(0); //0に戻し、モード選択に移り変わる
                SW.SetMode(0);//0に戻し、モード選択に移り変わる
                wait_ms(10);//デットタイム
                while (SW.GetSw1State() == 0) {
                        SW.ReadLongPusshPlus(); SW.ReadLongPusshMinus();//長押し判定
                        camSetupMode = SW.GetMode();//モード読み取り
                        if(camSetupMode < 0) {
                                camSetupMode = 4;
                                SW.SetMode(4);
                        }else if(camSetupMode > 4) {
                                camSetupMode = 0;
                                SW.SetMode(0);
                        }
                        switch (camSetupMode) {
                        case SETPOSITION:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,7,64),10);
                                LCD.printf("SetPosi"); LCD.display();
                                break;
                        case GOTOHOME:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                                LCD.printf("GoHome"); LCD.display();
                                break;
                        case FACINGOPSGOAL:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,10,64),10);
                                LCD.printf("FacingGoal"); LCD.display();
                                break;
                        case POSITIONMONITOR:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,8,64),10);
                                LCD.printf("PosiMoni"); LCD.display();
                                break;
                        case 4:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                                LCD.printf("EXIT!!"); LCD.display();
                                break;
                        }
                }
                switch (camSetupMode) {
                case SETPOSITION:
                        Main.putc('a');
                        SW.SetSw1(0); //0に戻し、モード選択に移り変わる
                        SW.SetMode(1);//0に戻し、モード選択に移り変わる
                        SetPosiMenu = 1;
                        while (SetPosiMenu != 6) {
                                SW.SetSw1(1);
                                while(SW.GetSw1State() == 1) {
                                        SW.ReadLongPusshPlus(); SW.ReadLongPusshMinus();//長押し判定
                                        SetPosiMenu = SW.GetMode();//モード読み取り
                                        if(SetPosiMenu < 1) {
                                                SetPosiMenu = 6;
                                                SW.SetMode(6);
                                        }else if(SetPosiMenu > 6) {
                                                SetPosiMenu = 1;
                                                SW.SetMode(1);
                                        }

                                        switch (SetPosiMenu) {
                                        case 1:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,8,64),10);
                                                LCD.printf("Opposite");
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),26);
                                                LCD.printf("Right");
                                                LCD.display();
                                                break;
                                        case 2:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,8,64),10);
                                                LCD.printf("Opposite");
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,4,64),26);
                                                LCD.printf("Left");
                                                LCD.display(); break;
                                        case 3:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                                                LCD.printf("Center");
                                                LCD.display(); break;
                                        case 4:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,3,64),10);
                                                LCD.printf("Own");
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),26);
                                                LCD.printf("Right");
                                                LCD.display(); break;
                                        case 5:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,3,64),10);
                                                LCD.printf("Own");
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,4,64),26);
                                                LCD.printf("Left");
                                                LCD.display(); break;
                                        case 6:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(3);  LCD.setTextCursor(CenterX(3,5,64),CenterY(3,32));
                                                LCD.printf("EXIT!");
                                                LCD.display(); break;
                                        }
                                        pc.printf("SetPosi:%d\r\n",SetPosiMenu);
                                }
                                if(SetPosiMenu != 6) {SP.sound(5);}
                                Main.putc((uint8_t)(SetPosiMenu));
                        }

                        break;
                case GOTOHOME:
                        Main.putc('b');
                        while(SW.GetSw1State() == 1) {
                                pc.printf("HomingProgram MOVING...\r\n");
                        } break;
                case FACINGOPSGOAL:
                        Main.putc('c');
                        while(SW.GetSw1State() == 1) {
                                pc.printf("FacingProgram MOVING...\r\n");
                        } break;
                case POSITIONMONITOR:
                        Main.putc('d');
                        while(SW.GetSw1State() == 1) {
                                pc.printf("PosiMoni MOVING...\r\n");
                        } break;
                case 4: Main.putc(MODESW);
                        break;//exit
                }
                wait_ms(3);//デットタイム
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(CAM_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,3,64),10);
                LCD.printf("Cam"); LCD.display();
        }

}
INLINE void DebugModeProgram(){
        LED.On(CYAAN);
        if (UI.Start) {
                Main.putc(DEBUG);
                while(SW.GetSw1State() == 1) {
                        pc.printf("DebugMode MOVING...\r\n");
                }
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(DEBUG_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),10);
                LCD.printf("Debug"); LCD.display();
        }

}


INLINE void SpeedModeProgram(){
        if (UI.Start) {
                speed = 1;//初期値
                Main.putc(SPEED);//モードを送信
                Main.attach(&ReceiveSpeed,Serial::RxIrq);//割り込み関数の切り替え
                ///wait_ms(50);//メイン側の割り込み関数切り替えのためのデットタイム
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),CenterY(2,25));
                LCD.printf("speed"); LCD.display();
                SW.SetMode(speed);
                while(SW.GetSw1State() == 1) {
                        SW.ReadLongPusshPlus(); SW.ReadLongPusshMinus();//長押し判定
                        speed = SW.GetMode();
                        if(speed < 1) {
                                SW.SetMode(1);
                                SP.sound(2);
                                LED.On(BLUE);
                        }else if(speed <= 100&&speed >=80) {
                                LED.On(GREEN);
                        }else if(speed < 80) {
                                LED.On(CYAAN);
                        }else if(speed > 160) {
                                SW.SetMode(160);
                                SP.sound(2);
                                LED.On(RED);
                        }else{
                                LED.On(MAGENTA);
                        }

                        if(speed < 10) {
                                LCD.clearDisplay();
                                LCD.setTextSize(3);  LCD.setTextCursor(CenterX(3,1,64),CenterY(3,33));
                                bargraph(speed*1.28);
                                LCD.printf("%d",speed); LCD.display();
                        }else if(speed < 100) {
                                LCD.clearDisplay();
                                LCD.setTextSize(3);  LCD.setTextCursor(CenterX(3,2,64),CenterY(3,33));
                                bargraph(speed*1.28);
                                LCD.printf("%d",speed); LCD.display();
                        }else{
                                LCD.clearDisplay();
                                LCD.setTextSize(3);  LCD.setTextCursor(CenterX(3,3,64),CenterY(3,33));
                                bargraph(speed*1.28);
                                LCD.printf("%d",speed); LCD.display();
                        }
                        pc.printf("SpeedMode MOVING...%d\r\n",speed);
                }
                SW.SetSw1(0);
                SW.SetMode(SPEED_MODE);
                Main.putc(speed); wait_ms(10);
                Main.putc(MODESW);
        }else{
                if(speed < 1) {
                        LED.On(BLUE);
                }else if(speed <= 100&&speed >=80) {
                        LED.On(GREEN);
                }else if(speed < 80) {
                        LED.On(CYAAN);
                }else if(speed > 160) {
                        LED.On(RED);
                }else{
                        LED.On(MAGENTA);
                }
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),10);
                LCD.printf("Speed"); LCD.display();
        }
}

INLINE void KickerModeProgram(){
        if(UI.Start) {
                ///Kicker.Kicker = 1;
                Kicker.Kick();
                SW.SetSw1(0);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                LCD.printf("Kicker"); LCD.display();
                LED.On(MAGENTA);
        }
}
void ReadMainState(){//メインからくるキッカーやLEDの情報を読み取る関数
        mainstate = (uint8_t)Main.getc();
        r = bitRead(mainstate, 0);
        g = bitRead(mainstate, 1);
        b = bitRead(mainstate, 2);

        buzzer = (mainstate>>5);

        KickFlag = bitRead(mainstate, 4);
}
void ReadBallState(){//メインから来るボールの状態
        if(data_length_ball == 0)       {//ヘッダ（H）の受信
                ballhead =  Main.getc();
                if(ballhead == 'H') {
                        data_length_ball += 1;
                }else{
                        data_length_ball = 0;
                }
        }else if(data_length_ball == 1) {//上バイト受信
                balllowbyte = Main.getc();
                data_length_ball += 1;
        }else if(data_length_ball == 2) {//下バイト受信
                ballhighbyte  =  Main.getc();
                data_length_ball = 0; //リセット
                balldata =  ballhighbyte*256+ balllowbyte; //復元
                if(balldata == 6000) {
                        balldist = 0;
                        balldir = 0;
                }else if(balldata >= 5000) {
                        balldist = 30;
                        balldir = balldata-5000;
                }else if(balldata >= 4000) {
                        balldist = 24;
                        balldir = balldata-4000;
                }else if(balldata >= 3000) {
                        balldist = 18;
                        balldir = balldata-3000;
                }else if(balldata >= 2000) {
                        balldist = 12;
                        balldir = balldata-2000;
                }else if(balldata >= 1000) {
                        balldist = 6;
                        balldir = balldata-1000;
                }
                bally = (short)(balldist * -cos(balldir / 180.0 * PI)+32);
                ballx = (short)(balldist * sin(balldir / 180.0 * PI)+32);

        }

}

INLINE void MotorModeProgram(){
        LED.On(BLUE);
        if (UI.Start) {
                Main.putc(MOTOR);
                SW.SetSw1(0); //0に戻し、モード選択に移り変わる
                SW.SetMode(0);//0に戻し、モード選択に移り変わる
                wait_ms(10);//デットタイム
                while (SW.GetSw1State() == 0) {
                        SW.ReadLongPusshPlus(); SW.ReadLongPusshMinus();//長押し判定
                        motorModeMode = SW.GetMode();//モード読み取り
                        if(motorModeMode < 0) {
                                motorModeMode = 2;
                                SW.SetMode(2);
                        }else if(motorModeMode > 2) {
                                motorModeMode = 0;
                                SW.SetMode(0);
                        }
                        switch (motorModeMode) {
                        case 0:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,8,64),10);
                                LCD.printf("EachMoto"); LCD.display();
                                pc.printf("Each motor\r\n");
                                break;
                        case 1:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,7,64),10);
                                LCD.printf("TriFunc"); LCD.display();
                                pc.printf("TriFunc\r\n");
                                break;
                        case 2:
                                LCD.clearDisplay();
                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                                LCD.printf("EXIT!!"); LCD.display();
                                pc.printf("EXIT\r\n");
                                break;
                        }
                }
                switch (motorModeMode) {
                case 0: Main.putc('a');
                        SW.SetSw1(0); //0に戻し、モード選択に移り変わる
                        SW.SetMode(0);//0に戻し、モード選択に移り変わる
                        while (1) {
                                while(SW.GetSw1State()==0) {
                                        if(SW.GetMode() > 5) {
                                                SW.SetMode(0);
                                        }else if(SW.GetMode() < 0) {
                                                SW.SetMode(4);
                                        }
                                        switch (SW.GetMode()) {
                                        case 0: LCD.clearDisplay();
                                                LCD.setTextSize(2);
                                                if(m_1<10&&m_1>=0) {LCD.setTextCursor(CenterX(2,4,64),10);}
                                                else if((m_1<100&&m_1>=10)||(m_1<0&&m_1>-10)) {LCD.setTextCursor(CenterX(2,5,64),10);}
                                                else if(m_1>=100||m_1<=-10) {LCD.setTextCursor(CenterX(2,6,64),10);}
                                                else if(m_1<=-100) {LCD.setTextCursor(CenterX(2,7,64),10);}
                                                LCD.printf("M1:%d",m_1); LCD.display(); break;
                                        case 2: LCD.clearDisplay();
                                                LCD.setTextSize(2);
                                                if(m_2<10&&m_2>=0) {LCD.setTextCursor(CenterX(2,4,64),10);}
                                                else if((m_2<100&&m_2>=10)||(m_2<0&&m_2>-10)) {LCD.setTextCursor(CenterX(2,5,64),10);}
                                                else if(m_2>=100||m_2<=-10) {LCD.setTextCursor(CenterX(2,6,64),10);}
                                                else if(m_2<=-100) {LCD.setTextCursor(CenterX(2,7,64),10);}
                                                LCD.printf("M2:%d",m_2); LCD.display(); break;
                                        case 3: LCD.clearDisplay();
                                                LCD.setTextSize(2);
                                                if(m_3<10&&m_3>=0) {LCD.setTextCursor(CenterX(2,4,64),10);}
                                                else if((m_3<100&&m_3>=10)||(m_3<0&&m_3>-10)) {LCD.setTextCursor(CenterX(2,5,64),10);}
                                                else if(m_3>=100||m_3<=-10) {LCD.setTextCursor(CenterX(2,6,64),10);}
                                                else if(m_3<=-100) {LCD.setTextCursor(CenterX(2,7,64),10);}
                                                LCD.printf("M3:%d",m_3); LCD.display(); break; LCD.display(); break;
                                        case 4: LCD.clearDisplay();
                                                LCD.setTextSize(2);
                                                if(m_4<10&&m_4>=0) {LCD.setTextCursor(CenterX(2,4,64),10);}
                                                else if((m_4<100&&m_4>=10)||(m_4<0&&m_4>-10)) {LCD.setTextCursor(CenterX(2,5,64),10);}
                                                else if(m_4>=100||m_4<=-10) {LCD.setTextCursor(CenterX(2,6,64),10);}
                                                else if(m_4<=-100) {LCD.setTextCursor(CenterX(2,7,64),10);}
                                                LCD.printf("M4:%d",m_4); LCD.display(); break;
                                        case 5:
                                                LCD.clearDisplay();
                                                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,6,64),10);
                                                LCD.printf("EXIT!!"); LCD.display();
                                                break;
                                        }
                                }

                                while(SW.GetSw1State()==1) {
                                        switch (SW.GetMode()) {
                                        case 1: break;
                                        case 2: break;
                                        case 3: break;
                                        case 4: break;
                                        case 5: SW.SetSw1(0); break;
                                        }
                                }

                        }
                        break;
                case 1: Main.putc('b'); break;
                case 2: Main.putc(MODESW); break;
                }

                while(SW.GetSw1State() == 1) {
                        pc.printf("Motor mode MOVING...\r\n");
                }
                wait_ms(3);
                Main.putc(MODESW);
                SW.SetSw1(0);
                SW.SetMode(MOTOR_MODE);
        }else{
                LCD.clearDisplay();
                LCD.setTextSize(2);  LCD.setTextCursor(CenterX(2,5,64),10);
                LCD.printf("Motor"); LCD.display();
        }
}

void ReceiveSpeed(){//デフォルトのスピードをメインから受信する
        speed = Main.getc();
        SW.SetMode(speed);
}

void bargraph(int r){
        //draw the bar graph
        LCD.fillRect(r, 50, 128 - r, 10, BLACK);
        LCD.fillRect(3, 50, r, 10, WHITE);

        for (uint8_t i = 1; i < 13; i++) {
                LCD.fillRect(i * 10, 50, 2, 10, BLACK);
        }
}

void setup(){
        pc.baud(230400);//pc
        Main.baud(BAUDRATE);//メインマイコン　BAUDRATE
        i2c.frequency(400000);//400kHz
        Main.putc(MODESW);//初期値を投げる(リセットを数回繰り返せばどのモードであっても同期を取り戻せる)
        pc.printf("\n\nHello world!! LEGEND US_module\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n", (SystemCoreClock / 1000000));
        LCD.clearDisplay();
        LCD.fillScreen(BLACK);
        for (uint8_t i = 0; i < 65; i += 15) {
                LCD.clearDisplay();
                LCD.fillCircle(64,32,i, WHITE);
                LCD.display();
        }
        switch (__HAL_RCC_GET_SYSCLK_SOURCE()) {
        case RCC_SYSCLKSOURCE_STATUS_HSE:    printf("HSE\n"); break;
        case RCC_SYSCLKSOURCE_STATUS_HSI:    printf("HSI\n"); break;
        case RCC_SYSCLKSOURCE_STATUS_PLLCLK: printf("PLL\n"); break;
        default: break;
        }//クロックのモードをプリント
        SP.State(BUZZERSTATE); SW.SetSound(BUZZERSTATE);//ブザーのセットアップ
        LCD.clearDisplay(); LCD.display();
        LCD.setTextSize(3);  LCD.setTextCursor(CenterX(3,6,64),10);
        LCD.printf("LEGEND"); LCD.display();
        for (size_t i = 0; i < 8; i++) {LED.On(i); wait_ms(30);}//チカチカさせる
        LED.On(WHITE);
        SP.sound(0); SP.sound(1); SP.sound(2);
        wait_ms(200); SP.sound(3);
}

int main() {
        setup();
        while (1) {
                SW.ReadLongPusshPlus(); SW.ReadLongPusshMinus();//長押し判断
                UI.Mode  = SW.GetMode();//モードを取得
                UI.Start = SW.GetSw1State();//スタートスイッチのOnかOffを取得
                if(UI.Mode > MODE_MAX) {SW.SetMode(0);}else if(UI.Mode < 0) {SW.SetMode(MODE_MAX);}
                SP.sound(7);
                switch (UI.Mode) {
                case START_MODE:   MainModeProgram();    break;
                case BALL_MODE:    BallModeProgram();    break;
                case LINE_MODE:    LineModeProgram();    break;
                case COMPASS_MODE: CompassModeProgram(); break;
                case US_MODE:      USModeProgram();      break;
                case CAM_MODE:     CamModeProgram();     break;
                case DEBUG_MODE:   DebugModeProgram();   break;
                case SPEED_MODE:   SpeedModeProgram();   break;
                case KICKER_MODE:  KickerModeProgram();  break;
                // case MOTOR_MODE:  MotorModeProgram();  break;
                default: break;
                }
                pc.printf("MODE:%d\n", UI.Mode);
        }
}
