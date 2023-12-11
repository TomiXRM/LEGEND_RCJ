#include "TrigonometricTable.h"

float sin32_T(int theta){

        float ret;
        int theta_cal;
        while (theta < 0) {
                theta += 360;
        }
        if(theta >= 360) {
                theta = theta%360;  //余り=0~359の数値
        }
        theta_cal = theta%90;
        if(theta >= 90 && theta < 180) {
                theta_cal = 90 - theta_cal;
        }
        if(theta >=270 && theta < 360) {
                theta_cal = 90 - theta_cal;
        }


        if(theta_cal==0) {ret=SIN0;}
        else if(theta_cal==1) {ret=SIN1;}
        else if(theta_cal==2) {ret=SIN2;}
        else if(theta_cal==3) {ret=SIN3;}
        else if(theta_cal==4) {ret=SIN4;}
        else if(theta_cal==5) {ret=SIN5;}
        else if(theta_cal==6) {ret=SIN6;}
        else if(theta_cal==7) {ret=SIN7;}
        else if(theta_cal==8) {ret=SIN8;}
        else if(theta_cal==9) {ret=SIN9;}
        else if(theta_cal==10) {ret=SIN10;}
        else if(theta_cal==11) {ret=SIN11;}
        else if(theta_cal==12) {ret=SIN12;}
        else if(theta_cal==13) {ret=SIN13;}
        else if(theta_cal==14) {ret=SIN14;}
        else if(theta_cal==15) {ret=SIN15;}
        else if(theta_cal==16) {ret=SIN16;}
        else if(theta_cal==17) {ret=SIN17;}
        else if(theta_cal==18) {ret=SIN18;}
        else if(theta_cal==19) {ret=SIN19;}
        else if(theta_cal==20) {ret=SIN20;}
        else if(theta_cal==21) {ret=SIN21;}
        else if(theta_cal==22) {ret=SIN22;}
        else if(theta_cal==23) {ret=SIN23;}
        else if(theta_cal==24) {ret=SIN24;}
        else if(theta_cal==25) {ret=SIN25;}
        else if(theta_cal==26) {ret=SIN26;}
        else if(theta_cal==27) {ret=SIN27;}
        else if(theta_cal==28) {ret=SIN28;}
        else if(theta_cal==29) {ret=SIN29;}
        else if(theta_cal==30) {ret=SIN30;}
        else if(theta_cal==31) {ret=SIN31;}
        else if(theta_cal==32) {ret=SIN32;}
        else if(theta_cal==33) {ret=SIN33;}
        else if(theta_cal==34) {ret=SIN34;}
        else if(theta_cal==35) {ret=SIN35;}
        else if(theta_cal==36) {ret=SIN36;}
        else if(theta_cal==37) {ret=SIN37;}
        else if(theta_cal==38) {ret=SIN38;}
        else if(theta_cal==39) {ret=SIN39;}
        else if(theta_cal==40) {ret=SIN40;}
        else if(theta_cal==41) {ret=SIN41;}
        else if(theta_cal==42) {ret=SIN42;}
        else if(theta_cal==43) {ret=SIN43;}
        else if(theta_cal==44) {ret=SIN44;}
        else if(theta_cal==45) {ret=SIN45;}
        else if(theta_cal==46) {ret=SIN46;}
        else if(theta_cal==47) {ret=SIN47;}
        else if(theta_cal==48) {ret=SIN48;}
        else if(theta_cal==49) {ret=SIN49;}
        else if(theta_cal==50) {ret=SIN50;}
        else if(theta_cal==51) {ret=SIN51;}
        else if(theta_cal==52) {ret=SIN52;}
        else if(theta_cal==53) {ret=SIN53;}
        else if(theta_cal==54) {ret=SIN54;}
        else if(theta_cal==55) {ret=SIN55;}
        else if(theta_cal==56) {ret=SIN56;}
        else if(theta_cal==57) {ret=SIN57;}
        else if(theta_cal==58) {ret=SIN58;}
        else if(theta_cal==59) {ret=SIN59;}
        else if(theta_cal==60) {ret=SIN60;}
        else if(theta_cal==61) {ret=SIN61;}
        else if(theta_cal==62) {ret=SIN62;}
        else if(theta_cal==63) {ret=SIN63;}
        else if(theta_cal==64) {ret=SIN64;}
        else if(theta_cal==65) {ret=SIN65;}
        else if(theta_cal==66) {ret=SIN66;}
        else if(theta_cal==67) {ret=SIN67;}
        else if(theta_cal==68) {ret=SIN68;}
        else if(theta_cal==69) {ret=SIN69;}
        else if(theta_cal==70) {ret=SIN70;}
        else if(theta_cal==71) {ret=SIN71;}
        else if(theta_cal==72) {ret=SIN72;}
        else if(theta_cal==73) {ret=SIN73;}
        else if(theta_cal==74) {ret=SIN74;}
        else if(theta_cal==75) {ret=SIN75;}
        else if(theta_cal==76) {ret=SIN76;}
        else if(theta_cal==77) {ret=SIN77;}
        else if(theta_cal==78) {ret=SIN78;}
        else if(theta_cal==79) {ret=SIN79;}
        else if(theta_cal==80) {ret=SIN80;}
        else if(theta_cal==81) {ret=SIN81;}
        else if(theta_cal==82) {ret=SIN82;}
        else if(theta_cal==83) {ret=SIN83;}
        else if(theta_cal==84) {ret=SIN84;}
        else if(theta_cal==85) {ret=SIN85;}
        else if(theta_cal==86) {ret=SIN86;}
        else if(theta_cal==87) {ret=SIN87;}
        else if(theta_cal==88) {ret=SIN88;}
        else if(theta_cal==89) {ret=SIN89;}
        else if(theta_cal==90) {ret=SIN90;}


        if(theta >= 0 && theta <= 90) {
                //0~90
        }else if(theta > 90 && theta <= 180) {
                //91~180
        }else if(theta > 180 && theta <= 270) {
                //181~270
                ret = -ret;
        }else if(theta > 270 && theta < 360) {
                //271~360
                ret = -ret;
        }
        return ret;
}

float cos32_T(int theta){

        float ret;
        int theta_cal;
        while (theta < 0) {
                theta += 360;
        }
        if(theta >= 360) {
                theta = theta%360;  //余り=0~359の数値
        }
        theta_cal = theta%90;
        if(theta >=90 && theta < 180) {
                theta_cal = 90 - theta_cal;
        }
        if(theta >=270 && theta < 360) {
                theta_cal = 90 - theta_cal;
        }

        if(theta_cal==0) {ret=COS0;}
        else if(theta_cal==1) {ret=COS1;}
        else if(theta_cal==2) {ret=COS2;}
        else if(theta_cal==3) {ret=COS3;}
        else if(theta_cal==4) {ret=COS4;}
        else if(theta_cal==5) {ret=COS5;}
        else if(theta_cal==6) {ret=COS6;}
        else if(theta_cal==7) {ret=COS7;}
        else if(theta_cal==8) {ret=COS8;}
        else if(theta_cal==9) {ret=COS9;}
        else if(theta_cal==10) {ret=COS10;}
        else if(theta_cal==11) {ret=COS11;}
        else if(theta_cal==12) {ret=COS12;}
        else if(theta_cal==13) {ret=COS13;}
        else if(theta_cal==14) {ret=COS14;}
        else if(theta_cal==15) {ret=COS15;}
        else if(theta_cal==16) {ret=COS16;}
        else if(theta_cal==17) {ret=COS17;}
        else if(theta_cal==18) {ret=COS18;}
        else if(theta_cal==19) {ret=COS19;}
        else if(theta_cal==20) {ret=COS20;}
        else if(theta_cal==21) {ret=COS21;}
        else if(theta_cal==22) {ret=COS22;}
        else if(theta_cal==23) {ret=COS23;}
        else if(theta_cal==24) {ret=COS24;}
        else if(theta_cal==25) {ret=COS25;}
        else if(theta_cal==26) {ret=COS26;}
        else if(theta_cal==27) {ret=COS27;}
        else if(theta_cal==28) {ret=COS28;}
        else if(theta_cal==29) {ret=COS29;}
        else if(theta_cal==30) {ret=COS30;}
        else if(theta_cal==31) {ret=COS31;}
        else if(theta_cal==32) {ret=COS32;}
        else if(theta_cal==33) {ret=COS33;}
        else if(theta_cal==34) {ret=COS34;}
        else if(theta_cal==35) {ret=COS35;}
        else if(theta_cal==36) {ret=COS36;}
        else if(theta_cal==37) {ret=COS37;}
        else if(theta_cal==38) {ret=COS38;}
        else if(theta_cal==39) {ret=COS39;}
        else if(theta_cal==40) {ret=COS40;}
        else if(theta_cal==41) {ret=COS41;}
        else if(theta_cal==42) {ret=COS42;}
        else if(theta_cal==43) {ret=COS43;}
        else if(theta_cal==44) {ret=COS44;}
        else if(theta_cal==45) {ret=COS45;}
        else if(theta_cal==46) {ret=COS46;}
        else if(theta_cal==47) {ret=COS47;}
        else if(theta_cal==48) {ret=COS48;}
        else if(theta_cal==49) {ret=COS49;}
        else if(theta_cal==50) {ret=COS50;}
        else if(theta_cal==51) {ret=COS51;}
        else if(theta_cal==52) {ret=COS52;}
        else if(theta_cal==53) {ret=COS53;}
        else if(theta_cal==54) {ret=COS54;}
        else if(theta_cal==55) {ret=COS55;}
        else if(theta_cal==56) {ret=COS56;}
        else if(theta_cal==57) {ret=COS57;}
        else if(theta_cal==58) {ret=COS58;}
        else if(theta_cal==59) {ret=COS59;}
        else if(theta_cal==60) {ret=COS60;}
        else if(theta_cal==61) {ret=COS61;}
        else if(theta_cal==62) {ret=COS62;}
        else if(theta_cal==63) {ret=COS63;}
        else if(theta_cal==64) {ret=COS64;}
        else if(theta_cal==65) {ret=COS65;}
        else if(theta_cal==66) {ret=COS66;}
        else if(theta_cal==67) {ret=COS67;}
        else if(theta_cal==68) {ret=COS68;}
        else if(theta_cal==69) {ret=COS69;}
        else if(theta_cal==70) {ret=COS70;}
        else if(theta_cal==71) {ret=COS71;}
        else if(theta_cal==72) {ret=COS72;}
        else if(theta_cal==73) {ret=COS73;}
        else if(theta_cal==74) {ret=COS74;}
        else if(theta_cal==75) {ret=COS75;}
        else if(theta_cal==76) {ret=COS76;}
        else if(theta_cal==77) {ret=COS77;}
        else if(theta_cal==78) {ret=COS78;}
        else if(theta_cal==79) {ret=COS79;}
        else if(theta_cal==80) {ret=COS80;}
        else if(theta_cal==81) {ret=COS81;}
        else if(theta_cal==82) {ret=COS82;}
        else if(theta_cal==83) {ret=COS83;}
        else if(theta_cal==84) {ret=COS84;}
        else if(theta_cal==85) {ret=COS85;}
        else if(theta_cal==86) {ret=COS86;}
        else if(theta_cal==87) {ret=COS87;}
        else if(theta_cal==88) {ret=COS88;}
        else if(theta_cal==89) {ret=COS89;}
        else if(theta_cal==90) {ret=COS90;}

        if(theta >= 0 && theta <= 90) {
                //0~90
        }else if(theta > 90 && theta <= 180) {
                //91~180
                ret = -ret;
        }else if(theta > 180 && theta <= 270) {
                //181~270
                ret = -ret;
        }else if(theta > 270 && theta < 360) {
                //271~360
        }
        return ret;
}
