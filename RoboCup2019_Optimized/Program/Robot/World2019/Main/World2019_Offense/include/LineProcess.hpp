#include "setup_com.h"
#include "setup.h"

extern LGUI ui;
extern LineManage Line;


void Body_Line();

void initLineProcess(){
        ui.Entry(LINE,DONOTHING,callback(&Body_Line),DONOTHING);
}

void Body_Line(){
        // pc.printf("Line Mode!!\r\n");
        pc.printf("%d%d%d%d\r\n",Line.front,Line.right,Line.back,Line.left);
}
