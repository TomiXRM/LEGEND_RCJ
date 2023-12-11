#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern RawSerial pc;
extern OpenMV cam;
extern Motors MD;
// extern struct sensors;
//OpenMV.hの変数で中立点の配列作ってグローバルにする。

char mode;//カメラモードの中のモード
uint8_t posinum;

void Body_cam();
void ReceivecamMode();
void RecieveMode();
void ReceiveXY();
INLINE void SetPosition();
INLINE void GoToHome();
INLINE void FacingGoal();
INLINE void PositionMonitor();

void initCameraProcess(){
        ui.Entry(CAM,DONOTHING,callback(&Body_cam),DONOTHING);
        posinum = 0;
}

void ReceiveCamMode(){
        mode = ui.getc();
}

void ReceiveMode(){
        ui.mode = ui.getc();
}

void ReceiveXY(){
        /*posinum
           1:相手右
           2:相手左
           3:中心
           4:自右
           5:自左
           6:Exit
         */
        posinum = ui.getc();
        // 中立点の書き込み
        cam.PointOwnX[posinum-1] = cam.OwnX;
        cam.PointOwnY[posinum-1] = cam.OwnY;
        cam.PointOpsX[posinum-1] = cam.OpsX;
        cam.PointOpsY[posinum-1] = cam.OpsY;


        if(posinum == 1) {
                pc.printf("Opposite Righ\r\n");
        }else if(posinum == 2) {
                pc.printf("Opposite Left\r\n");
        }else if(posinum == 3) {
                pc.printf("Center\r\n");
        }else if(posinum == 4) {
                pc.printf("Own Righ\r\n");
        }else if(posinum == 5) {
                pc.printf("Own Left\r\n");
        }else if(posinum == 6) {
                pc.printf("Exit!!\r\n");
                ui.mode = MODESW;
        }
}
void Body_cam(){
        pc.printf("cam Mode!!\r\n");
        mode = CAM;
        ui.attachRx(&ReceiveCamMode);
        while (mode == CAM) {
                pc.printf("cam Mode waiting...\r\n"); wait_ms(1);
        }

        pc.printf("Mode recieverd!!:%c\r\n",mode);
        switch (mode) {
        case SETPOSITION: SetPosition(); break;
        case GOTOHOME: GoToHome(); break;
        case FACINGOPSGOAL: FacingGoal(); break;
        case POSITIONMONITOR: PositionMonitor(); break;
        case MODESW:
                ui.attachRx(&ReceiveMode);
                ui.mode = CAM;
                while (ui.mode == CAM) {
                        pc.printf("Exit waiting...\r\n");
                } break;
        }
}



INLINE void SetPosition(){
        posinum = 0;
        ui.attachRx(&ReceiveXY);
        ui.mode = SETPOSITION;
        while (ui.mode == SETPOSITION) {
                cam.get();
                wait_ms(10);
                pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\r\n",cam.OwnX,cam.OwnY,cam.OpsX,cam.OpsY,cam.calDegree(1),cam.calDegree(0));
        } ui.attachRx(&ReceiveMode);
        pc.printf("OppositeRight   ownX:%d \townY:%d \topsX:%d \topsY:%d\r\n",cam.PointOwnX[0],cam.PointOwnY[0],cam.PointOpsX[0],cam.PointOpsY[0]);
        pc.printf("OppositeLeft    ownX:%d \townY:%d \topsX:%d \topsY:%d\r\n",cam.PointOwnX[1],cam.PointOwnY[1],cam.PointOpsX[1],cam.PointOpsY[1]);
        pc.printf("CenterPoint     ownX:%d \townY:%d \topsX:%d \topsY:%d\r\n",cam.PointOwnX[2],cam.PointOwnY[2],cam.PointOpsX[2],cam.PointOpsY[2]);
        pc.printf("OwnsideRight    ownX:%d \townY:%d \topsX:%d \topsY:%d\r\n",cam.PointOwnX[3],cam.PointOwnY[3],cam.PointOpsX[3],cam.PointOpsY[3]);
        pc.printf("OwnsideLeft     ownX:%d \townY:%d \topsX:%d \topsY:%d\r\n",cam.PointOwnX[4],cam.PointOwnY[4],cam.PointOpsX[4],cam.PointOpsY[4]);
        pc.printf("\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n"); wait(2);


}
INLINE void GoToHome(){
        ui.mode = GOTOHOME;
        ui.attachRx(&ReceiveMode);
        while (ui.mode == GOTOHOME) {
                pc.printf("Go to Home!!\r\n");
        }
}

INLINE void FacingGoal(){
        ui.mode = FACINGOPSGOAL;
        ui.attachRx(&ReceiveMode);
        while (ui.mode == FACINGOPSGOAL) {
                pc.printf("Facing opposite goal!!\r\n");
        }
}

INLINE void PositionMonitor(){
        ui.mode = POSITIONMONITOR;
        ui.attachRx(&ReceiveMode);
        while (ui.mode == POSITIONMONITOR) {
                cam.get();
                wait_ms(10);
                // pc.printf("OwnX:%d \t OwnY:%d \t OpsX:%d \t OpsY:%d\r\r\n",cam.OwnX,cam.OwnY,cam.OpsX,cam.OpsY);
                pc.printf("OwnX:%d\tOwnY:%d\tOpsX:%d\tOpsY:%d\tOpsDir:%d\tOwnDir:%d\tOwnDist:%d\tOpsDist:%d\r\n",cam.OwnX,cam.OwnY,cam.OpsX,cam.OpsY,cam.calDegree(1),cam.calDegree(0),(uint16_t)sqrt(pow(cam.OwnX,2)+pow(cam.OwnY,2)),(uint16_t)sqrt(pow(cam.OpsX,2)+pow(cam.OpsY,2)));
                ui.putc('V');
                ui.putc((char)(cam.OwnX));
                ui.putc((char)(cam.OwnY));
                ui.putc((char)(cam.OpsX));
                ui.putc((char)(cam.OpsY));
        }

}
