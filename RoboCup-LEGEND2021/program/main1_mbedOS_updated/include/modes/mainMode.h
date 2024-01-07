#ifndef _MAINMOEDEH_
#define _MAINMOEDEH_
#include "setup.h"

#define CAMDIR_SET

#define IS_BALL_FOUND (sensors.irDeg != -5000)
#define IS_BALL_IN_FRONT_OF_ME (abs(sensors.irDeg) <= 30)

#define IS_HOLDING_THE_BALL (abs(sensors.irDeg) < 30 && sensors.irDist > 160 && PhotoF < 200)
#define IS_ABLE_TO_KICK (IS_HOLDING_THE_BALL && PhotoB < 200)
#define IS_OPS_GOAL_FOUND (camera.OpsW != 0)
#define IS_OWN_GOAL_FOUND (camera.OwnW != 0)
#define IS_FRONT_OF_OWN_GOAL (abs(camera.OwnY - (0.0000004 * pow(camera.OwnX, 4) - 0.00002 * pow(camera.OwnX, 3) + 0.0047 * pow(camera.OwnX, 2) + 0.0265 * camera.OwnX - 87)) < 25)
//  #define IS_FRONT_OF_OWN_GOAL camera.OwnY-(-0.000006*pow(camera.OwnX,5) + 0.0000002*pow(camera.OwnX,4) + 0.00005*pow(camera.OwnX,3) + 0.0051*pow(camera.OwnX,2) + 0.0058*camera.OwnX - 80.336)
int16_t TrackingDeg;
uint16_t PhotoF, PhotoB;
bool lineTraceF;
bool camNone;          // カメラ見えないフラグ
uint8_t useOpsGoalDir; // 相手ゴールの角度を利用するかどうか
Timer keepCamDirT;
Timer moveBackTimer;
Timer dribHoldBackT;
Timer dribBallHoldT;
Timer dribStackT;
Timer dribHoldOwnSide;
Timer missTheBallT;
bool setKicker;
bool isStack;

void before_Main() {
    moveBackTimer.start();
    moveBackTimer.reset();
    keepCamDirT.start();
    keepCamDirT.reset();
    dribHoldBackT.start();
    dribHoldBackT.reset();
    dribStackT.start();
    dribStackT.reset();
    dribBallHoldT.start();
    dribBallHoldT.reset();
    dribHoldOwnSide.start();
    dribHoldOwnSide.reset();
    missTheBallT.start();
    missTheBallT.reset();
}

INLINE void getSensors() {
    sensors.dir = IMU.getDeg();
    Line.readLine();
    sensors.irRecieved = IR.get();       // IR取得開始
    sensors.irDegLast = sensors.irDeg;   // IR前回の角度
    sensors.irDistLast = sensors.irDist; // IR前回の距離
    sensors.irDeg = IR.getDir();         // IR角度代入
    sensors.irDist = IR.getDist();       // IR距離代
    // sensors.irDist *= 0.8;
    if (sensors.irDist > 190) {
        sensors.irDist = 190;
    } // IR距離調整

    IRv = makeXYFromRT(sensors.irDeg == -5000 ? 0 : 100, sensors.irDeg); // 角度をベクトル
    sensors.velocity = IMU.getVelocity();                                // 速度

    PhotoF = (uint16_t)(Photo1.read_u16() / 65.535);
    PhotoB = (uint16_t)(Photo2.read_u16() / 65.535);
    sensors.distB = (int16_t)(BK.read_u16() / 65.535);
    // pc.printf("%d\t%d\r\n",(uint16_t)(Photo1.read_u16()/65.535),(uint16_t)(Photo2.read_u16()/65.535));
    cam.recognizeCheck();                                                         // ゴールが見えているかいないか
    camera.OwnX = cam.OwnX;                                                       // 自陣ゴールX座標
    camera.OwnY = cam.OwnY;                                                       // 自陣ゴールY座標
    camera.OpsX = cam.OpsX;                                                       // 敵陣ゴールX座標
    camera.OpsY = cam.OpsY;                                                       // 敵陣ゴールY座標
    camera.OwnW = cam.OwnW;                                                       // 自陣ゴール幅
    camera.OpsW = cam.OpsW;                                                       // 敵陣ゴール幅
    camera.OpsDir = -cam.calDegree(1);                                            // 敵陣ゴールの角度
    camera.OwnDir = -cam.calDegree(0);                                            // 自陣ゴールの角度
    camera.OwnDist = sqrt(camera.OwnX * camera.OwnX + camera.OwnY * camera.OwnY); // 敵陣ゴールの距離(カメラの値上)
    camera.OpsDist = sqrt(camera.OpsX * camera.OpsX + camera.OpsY * camera.OpsY); // 自陣ゴールの距離(カメラの値上)

    // カメラで幅と端を書き込むプログラム
    camera.OwnGoalRightEnd = cam.OwnGoalRightEnd;
    camera.OwnGoalLeftEnd = cam.OwnGoalLeftEnd;
    camera.OpsGoalRightEnd = cam.OpsGoalRightEnd;
    camera.OpsGoalLeftEnd = cam.OpsGoalLeftEnd;
    sensors.calDir = (float)sensors.dir;
    //   camera.probabilityNearOwnGoal = camera.OwnY-(-0.000000001*pow(camera.OwnX,5) + 0.0000006*pow(camera.OwnX,4) - 0.000005*pow(camera.OwnX,3) + 0.0042*pow(camera.OwnX,2) - 0.0973*camera.OwnX - 80);
    camera.probabilityNearOwnGoal = abs(camera.OwnY - (-0.0000000058 * pow(camera.OwnX, 5) + 0.0000002436 * pow(camera.OwnX, 4) + 0.0000461367 * pow(camera.OwnX, 3) + 0.0051113258 * pow(camera.OwnX, 2) + 0.0058177192 * camera.OwnX - 80.3358845775));
    if (Line.lastOnLineFlag != Line.isOnLine && Line.isOnLine) mouse.reset();
    MouseV.x = mouse.x;
    MouseV.y = mouse.y;
}

INLINE void GotToCenter() {
    if (camera.OpsW != 0) {
        DriveV.x = camera.OpsX * 3;
        if (abs(DriveV.x) > 60) {
            if (DriveV.x < 0) {
                DriveV.x = -60;
            } else {
                DriveV.x = 60;
            }
        }

    } else {
        if (camera.OwnW != 0) {
            DriveV.x = camera.OwnX * 3;
            if (DriveV.x < 0) {
                DriveV.x = -60;
            } else {
                DriveV.x = 60;
            }
        } else {
            DriveV = setZero();
        }
    }
    if (moveBackTimer.read_ms() < 500) {
        DriveV.y = -40;
    } else {
        DriveV.y = 0;
    }
}
#define OPSDIR_MAXDEG 45
INLINE void decideRobotDir(uint8_t useCamDir) {
    // set == 1 -> caldir = cam | 0 -> imu
#ifdef CAMDIR_SET
    if (IS_BALL_FOUND && useCamDir > 0 && abs(sensors.dir) < 70) { // abs(sensors.irDeg - camera.OpsDir) < 70
        if (IS_OPS_GOAL_FOUND) {
            camNone = 0;
            if (degBetween(sensors.dir, camera.OpsDir) < OPSDIR_MAXDEG) { // なるべく正面向きたいので制限をかける
                if (abs(degBetween((int16_t)sensors.irDeg, camera.OpsDir)) < 60) {
                    pidDir.target = 0;
                    if (useCamDir == 1) {
                        sensors.turnDir = -camera.OpsX * 0.3;
                        imuDirEnable = false; // 重要
                    } else if (useCamDir == 2) {
                        sensors.turnDir = -camera.OpsX * 0.2;
                        imuDirEnable = false; // 重要
                    }
                } else {
                    // sensors.turnDir = -camera.OpsX*0.7;
                    pidDir.target = 0;
                    sensors.calDir = sensors.dir;
                    imuDirEnable = true; // 重要
                }
            } else {
                sensors.calDir = sensors.dir;
                pidDir.target = (degBetween_signed(sensors.dir, camera.OpsDir) > 0 ? -OPSDIR_MAXDEG : OPSDIR_MAXDEG);
                imuDirEnable = 1;
            }
        } else {
            if (camNone == 1) {
                if (keepCamDirT.read_ms() > 500) {
                    camNone = 1;
                    sensors.calDir = sensors.dir;
                    pidDir.target = 0;
                    imuDirEnable = true; // 重要
                }
            } else {
                keepCamDirT.reset();
                camNone = 1;
                if (abs(sensors.dir) < 60) {
                    sensors.turnDir = -camera.OpsX * 0.3;
                    imuDirEnable = false; // 重要
                } else {
                    sensors.calDir = sensors.dir;
                    pidDir.target = 0;
                    imuDirEnable = true; // 重要
                }
            }
        }
    } else {
        sensors.calDir = sensors.dir;
        pidDir.target = 0;
        imuDirEnable = true;
    }
#else
    sensors.calDir = sensors.dir;
    imuDirEnable = true;
#endif
}

// INLINE void BallTracking(uint8_t MovingSpeed){
//         if(IS_BALL_FOUND) {
//                 if(abs(sensors.irDeg) < 90) {
//                         //drib.state(1);
//                 }else{
//                         //drib.state(0);
//                 }
//                 if(abs(sensors.irDeg) <= 25) {
//                         if(sensors.irDist < 180) {
//                                 TrackingDeg = sensors.irDeg*1.8;
//                         }else{
//                                 TrackingDeg = sensors.irDeg*3;
//                         }
//                         DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
//                 }else if(abs(sensors.irDeg) >= 120) {
//                         if(abs(sensors.irDeg) < 111&&abs(sensors.irDeg) <140) {
//                                 if(sensors.irDeg > 0) {
//                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg+60)+asin(50/(255 - sensors.irDist))/PI*180.0;
//                                 }else{
//                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                         TrackingDeg = (sensors.irDeg-60)-asin(50/(255 - sensors.irDist))/PI*180.0;
//                                 }
//                         }else{
//                                 if(sensors.irDist < 190) {
//                                         if(abs(sensors.irDeg) == 180) {
//                                                 TrackingDeg = sensors.irDeg+30;//ない
//                                         }else{
//                                                 if(sensors.irDeg > 0) {
//                                                         //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                                         TrackingDeg = sensors.irDeg+23;
//                                                 }else{
//                                                         //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                                         TrackingDeg = sensors.irDeg-23;
//                                                 }
//                                         }
//                                 }else{
//                                         if(sensors.irDeg > 0) {
//                                                 //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                                 TrackingDeg = (sensors.irDeg+60)+asin(60/(255 - sensors.irDist))/PI*180.0;
//                                         }else{
//                                                 //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                                 TrackingDeg = (sensors.irDeg-60)-asin(60/(255 - sensors.irDist))/PI*180.0;
//                                         }
//                                 }
//                         }
//                         DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
//                 }else{//120みまん
//                         if(sensors.irDeg > 0) {
//                                 //TrackingDeg = pow(abs(sensors.irDeg),1.3);
//                                 if(sensors.irDist < 190) {
//                                         TrackingDeg = sensors.irDeg+15;
//                                 }else{
//                                         TrackingDeg = (sensors.irDeg+60)+asin(60/(255 - sensors.irDist))/PI*180.0;
//                                 }
//                         }else{
//                                 //TrackingDeg = -pow(abs(sensors.irDeg),1.3);
//                                 if(sensors.irDist < 180) {
//                                         TrackingDeg = sensors.irDeg-15;
//                                 }else{
//                                         TrackingDeg = (sensors.irDeg-60)-asin(60/(255 - sensors.irDist))/PI*180.0;
//                                 }

//                         }
//                         DriveV = makeXYFromRT(MovingSpeed,TrackingDeg);
//                 }
//                 // if(sensors.irDeg < 0&& sensors.irDeg >= 90) {
//                 //         DriveV.x -= 7;
//                 // }
//                 // if(abs(sensors.irDeg) <= 30 ) {
//                 //         DriveV.y +=40;
//                 // }
//         }else{
//                 // DriveV.x = 0; DriveV.y = 0;
//                 //drib.state(0);
//                 GotToCenter();
//         }
//         decideRobotDir(1);
// }

char driblOrder = 0;
INLINE void drible(uint8_t MovingSpeed) {
    if (IS_OPS_GOAL_FOUND) { // Opposide goal found
        int16_t diffDeg = degBetween((int16_t)(sensors.dir), camera.OpsDir);
        if (diffDeg < 35) {
            driblOrder = 1;
            ////////////////////////////////////////////////////////////
            if (sensors.velocity < 2 && isStack == 0) { ///
                isStack = 1;
                dribStackT.reset();
            }
            if (isStack == 1) {
                if (dribStackT.read_ms() < 500) {
                    DriveV.y = 30;
                    DriveV.x = camera.OpsX * 4;
                    // ui.SetBZ(3);
                    setKicker = 0;
                    if (abs(camera.OpsX) < 20) {
                        isStack = 0;
                    }
                } else {
                    if (abs(camera.OpsX) < 15) {
                        DriveV.y = 70;
                        if (IS_ABLE_TO_KICK) setKicker = 1;
                    }
                    isStack = 0;
                }
            } else {
                // the position able to shoot
                if (dribHoldBackT.read_ms() < 400) {
                    useOpsGoalDir = 2;
                    // int16_t Vx = -(DriveV.x*cos32_T(sensors.dir) + DriveV.y*sin32_T(sensors.dir));
                    // int16_t Vy = -(DriveV.x*sin32_T(sensors.dir) - DriveV.y*cos32_T(sensors.dir));//座標復元
                    // Vy = -70;
                    // Vx = 3*camera.OpsX;
                    // DriveV.x = -(Vx*cos32_T(sensors.dir) + Vy*sin32_T(sensors.dir));
                    // DriveV.y = -(Vx*sin32_T(sensors.dir) - Vy*cos32_T(sensors.dir));
                    //     DriveV.y = -70;
                    DriveV.x = 4 * camera.OpsX;
                    if (abs(camera.OpsX) < 25) {
                        if (driblOrder == 3 && dribHoldBackT.read_ms() < 200) {
                            DriveV.x = 0;
                            DriveV.y = 0;
                            // drib.state(1);
                        } else if (dribHoldBackT.read_ms() < 300) {
                            // drib.state(0);
                            DriveV.y = 140;
                            DriveV.x = 0;
                        } else {
                            // drib.state(0);
                            DriveV.y = 140;
                            DriveV.x = 0;
                            if (IS_ABLE_TO_KICK) setKicker = 1;
                        }
                    } else {
                        // drib.state(1);
                        setKicker = 1;
                    }
                } else {
                    useOpsGoalDir = 1;
                    if (abs(camera.OpsX) < 15) {
                        DriveV.y = 80;
                        DriveV.x = 0;
                        // drib.state(0);
                        if (IS_ABLE_TO_KICK) setKicker = 1;
                    } else {
                        // drib.state(1);
                        if (IS_ABLE_TO_KICK) setKicker = 1;
                    }
                    DriveV.x = 3 * camera.OpsX;
                    // DriveV.y = 70;
                }
            }

        } else { // diffDeg > 35
            useOpsGoalDir = 1;
            if (dribBallHoldT.read_ms() < 70) {
                DriveV.y = 40;
                driblOrder = 2;
            } else {
                // //move the position could shoot
                // DriveV.x = 5*camera.OpsX;
                // DriveV.y = -90;
                // useOpsGoalDir = 0;
                // setKicker = 0;
                if (abs(camera.OpsDir) < 50) {
                    useOpsGoalDir = 2;
                    DriveV.y = 40;
                    DriveV.x = 1.2 * camera.OpsX;
                } else {
                    driblOrder = 3;
                    useOpsGoalDir = 1;
                    DriveV.x = camera.OpsX * 4;
                }
                //   DriveV.y -=30;

                dribHoldBackT.reset();
                // drib.state(1);
                setKicker = 0;
            }
        }
    } else {
        useOpsGoalDir = 0;
        if (IS_OWN_GOAL_FOUND) { // Own Goal
            if (dribBallHoldT.read_ms() < 20) {
                DriveV.x = 0;
                DriveV.y = 100;
                // drib.state(1);
                setKicker = 0;

            } else {
                // if(dribHoldOwnSide.read_ms() < 500) {
                //         DriveV.y = MovingSpeed;
                //         DriveV.x = camera.OwnX*6;
                //         setKicker = 0;
                // }else{
                //         DriveV.y = MovingSpeed;
                //         setKicker = 1;
                // }
                DriveV.y = MovingSpeed;
                DriveV.x = camera.OwnX * 4;
                if (DriveV.x > 100) {
                    DriveV.x = 100;
                }
                if (DriveV.x < -100) {
                    DriveV.x = -100;
                }
                if (abs(camera.OwnX) < 20 || dribBallHoldT.read_ms() > 400) {
                    // drib.state(0);
                    setKicker = 1;
                    DriveV.y = 70;
                    DriveV.x = 0;
                    dribBallHoldT.reset();
                } else {
                    // drib.state(1);
                    setKicker = 0;
                }
            }
        } else {
            // none

            // DriveV.y = 10;
            // if(dribBallHoldT.read_ms() < 200) {
            //         setKicker = 0;
            // }else{
            //         setKicker = 1;
            // }
            // drib.state(1);
            setKicker = 1;
        }
    }
    pc.printf("LineDeg:%d\tIRDeg:%d\tPhF:%d\tPhB:%d\tOpsDir:%d\tTimer:%d\r\n", Line.lineDegree, sensors.irDeg, PhotoF, PhotoB, camera.OpsDir, dribBallHoldT.read_ms());
}

INLINE void BallTracking(uint8_t MovingSpeed) {
    useOpsGoalDir = 0;
    if (IS_BALL_FOUND) {
        if (abs(sensors.irDeg) <= 25) {
            if (sensors.irDist < 170) {
                TrackingDeg = sensors.irDeg * 1.8;
            } else {
                TrackingDeg = sensors.irDeg * 3;
            }
            DriveV = makeXYFromRT(MovingSpeed, TrackingDeg);
            if (sensors.irDeg == 0) DriveV.y = 70;
            if (IS_HOLDING_THE_BALL) {
                // if(sensors.velocity < 1) {
                //         //止まってる
                //         setKicker = 0;
                //         //ui.SetBZ(3);
                // }else{
                //
                if (IS_OPS_GOAL_FOUND) {
                    useOpsGoalDir = 1;

                    if (abs(camera.OpsX) < 15) {
                        setKicker = 1;
                    } else {
                        setKicker = 0;
                    }
                } else {
                    setKicker = 1;
                }

                // }
                //   drible(MovingSpeed);
            } else {
                setKicker = 0;
                dribBallHoldT.reset();
                dribHoldOwnSide.reset();
                driblOrder = 0;
            }

        } else if (abs(sensors.irDeg) >= 120) {
            // drib.state(0);
            driblOrder = 0;
            if (abs(sensors.irDeg) < 111 && sensors.irDist > 140) {
                if (sensors.irDeg > 0) {
                    // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = (sensors.irDeg + 60) + asin(50 / (255 - sensors.irDist)) / PI * 180.0;
                } else {
                    // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = (sensors.irDeg - 60) - asin(50 / (255 - sensors.irDist)) / PI * 180.0;
                }
            } else {
                if (sensors.irDist < 115) {
                    if (abs(sensors.irDeg) == 180) {
                        TrackingDeg = sensors.irDeg + 30;
                    } else {
                        if (sensors.irDeg > 0) {
                            // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                            TrackingDeg = sensors.irDeg + 20;
                        } else {
                            // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                            TrackingDeg = sensors.irDeg - 20;
                        }
                    }
                } else {
                    if (sensors.irDeg > 0) {
                        // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                        TrackingDeg = (sensors.irDeg + 60) + asin(60 / (255 - sensors.irDist)) / PI * 180.0;
                    } else {
                        // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                        TrackingDeg = (sensors.irDeg - 60) - asin(60 / (255 - sensors.irDist)) / PI * 180.0;
                    }
                }
            }
            DriveV.x = 0;
            DriveV.y = 0;
            setKicker = 0;
            dribBallHoldT.reset();
            dribHoldOwnSide.reset();
            // isHoldTheBallInCorner = 0;
            DriveV = makeXYFromRT(MovingSpeed, TrackingDeg);
        } else {
            driblOrder = 0;
            // drib.state(0);
            if (sensors.irDist < 170) {
                if (sensors.irDeg > 0) {
                    // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = sensors.irDeg + 30;
                } else {
                    // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = sensors.irDeg - 30;
                }
            } else {
                if (sensors.irDeg > 0) {
                    // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = (sensors.irDeg + 60) + asin(60 / (255 - sensors.irDist)) / PI * 180.0;
                } else {
                    // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                    TrackingDeg = (sensors.irDeg - 60) - asin(60 / (255 - sensors.irDist)) / PI * 180.0;
                }
            }
            setKicker = 0;
            dribBallHoldT.reset();
            dribHoldOwnSide.reset();
            // isHoldTheBallInCorner = 0;
            DriveV = makeXYFromRT(MovingSpeed, TrackingDeg);
            DriveV.y -= 30;
        }
        if (abs(sensors.irDeg) <= 90 && abs(sensors.irDeg) > 22 && sensors.irDist < 165) {
            if (sensors.irDeg > 0) {
                // TrackingDeg = pow(abs(sensors.irDeg),1.3);
                DriveV.x = -80;
            } else {
                // TrackingDeg = -pow(abs(sensors.irDeg),1.3);
                DriveV.x = 80;
            }
        }
        moveBackTimer.reset();
        missTheBallT.reset();

        if (abs(sensors.irDeg) > 120) {
        }
    } else {
        setKicker = 0;
        driblOrder = 0;
        // if(IS_OPS_GOAL_FOUND) {
        //         DriveV.x = camera.OpsX*5;
        // }else{
        //         DriveV = SetZero();
        // }
        // if(moveBackTimer.read_ms() < 500) {
        //         DriveV.y=-70;
        // }
        GotToCenter();
        // if(missTheBallT.read() < 10) {
        //         GotToCenter();
        //         // MD.drive(0,0,sensors.turnDir);
        //         DriveV.x = 0; DriveV.y = 0;
        // }else{
        //         MD.drive(0,70,sensors.turnDir);
        //         missTheBallT.read();
        //         while(!IS_BALL_FOUND && missTheBallT.read() < 2) {
        //                 getSensors();
        //                 MD.drive(0,70,sensors.turnDir);
        //         } missTheBallT.reset();
        //
        //
        // }
        // drib.state(0);        //
        dribBallHoldT.reset();
        dribHoldOwnSide.reset();
    }
    if (Line.isOnLine & (degBetween(90, Line.lineDegree) < 30 || degBetween(-90, Line.lineDegree) < 30)) useOpsGoalDir = 0;
    if (abs(sensors.dir) > 70) {
        useOpsGoalDir = 0;
    }

    decideRobotDir(useOpsGoalDir);
}

INLINE void avoidOwnGoalLine() {
    if (abs(camera.probabilityNearOwnGoal) < 12 && abs(camera.OwnX) > 20) {
        //     int16_t driveDeg = (short)(atan2(DriveV.x,DriveV.y)/PI*180.0);
        if (degBetween(sensors.irDeg, Line.lineDegree) < 30) { // degBetween(driveDeg,Line.lineDegree) < 3090-----
            if (camera.OwnX > 0 && sensors.irDeg >= 45) {
                Line.lineTrace(&DriveV, -70);
            } else if (camera.OwnX < 0 && sensors.irDeg <= -45) {
                Line.lineTrace(&DriveV, 70);
            }
        }
    } else {
        Line.manage(&DriveV, &IRv, &MouseV, sensors.irDeg, sensors.dir, &camera);
    }
}

// 試合中の処理
uint8_t speed = 0;
void body_Main() {
    getSensors();
    Line.lineConpute(sensors.dir);
    if (sensors.irDist < 140) {
        speed = 100;
    } else {
        speed = 70;
    }
    BallTracking(speed);
    Line.manage(&DriveV, &IRv, &MouseV, sensors.irDeg, sensors.dir, &camera);
    if (setKicker && IS_ABLE_TO_KICK) Kicker.Kick();

    btn.soundIndex(!Line.isOnLine * 7);
    MD.driveVector(DriveV, sensors.turnDir);

    // pc.printf("WhiteQty:%d\tCurrentDeg:%d\r\n",Line.onWhiteSensorQty,Line.lineCurrentDegree);
}

void after_Main() {
    // 停止時の処理
    btn.soundIndex(2);
    // drib.state(0);
}

const LG_mode_t mainMode = {name : "Main", before : callback(before_Main), body : callback(body_Main), after : callback(after_Main), modeColor : white};

#endif
//   ゴール前ライン対策として横のラインをふれた時にバックして回避したい
//   if(abs(sensors.irDeg) < 80) {
//           if(IRv.x > 0 && DriveV.x > 0 && Line.currentLineVPer45.x > 0 && sensors.irDist < 190) DriveV.y = 100;
//           if(IRv.x < 0 && DriveV.x < 0 && Line.currentLineVPer45.x < 0 && sensors.irDist < 190) DriveV.y = 100;

//   }else if(abs(sensors.irDeg) > 100) {
//           if(IRv.x > 0 && DriveV.x > 0 && Line.currentLineVPer45.x > 0 && sensors.irDist < 190) DriveV.y = -100;
//           if(IRv.x < 0 && DriveV.x < 0 && Line.currentLineVPer45.x < 0 && sensors.irDist < 190) DriveV.y = -100;
//   }

//   if(IS_BALL_FOUND && !Line.isHalfOut && abs(sensors.irDeg) < 100 && abs(sensors.irDeg) > 25) {
//           DriveV.x *=50; DriveV.y = 0;
//           if(sensors.irDeg > 0) {
//                   DriveV.x = -100;
//           }else if(sensors.irDeg < 0) {
//                   DriveV.x = 100;
//           }
//   }
//   Line.manage(&DriveV,&IRv,sensors.irDeg,sensors.dir);

//   if(IRv.x > 0 && Line.isHalfOut == 0 && DriveV.x > 0 && abs(Line.lineCurrentDegree + 90) < 45) {
//           Line.lineTrace(&DriveV,-100);
//   }else if(IRv.x < 0 && Line.isHalfOut == 0 && DriveV.x > 0 && abs(Line.lineCurrentDegree - 90) < 45) {
//           Line.lineTrace(&DriveV,100);
//   }
//   if(abs(sensors.irDeg - Line.lineDegree) < 60 && Line.isHalfOut == 0 && sensors.irDist < 190 && sensors.distB > 290) {
//           rgb.turnOn(white);
//           if(sensors.irDeg > 0) {
//                   Line.lineTrace(&DriveV,100);
//           }else{
//                   Line.lineTrace(&DriveV,-100);
//           }
//   }else{
//           rgb.turnOff();
//   }

//   }