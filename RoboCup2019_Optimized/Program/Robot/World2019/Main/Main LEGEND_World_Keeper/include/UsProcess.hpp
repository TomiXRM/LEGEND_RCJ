#include "setup.h"
#include "setup_com.h"

#define FLG_DISTB_TOOLONG_THLESHOLD 0
#define FLG_DISTB_BACKSTOP_THLESHOLD 1
#define FLG_DISTB_TOOCLOSE_THRESHOLD 2
#define FLG_BALLY_FRONT_MIN 3
#define FLG_BALLY_SIDE_THRESHOLD 4
#define FLG_US_BACK_SPACE_TURN_AROUND 5
#define FLG_OWNRIGHT_BORDER 6
#define FLG_OWNRIGHT_STOP_BORDER 7
#define FLG_OWNLEFT_BORDER 8
#define FLG_OWNLEFT_STOP_BORDER 9
#define FLG_THRESHOLD_PHOTO 10

extern uint16_t DISTB_TOOLONG_THLESHOLD;
extern uint16_t DISTB_BACKSTOP_THLESHOLD;
extern uint16_t DISTB_TOOCLOSE_THRESHOLD;
extern uint8_t BALLY_FRONT_MIN;
extern uint8_t BALLY_SIDE_THRESHOLD;
extern uint8_t US_BACK_SPACE_TURN_AROUND;
extern uint8_t OWNRIGHT_BORDER;
extern uint8_t OWNRIGHT_STOP_BORDER;
extern uint8_t OWNLEFT_BORDER;
extern uint8_t OWNLEFT_STOP_BORDER;
extern uint16_t THRESHOLD_PHOTO;

extern LGUI ui;
extern RawSerial pc;
extern type_sensors sensors;
extern type_camera camera;
extern void getSensors();
extern VectorXY IRv;
void Body_US();
void ReceiveThreshold();
void InitUS();

void initUsProcess() {
        ui.Entry(US, callback(&InitUS), callback(&Body_US), DONOTHING);
}

void InitUS() {
        ui.attachRx(&ReceiveThreshold);
}
void Body_US() {
        getSensors();

        pc.printf(
                "=============================================\n uint16_t "
                "DISTB_TOOLONG_THLESHOLD = % d; \n uint16_t DISTB_BACKSTOP_THLESHOLD = % "
                "d;\n uint16_t DISTB_TOOCLOSE_THRESHOLD = % d;\n uint8_t BALLY_FRONT_MIN "
                "= % d;\n uint8_t BALLY_SIDE_THRESHOLD = % d;\n uint8_t "
                "US_BACK_SPACE_TURN_AROUND = % d;\n uint8_t OWNRIGHT_BORDER = % d;\n "
                "uint8_t OWNRIGHT_STOP_BORDER = % d;\n uint8_t OWNLEFT_BORDER = % d;\n "
                "uint8_t OWNLEFT_STOP_BORDER = % d;\n uint16_t THRESHOLD_PHOTO = % "
                "d;\n=============================================\n",
                DISTB_TOOLONG_THLESHOLD, DISTB_BACKSTOP_THLESHOLD,
                DISTB_TOOCLOSE_THRESHOLD, BALLY_FRONT_MIN, BALLY_SIDE_THRESHOLD,
                US_BACK_SPACE_TURN_AROUND, OWNRIGHT_BORDER, OWNRIGHT_STOP_BORDER,
                OWNLEFT_BORDER, OWNLEFT_STOP_BORDER, THRESHOLD_PHOTO);
}

void ReceiveThreshold() {

        char c = ui.getc();

        if (c != MODESW) {

                switch (c) {
                case FLG_DISTB_TOOLONG_THLESHOLD:
                        DISTB_TOOLONG_THLESHOLD = sensors.DistB;
                        break;
                case FLG_DISTB_BACKSTOP_THLESHOLD:
                        DISTB_BACKSTOP_THLESHOLD = sensors.DistB;
                        break;
                case FLG_DISTB_TOOCLOSE_THRESHOLD:
                        DISTB_TOOCLOSE_THRESHOLD = sensors.DistB;
                        break;
                case FLG_BALLY_FRONT_MIN:
                        BALLY_FRONT_MIN = IRv.y;
                        break;
                case FLG_BALLY_SIDE_THRESHOLD:
                        BALLY_SIDE_THRESHOLD = IRv.y;
                        break;
                case FLG_US_BACK_SPACE_TURN_AROUND:
                        US_BACK_SPACE_TURN_AROUND = sensors.USDist;
                        break;
                case FLG_OWNRIGHT_BORDER:
                        OWNRIGHT_BORDER = camera.OwnGoalRightEnd;
                        break;
                case FLG_OWNRIGHT_STOP_BORDER:
                        OWNRIGHT_STOP_BORDER = camera.OwnGoalRightEnd;
                        break;
                case FLG_OWNLEFT_BORDER:
                        OWNLEFT_BORDER = camera.OwnGoalLeftEnd;
                        break;
                case FLG_OWNLEFT_STOP_BORDER:
                        OWNLEFT_STOP_BORDER = camera.OwnGoalLeftEnd;
                        break;
                case FLG_THRESHOLD_PHOTO:
                        THRESHOLD_PHOTO = sensors.PhotoSensor;
                        break;
                }
        } else {

                ui.mode = MODESW;
        }
}
