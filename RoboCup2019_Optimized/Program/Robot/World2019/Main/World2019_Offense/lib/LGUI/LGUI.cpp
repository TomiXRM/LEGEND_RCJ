#include "mbed.h"
#include "LGUI.h"
#include "setup_com.h"

LGUI::LGUI(RawSerial* _pc,DigitalOut *_slp,PinName tx,PinName rx) : device(tx,rx){
        pc=_pc;
        slp = _slp;
        device.attach(callback(this,&LGUI::ModeSelect),Serial::RxIrq);
        modeLast=MODESW;
        mode = MODESW;
        speed = 100;
        data_length = 0;
        state = 0;
        SetBZ(7);//ブザーを鳴らさないように
        Entry(MODESW,callback(this,&LGUI::Before_ModeSW),callback(this,&LGUI::Body_ModeSW),DONOTHING);
}

void LGUI::attachRx (Callback< void()> method){
        device.attach(method,Serial::RxIrq);
};

//Wrappers
void LGUI::putc(char c){
        device.putc(c);
}



void doNothing(){

}

char LGUI::getc(){
        return device.getc();
}

void LGUI::baud(uint64_t baudLate){
        device.baud(baudLate);
}


/*LGUI::Entry.
   You can add mode with this function.
   As 1st argument,set an iddentifer character of the mode.
   2nd,set the function that should be called before running.
   3rd,set the function that should be called once a loop session.
   4th,set the function that should be called after the mode has changed.

   If you don't need to do something in 2nd to 4th argument,you can put there DONOTHING.
 */


void LGUI::Entry(char c,Callback<void()> before, Callback<void()>loop,Callback<void()> after){

//Setup values.
        modeList.push_back(c);
        Before.push_back(before);
        Loop.push_back(loop);
        After.push_back(after);

}

//Find index character from the modeList array.
//When not found,returns 255;
unsigned char LGUI::findIndex(char index){

        unsigned char ArrayLength = (sizeof(modeList)/sizeof(modeList[0]));//Set "How many modes?""
        unsigned char i=0;

        for(i=0; i<ArrayLength; i++) {
                //Scan all
                if(modeList[i]==index) {
                        //Found
                        return i;
                }
        }

        //Not found
        return 255;
}


void LGUI::Run(){

        unsigned char i_mode=findIndex(mode);

        if(i_mode==255) {

//ERROR! おかしな文字が来やがった!!

                pc->printf("[ERROR]{LGUI} Unknown mode character. '%c'.\n Switch for modesw.",mode);//とりまprint
                (After[findIndex(modeLast)])();//前の生きてた頃の片付け(=前のモードの遺品整理)
                modeLast=MODESW;//モードを強制的に修正(生きてたモードの影を消してクラッシュの痕跡をなくす)
                mode=MODESW;//MODESWモードに強制切り替え
                (Before[findIndex(MODESW)])();//MODESWにするのでその準備

                //*この後、modeLast==modeになるので普通にMODESWのloopが実行され、街には平和が戻る。


        }else{
                if(modeLast!=mode) {//Mode diff check

                        unsigned char i_modeBefore=findIndex(modeLast);

                        //AfterMode function
                        After[i_modeBefore]();

                        //Setup of next mode
                        (Before[i_mode])();


                        modeLast=mode;                    //Initialize

                }else{
                        (Loop[i_mode])();
                }

        }

}

void LGUI::SetLED(bool r, bool g, bool b){
        bitWrite(state, 0, r);
        bitWrite(state, 1, g);
        bitWrite(state, 2, b);
}

void LGUI::SetBZ(uint8_t i){
        switch (i) {
        case 1: bitWrite(state, 5, 1);
                bitWrite(state, 6, 0);
                bitWrite(state, 7, 0); break;
        case 2: bitWrite(state, 5, 0);
                bitWrite(state, 6, 1);
                bitWrite(state, 7, 0); break;
        case 3: bitWrite(state, 5, 1);
                bitWrite(state, 6, 1);
                bitWrite(state, 7, 0); break;
        case 4: bitWrite(state, 5, 0);
                bitWrite(state, 6, 0);
                bitWrite(state, 7, 1); break;
        case 5: bitWrite(state, 5, 1);
                bitWrite(state, 6, 0);
                bitWrite(state, 7, 1); break;
        case 6: bitWrite(state, 5, 0);
                bitWrite(state, 6, 1);
                bitWrite(state, 7, 1); break;
        case 7: bitWrite(state, 5, 1);
                bitWrite(state, 6, 1);
                bitWrite(state, 7, 1); break;
        case 0: default:
                bitWrite(state, 5, 0);
                bitWrite(state, 6, 0);
                bitWrite(state, 7, 0); break;
        }
}

void LGUI::SetKICKER(bool flg){
        bitWrite(state, 4, flg);
}

void LGUI::Before_ModeSW(){
        device.attach(callback(this,&LGUI::ModeSelect),Serial::RxIrq);
}

void LGUI::Body_ModeSW(){
        pc->printf("MODESW!!\r\n");
        slp->write(0);
}

//MODESWで呼び出しする関数
void LGUI::ModeSelect(){
        modeLast=mode;//Save on latest mode buffer.
        mode = getc();//Renew Mode character
        pc->printf("%c\r\n",mode);
}
