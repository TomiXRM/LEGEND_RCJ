#include "mbed.h"
#include "moving_average.h"
#include "setup.h"
#include "setup_com.h"

MovingAverage smaDistB(10);

// prototypes
INLINE void getSensors();
INLINE void sensorMonitor();
INLINE void Body_Main();
INLINE void Body_Debug();
INLINE void BallTracking(uint8_t MovingSpeed);
INLINE void GoBackToHome(uint8_t MovingSpeed);

short corner_counter = 0;
short corner_runs_counter = 0;
short escape_counter = 0;
short escape_runs_counter = 0;

// Functions
INLINE void getSensors() {
        sensors.irRecieved = IR.get(); //取得開始
        cam.get();               //取得開始

        IMU.get();

        sensors.dir = (float)IMU.dir; //向いている角度の取得
        sensors.PhotoSensor = Photo.read_u16();
        if (abs(sensors.dir) < 4) {
                sensors.dir = 0;
        }                              //正面の微調整
        sensors.irDegLast = sensors.irDeg; //前回の角度
        sensors.irDistLast = sensors.irDist; //前回の距離
        sensors.irDeg = IR.getDir();   //角度代入
        sensors.irDist = IR.getDist(); //距離代入
        sensors.irDegTweak = sensors.irDeg; //計算ようの変数に代入
        if (abs(sensors.irDeg - sensors.irDegLast) <= 23) {
                sensors.irDegSame = 1;
        } else {
                sensors.irDegSame = 0;
        } //前回と一緒判定
        if (abs(sensors.irDist - sensors.irDistLast) <= 1) {
                sensors.irDistSame = 1;
        } else {
                sensors.irDistSame = 0;
        } //前回と一緒判定
        // IRv = makeXYFromRT(sensors.irDeg == 5000 ? 0 : 100,
        // sensors.irDeg);//角度をベクトルに
        IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100,
                           sensors.irDeg); //角度をベクトル
        IRvD = makeXYFromRT(sensors.irDist,
                            sensors.irDeg); //角度をベクトル

        cam.recognizeCheck();       //ゴールが見えているかいないか
        camera.OwnX = cam.OwnX;     //自陣ゴールX座標
        camera.OwnY = cam.OwnY;     //自陣ゴールY座標
        camera.OpsX = cam.OpsX;     //敵陣ゴールX座標
        camera.OpsY = cam.OpsY;     //敵陣ゴールY座標
        camera.OwnW = cam.OwnW;     //自陣ゴール幅
        camera.OpsW = cam.OpsW;     //敵陣ゴール幅
        camera.OpsDir = cam.calDegree(1); //敵陣ゴールの角度
        camera.OwnDir = cam.calDegree(0); //自陣ゴールの角度
        camera.OwnDist =
                sqrt(camera.OwnX * camera.OwnX +
                     camera.OwnY * camera.OwnY); //敵陣ゴールの距離(カメラの値上)
        camera.OpsDist =
                sqrt(camera.OpsX * camera.OpsX +
                     camera.OpsY * camera.OpsY); //自陣ゴールの距離(カメラの値上)

        //カメラで幅と端を書き込むプログラム
        camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
        camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
        camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
        camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;

        sensors.PhotoSensor = Photo.read_u16();

        //↓使う角度をカメラと方位センサから選ぶ
        /*if(camera.OpsW != 0) {//相手ゴールが見えている時
                //if(abs(sensors.dir) < 80) {
                sensors.camDir = ((float)(camera.OpsX)* -0.3);
                camera.useCamDirection = 1;
                directionOpsCtrl.reset();
                pidCamDir.Compute();
           }else{
                camera.useCamDirection = 0;
                pidDir.Compute();
           } */

        sensors.Distlb = PSDlb.read_u16();
        sensors.Distrb = PSDrb.read_u16();
        //  sensors.DistB = (sensors.Distlb + sensors.Distrb) / 2;
        sensors.DistB = (sensors.Distlb + sensors.Distrb) / 2;
        sensors.Distl = PSDl.read_u16();
        sensors.Distr = PSDr.read_u16();
        sensors.USDist = USBack.distance;

        sensors.LINE1 = LINE1;
        sensors.LINE2 = LINE2;
        sensors.LINE3 = LINE3;
        sensors.LINE4 = LINE4;
        pidDir.Compute();
}
INLINE void sensorMonitor() {
        //  pc.printf("Front:%d Right:%d Back:%d
        //  Left:%d\t",Line.front,Line.right,Line.back,Line.left);//ラインが触れているか
        // pc.printf("Order:%d\tRaw:%d\tLastRaw:%d\t",Line.order,Line.Raw,Line.LastRaw);
        // //ラインを踏んだ順番と現在踏んでいる角度

        pc.printf("IMUDir:%d\t", (short)(sensors.dir));
        // pc.printf("IRrecv:%d \tIRdeg:%d
        // \tIRdist%d\tIRx:%d\tIRy:%d\t",sensors.irRecieved,sensors.irDeg,sensors.irDist,((short)(IRv.x)),((short)(IRv.y)));//IR
        //  pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\t",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir);//カメラ
        // pc.printf("OwnDist:%d\tOpsDist%d\t",camera.OwnDist,camera.OpsDist);
        pc.printf("OwnRightEnd:%d\tOwnLeftEnd:%d\t", camera.OwnGoalRightEnd,
                  camera.OwnGoalLeftEnd);

        pc.printf("Back:%d\t", sensors.DistB);
        pc.printf("R:%d\t", sensors.Distr);
        pc.printf("RB:%d\t", sensors.Distrb);
        pc.printf("LB:%d\t", sensors.Distlb);
        pc.printf("L:%d\t", sensors.Distl);
        pc.printf("US:%d\t", sensors.USDist);
        pc.printf("Photo:%d\t", sensors.PhotoSensor);
        pc.printf("\r\n");
}

INLINE void SetX() {

        uint8_t Zero_Offset = 0;

        if (camera.OwnX != 0) {
                if (abs(camera.OwnX - Zero_Offset) < CAM_CENTER_RANGE) {
                        DriveV.x = 0;
                } else {
                        if (camera.OwnX - Zero_Offset > 0) {
                                DriveV.x = -100;
                        } else {
                                DriveV.x = 100;
                        }
                }
        } else {
                DriveV.x = 0;
        }
}

INLINE void SetY() {

        //家もでる
        if (sensors.DistB < DISTB_TOOLONG_THLESHOLD) {

                DriveV.y = -100;

        } else if (sensors.DistB < DISTB_BACKSTOP_THLESHOLD) {

                DriveV.y = -70;

        } else {
                if (sensors.DistB > DISTB_TOOCLOSE_THRESHOLD) {
                        DriveV.y = 30;
                } else {

                        DriveV.y = 0;
                }
        }
}

INLINE void Keeper(uint8_t MovingSpeed) {

        DriveV = SetZero();

        SetX();
        SetY();

        if (sensors.irDeg != -5000) {

                if (abs(sensors.irDeg) < BALL_STOP_RANGE_DEG) {
                        DriveV.x = 0;

                        if (sensors.DistB > DISTB_BACKSTOP_THLESHOLD + 500) {
                                DriveV.y = 20;
                        } else if (sensors.DistB > DISTB_BACKSTOP_THLESHOLD + 100) {
                                DriveV.y = 10;
                        }
                } else {

                        if (IRv.y > BALLY_FRONT_MIN || abs(sensors.irDeg) < 30) {

                                if (abs(sensors.irDeg) <= BALL_FRONT_RANGE_DEG) {

                                        if (IRv.x > 0) {
                                                DriveV.x = pow(sensors.irDeg, 2) / 9;
                                        } else {
                                                DriveV.x = -pow(sensors.irDeg, 2) / 9;
                                        }
                                } else {

                                        if (IRv.x > 0) {
                                                DriveV.x = 100;
                                        } else {
                                                DriveV.x = -100;
                                        }
                                }
                                if (sensors.DistB > DISTB_BACKSTOP_THLESHOLD + 500) {
                                        DriveV.y = 20;
                                }

                        } else if (IRv.y >= BALLY_SIDE_THRESHOLD || abs(sensors.irDeg) < 130) {
                                if (sensors.DistB < DISTB_TOOCLOSE_THRESHOLD) {
                                        DriveV.y = -80;
                                }

                                if (IRv.x > 0) {
                                        DriveV.x = 100;
                                } else {
                                        DriveV.x = -100;
                                }

                        } else {
                                if (sensors.USDist > US_BACK_SPACE_TURN_AROUND) {
                                        //後ろ余裕ある。回り込む。
                                        if (IRv.x > 0) {
                                                DriveV = makeXYFromRT(100, sensors.irDeg - 80);
                                        } else {
                                                DriveV = makeXYFromRT(100, sensors.irDeg + 80);
                                        }
                                } else {

                                        DriveV.y = -40;
                                        if (abs(sensors.irDeg) <= 135) {
                                                if (IRv.x > 0) {

                                                        DriveV.x = 100;

                                                } else {
                                                        DriveV.x = -100;
                                                }
                                        } else {
                                                if (IRv.x > 0) {
                                                        DriveV.x = 100;
                                                } else {
                                                        DriveV.x = -100;
                                                }
                                        }
                                }
                        }
                }

                if (sensors.DistB > DISTB_TOOCLOSE_THRESHOLD) {
                        DriveV.y = 50;
                }

                if ((camera.OwnGoalLeftEnd < -30) ||
                    (camera.OwnGoalRightEnd < -30 && sensors.Distlb > 35672)) {
                        DriveV.y = 80;
                }

                if (camera.OwnGoalRightEnd < -10) {
                        DriveV.x = -60;
                } else if (camera.OwnGoalLeftEnd < -10) {
                        DriveV.x = 60;
                }

                if (sensors.PhotoSensor < 2000 &&
                    abs(sensors.irDeg) < BALL_FRONT_RANGE_DEG) {
                        ui.SetKICKER(1);
                } else {
                        ui.SetKICKER(0);
                }

                /*
                                //右側にいる
                                if (camera.OwnGoalRightEnd < OWNRIGHT_BORDER) {
                                        DriveV.x = -20;

                                        if (sensors.Distlb > 17400) {
                                                DriveV.y += 30;
                                        }

                                } else if (camera.OwnGoalRightEnd < OWNRIGHT_STOP_BORDER &&
                                           sensors.irDeg < -22.5 && abs(sensors.irDeg) < 70)
                   { DriveV.x = 0;
                                }

                                //左側にいる
                                if (camera.OwnGoalLeftEnd < OWNLEFT_BORDER) {
                                        DriveV.x = 20;

                                        if (sensors.Distrb > 23300) {
                                                DriveV.y += 30;
                                        }

                                } else if (camera.OwnGoalLeftEnd < OWNLEFT_STOP_BORDER &&
                                           sensors.irDeg > 22.5 && abs(sensors.irDeg) < 70)
                   { DriveV.x = 0;
                                }*/
        }
}

INLINE void BallTracking(uint8_t MovingSpeed) {
        DriveV = SetZero();
        if (sensors.irDeg != -5000) {
                DriveV = makeXYFromRT(100, sensors.irDeg);

                if (sensors.irDist > 135) {

                        if (abs(sensors.irDeg) >= 90) {
                                if (IRv.x > 0) {
                                        DriveV = makeXYFromRT(100, sensors.irDeg - 90);
                                } else {
                                        DriveV = makeXYFromRT(100, sensors.irDeg + 90);
                                }

                        } else if (abs(sensors.irDeg) > 45) {

                                if (IRv.x > 0) {
                                        DriveV = makeXYFromRT(100, sensors.irDeg - 50);
                                } else {
                                        DriveV = makeXYFromRT(100, sensors.irDeg + 50);
                                }

                        } else if (abs(sensors.irDeg) > 20) {
                                if (IRv.x > 0) {
                                        DriveV = makeXYFromRT(100, sensors.irDeg - 30);
                                } else {
                                        DriveV = makeXYFromRT(100, sensors.irDeg + 30);
                                }

                        } else {

                                DriveV = makeXYFromRT(100, sensors.irDeg);
                        }
                }

        } else {
                DriveV = SetZero();
                // GoBackToHome(MovingSpeed);
        }

        if (sensors.PhotoSensor < 9000 && abs(sensors.irDeg) < 10) {
                ui.SetKICKER(1);
        } else {
                ui.SetKICKER(0);
        }
}

void SuperOffence() {
}
INLINE void Body_Main() {
        ui.SetBZ(7);
        getSensors();

        BallTracking(ui.speed);
        /*
                if (abs(sensors.dir) < 90) {
                        Keeper(ui.speed);
                }*/

        Bound = makeXYFromRT(100, Line.LastRaw); //前回ラインを踏んでいた

        Line.SetLine(&DriveV, &IRv, &Bound); //フィルタリング処理

        MD.driveVector(DriveV, sensors.turnDir);

        if (Line.OnFlag == 1) {
                ui.SetBZ(0);
        } /*0~7（7は音を鳴らさない）*/ // buzzer
        ui.putc(ui.state); //キッカー、LED、ブザーの情報をなげる
        // pc.printf("Order:%d\tRaw:%d\tLastRaw:%d\t",Line.order,Line.Raw,Line.LastRaw);//ラインを踏んだ順番と現在踏んでいる角度
        pc.printf("OwnX:%d\tOwnY:%d\tOwnW:%d\tOwnDir:%d\tOwnRigh:%d\tOwnLeft:%"
                  "d\tBack:%d\tPhoto:%d",
                  camera.OwnX, camera.OwnY, camera.OwnW, camera.OwnDir,
                  camera.OwnGoalRightEnd, camera.OwnGoalLeftEnd, sensors.DistB,
                  sensors.PhotoSensor);
        // pc.printf("FirstWhiteSensor:%d\tLastWhiteSensor:%d\torder:%d\5Raw:%d",Line.FirstWhiteSensor,Line.LastWhiteSensor,Line.order,Line.Raw);

        // pc.printf("Order:%d\tRaw:%d\tOutF:%d",Line.order,Line.Raw,Line.OutFlag);
        // //ラインを踏んだ順番と現在踏んでいる角度

        // pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\tCamDir:%d\tturnDir:%d",camera.OwnX,camera.OwnY,camera.OpsX,camera.OpsY,camera.OpsDir,camera.OwnDir,(short)(sensors.camDir*100),(short)(sensors.turnDir*100));//カメラ
        pc.printf("\r\n");
}

INLINE void Body_Debug() {
        // Body_Main();
        getSensors();
        Line.SetLine(&DriveV, &IRv, &Bound); //フィルタリング処理
        sensorMonitor();

        pc.printf("\r\n");
}

void setup() {
        pc.baud(230400); // パソコン,IRのボーレート
        ui.baud(BAUDRATE); // UIとのボーレート

        pidDir.SetMode(AUTOMATIC);
        pidDir.SetOutputLimits(-100, 100);
        pidDir.SetSampleTime(9); // 5
        pidCamDir.SetMode(AUTOMATIC);
        pidCamDir.SetOutputLimits(-60, 60);
        pidCamDir.SetSampleTime(5); // 5

        ui.speed = 100; //デフォルトスピード
        camera.useCamDirection = 1;

        _MotoSLP.mode(PullDown);
        wait(1);
        // Mode enrties.
        ui.Entry(MAIN, DONOTHING, callback(&Body_Main),
                 DONOTHING); // callback(&stopAll)
        ui.Entry(DEBUG, DONOTHING, callback(&Body_Debug), DONOTHING);

        initSpeedProcess();
        initBallProcess();
        initCompassProcess();
        initLineProcess();
        initCameraProcess();
        initUsProcess();
        KickerCtrl.start();
        initUsProcess();

        ui.Entry(SPEED, callback(&Before_Speed), callback(&Body_Speed), DONOTHING);
}

int main() {
        setup();
        pc.printf("\r\nHello world!! LEGEND US_module\r\n");
        pc.printf("This STM32 works with %d MHz clock\r\n",
                  (SystemCoreClock / 1000000));
        while (1) {
                ui.Run();
        }
}
