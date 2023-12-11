#include "Vector.h"


VectorXY SetZero() {
        VectorXY rslt;
        rslt.x = 0;
        rslt.y = 0;
        rslt.r = 0;
        return rslt;
}

VectorXY makeXYFromRT(short radius, short theta) {

        VectorXY rslt;
        theta += 90;
        rslt.x = (short)(radius * cos(theta / 180.0 * PI));
        rslt.y = (short)(radius * sin(theta / 180.0 * PI));

        return rslt;
}

VectorXY makeXY(short x, short y) {

        VectorXY rslt;
        rslt.x = x;
        rslt.y = y;

        return rslt;
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

float _sin(int theta){

        float ret;
        int theta_cal;
        while (theta < 0) {
                theta += 360;
        }
        if(theta >= 360) {
                theta = theta%360;//余り=0~359の数値
        }

        theta_cal = theta%90;
        if(theta >= 90 && theta < 180) {
                theta_cal = 90 - theta_cal;
        }
        if(theta >=270 && theta < 360) {
                theta_cal = 90 - theta_cal;
        }


        if(theta_cal==0) {ret=sin0;}
        else if(theta_cal==1) {ret=sin1;}
        else if(theta_cal==2) {ret=sin2;}
        else if(theta_cal==3) {ret=sin3;}
        else if(theta_cal==4) {ret=sin4;}
        else if(theta_cal==5) {ret=sin5;}
        else if(theta_cal==6) {ret=sin6;}
        else if(theta_cal==7) {ret=sin7;}
        else if(theta_cal==8) {ret=sin8;}
        else if(theta_cal==9) {ret=sin9;}
        else if(theta_cal==10) {ret=sin10;}
        else if(theta_cal==11) {ret=sin11;}
        else if(theta_cal==12) {ret=sin12;}
        else if(theta_cal==13) {ret=sin13;}
        else if(theta_cal==14) {ret=sin14;}
        else if(theta_cal==15) {ret=sin15;}
        else if(theta_cal==16) {ret=sin16;}
        else if(theta_cal==17) {ret=sin17;}
        else if(theta_cal==18) {ret=sin18;}
        else if(theta_cal==19) {ret=sin19;}
        else if(theta_cal==20) {ret=sin20;}
        else if(theta_cal==21) {ret=sin21;}
        else if(theta_cal==22) {ret=sin22;}
        else if(theta_cal==23) {ret=sin23;}
        else if(theta_cal==24) {ret=sin24;}
        else if(theta_cal==25) {ret=sin25;}
        else if(theta_cal==26) {ret=sin26;}
        else if(theta_cal==27) {ret=sin27;}
        else if(theta_cal==28) {ret=sin28;}
        else if(theta_cal==29) {ret=sin29;}
        else if(theta_cal==30) {ret=sin30;}
        else if(theta_cal==31) {ret=sin31;}
        else if(theta_cal==32) {ret=sin32;}
        else if(theta_cal==33) {ret=sin33;}
        else if(theta_cal==34) {ret=sin34;}
        else if(theta_cal==35) {ret=sin35;}
        else if(theta_cal==36) {ret=sin36;}
        else if(theta_cal==37) {ret=sin37;}
        else if(theta_cal==38) {ret=sin38;}
        else if(theta_cal==39) {ret=sin39;}
        else if(theta_cal==40) {ret=sin40;}
        else if(theta_cal==41) {ret=sin41;}
        else if(theta_cal==42) {ret=sin42;}
        else if(theta_cal==43) {ret=sin43;}
        else if(theta_cal==44) {ret=sin44;}
        else if(theta_cal==45) {ret=sin45;}
        else if(theta_cal==46) {ret=sin46;}
        else if(theta_cal==47) {ret=sin47;}
        else if(theta_cal==48) {ret=sin48;}
        else if(theta_cal==49) {ret=sin49;}
        else if(theta_cal==50) {ret=sin50;}
        else if(theta_cal==51) {ret=sin51;}
        else if(theta_cal==52) {ret=sin52;}
        else if(theta_cal==53) {ret=sin53;}
        else if(theta_cal==54) {ret=sin54;}
        else if(theta_cal==55) {ret=sin55;}
        else if(theta_cal==56) {ret=sin56;}
        else if(theta_cal==57) {ret=sin57;}
        else if(theta_cal==58) {ret=sin58;}
        else if(theta_cal==59) {ret=sin59;}
        else if(theta_cal==60) {ret=sin60;}
        else if(theta_cal==61) {ret=sin61;}
        else if(theta_cal==62) {ret=sin62;}
        else if(theta_cal==63) {ret=sin63;}
        else if(theta_cal==64) {ret=sin64;}
        else if(theta_cal==65) {ret=sin65;}
        else if(theta_cal==66) {ret=sin66;}
        else if(theta_cal==67) {ret=sin67;}
        else if(theta_cal==68) {ret=sin68;}
        else if(theta_cal==69) {ret=sin69;}
        else if(theta_cal==70) {ret=sin70;}
        else if(theta_cal==71) {ret=sin71;}
        else if(theta_cal==72) {ret=sin72;}
        else if(theta_cal==73) {ret=sin73;}
        else if(theta_cal==74) {ret=sin74;}
        else if(theta_cal==75) {ret=sin75;}
        else if(theta_cal==76) {ret=sin76;}
        else if(theta_cal==77) {ret=sin77;}
        else if(theta_cal==78) {ret=sin78;}
        else if(theta_cal==79) {ret=sin79;}
        else if(theta_cal==80) {ret=sin80;}
        else if(theta_cal==81) {ret=sin81;}
        else if(theta_cal==82) {ret=sin82;}
        else if(theta_cal==83) {ret=sin83;}
        else if(theta_cal==84) {ret=sin84;}
        else if(theta_cal==85) {ret=sin85;}
        else if(theta_cal==86) {ret=sin86;}
        else if(theta_cal==87) {ret=sin87;}
        else if(theta_cal==88) {ret=sin88;}
        else if(theta_cal==89) {ret=sin89;}
        else if(theta_cal==90) {ret=sin90;}


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

float _cos(int theta){

        float ret;
        int theta_cal;
        while (theta < 0) {
                theta += 360;
        }
        if(theta >= 360) {
                theta = theta%360;//余り=0~359の数値
        }
        theta_cal = theta%90;
        if(theta >=90 && theta < 180) {
                theta_cal = 90 - theta_cal;
        }
        if(theta >=270 && theta < 360) {
                theta_cal = 90 - theta_cal;
        }

        if(theta_cal==0) {ret=cos0;}
        else if(theta_cal==1) {ret=cos1;}
        else if(theta_cal==2) {ret=cos2;}
        else if(theta_cal==3) {ret=cos3;}
        else if(theta_cal==4) {ret=cos4;}
        else if(theta_cal==5) {ret=cos5;}
        else if(theta_cal==6) {ret=cos6;}
        else if(theta_cal==7) {ret=cos7;}
        else if(theta_cal==8) {ret=cos8;}
        else if(theta_cal==9) {ret=cos9;}
        else if(theta_cal==10) {ret=cos10;}
        else if(theta_cal==11) {ret=cos11;}
        else if(theta_cal==12) {ret=cos12;}
        else if(theta_cal==13) {ret=cos13;}
        else if(theta_cal==14) {ret=cos14;}
        else if(theta_cal==15) {ret=cos15;}
        else if(theta_cal==16) {ret=cos16;}
        else if(theta_cal==17) {ret=cos17;}
        else if(theta_cal==18) {ret=cos18;}
        else if(theta_cal==19) {ret=cos19;}
        else if(theta_cal==20) {ret=cos20;}
        else if(theta_cal==21) {ret=cos21;}
        else if(theta_cal==22) {ret=cos22;}
        else if(theta_cal==23) {ret=cos23;}
        else if(theta_cal==24) {ret=cos24;}
        else if(theta_cal==25) {ret=cos25;}
        else if(theta_cal==26) {ret=cos26;}
        else if(theta_cal==27) {ret=cos27;}
        else if(theta_cal==28) {ret=cos28;}
        else if(theta_cal==29) {ret=cos29;}
        else if(theta_cal==30) {ret=cos30;}
        else if(theta_cal==31) {ret=cos31;}
        else if(theta_cal==32) {ret=cos32;}
        else if(theta_cal==33) {ret=cos33;}
        else if(theta_cal==34) {ret=cos34;}
        else if(theta_cal==35) {ret=cos35;}
        else if(theta_cal==36) {ret=cos36;}
        else if(theta_cal==37) {ret=cos37;}
        else if(theta_cal==38) {ret=cos38;}
        else if(theta_cal==39) {ret=cos39;}
        else if(theta_cal==40) {ret=cos40;}
        else if(theta_cal==41) {ret=cos41;}
        else if(theta_cal==42) {ret=cos42;}
        else if(theta_cal==43) {ret=cos43;}
        else if(theta_cal==44) {ret=cos44;}
        else if(theta_cal==45) {ret=cos45;}
        else if(theta_cal==46) {ret=cos46;}
        else if(theta_cal==47) {ret=cos47;}
        else if(theta_cal==48) {ret=cos48;}
        else if(theta_cal==49) {ret=cos49;}
        else if(theta_cal==50) {ret=cos50;}
        else if(theta_cal==51) {ret=cos51;}
        else if(theta_cal==52) {ret=cos52;}
        else if(theta_cal==53) {ret=cos53;}
        else if(theta_cal==54) {ret=cos54;}
        else if(theta_cal==55) {ret=cos55;}
        else if(theta_cal==56) {ret=cos56;}
        else if(theta_cal==57) {ret=cos57;}
        else if(theta_cal==58) {ret=cos58;}
        else if(theta_cal==59) {ret=cos59;}
        else if(theta_cal==60) {ret=cos60;}
        else if(theta_cal==61) {ret=cos61;}
        else if(theta_cal==62) {ret=cos62;}
        else if(theta_cal==63) {ret=cos63;}
        else if(theta_cal==64) {ret=cos64;}
        else if(theta_cal==65) {ret=cos65;}
        else if(theta_cal==66) {ret=cos66;}
        else if(theta_cal==67) {ret=cos67;}
        else if(theta_cal==68) {ret=cos68;}
        else if(theta_cal==69) {ret=cos69;}
        else if(theta_cal==70) {ret=cos70;}
        else if(theta_cal==71) {ret=cos71;}
        else if(theta_cal==72) {ret=cos72;}
        else if(theta_cal==73) {ret=cos73;}
        else if(theta_cal==74) {ret=cos74;}
        else if(theta_cal==75) {ret=cos75;}
        else if(theta_cal==76) {ret=cos76;}
        else if(theta_cal==77) {ret=cos77;}
        else if(theta_cal==78) {ret=cos78;}
        else if(theta_cal==79) {ret=cos79;}
        else if(theta_cal==80) {ret=cos80;}
        else if(theta_cal==81) {ret=cos81;}
        else if(theta_cal==82) {ret=cos82;}
        else if(theta_cal==83) {ret=cos83;}
        else if(theta_cal==84) {ret=cos84;}
        else if(theta_cal==85) {ret=cos85;}
        else if(theta_cal==86) {ret=cos86;}
        else if(theta_cal==87) {ret=cos87;}
        else if(theta_cal==88) {ret=cos88;}
        else if(theta_cal==89) {ret=cos89;}
        else if(theta_cal==90) {ret=cos90;}

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
