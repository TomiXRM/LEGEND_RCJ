import sensor, image, time
import math
import pyb
from pyb import UART
from pyb import Pin, ExtInt

thresholds2 = [(35, 50, -10, 20, -40, -10)]
thresholds1 = [(54, 68, -31, -2, -7, 39)]
#thresholds1 = [(30, 55, -10, 20, -10, 30)]


thresholds =[(55, 95, 26, 78, -24, 30)]####
#thresholds =[(61, 95, 21, 71, -25, -6)]
#thresholds =[(20, 50, 20, 70, -10, 20)]#家

k=0
x_data=0
x=0
y=0
x_blue=0
y_blue=0
x_data_1=0
y_data=0
cm=0
cm_1=0
i=0
h=0
i_blue=0
h_blue=0
rem_1=0
rem_2=0
rem_3=0
rem_1_blue=0
rem_2_blue=0
rem_3_blue=0
degree=0
degree_1=0
degree_blue=0
degree_1_blue=0
h_rem=0
cm_rem=0
goal_cm=0
goal_cm_blue=0
area=0
first=2
p=pyb.Pin("P9",pyb.Pin.IN)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False,gain_db=17.785863) # must be turned off for color tracking
sensor.set_auto_whitebal(False,rgb_gain_db=(-6.296998,-5.119987,3.700855)) # must be turned off for color tracking
sensor.set_auto_exposure(False)

sensor.reset()
sensor.set_contrast(-2)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 300)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking

clock = time.clock()
uart = UART(1, 115200, timeout_char = 10)

while(True):
    i=0
    j=0
    k=0
    h_rem=0
    cm_rem=400
    degree_1=400
    goal_cm=0
    rem_2=0
    rem_3=0
    #degree=31
    clock.tick()
    img = sensor.snapshot()
    if first==1:
        if p.value()==1:
            thresholds2 = [(35, 50, -10, 20, -40, -10)]
            thresholds1 = [(44, 72, -48, 23, 22, 74)]
        else:
            thresholds1 = [(35, 50, -10, 20, -40, -10)]
            thresholds2 = [(44, 72, -48, 23, 22, 74)]
        thresholds =[(49, 76, 42, 87, 13, 55)]
    if first==2:
        if p.value()==1:
            thresholds2 = [(35, 50, -10, 20, -40, -10)]
            thresholds1 = [(44, 71, -63, 25, 26, 61)]
            thresholds1 = [(25, 81, -20, 40, 15, 80)]
        else:
            thresholds1 = [(19, 51, -15, 11, -45, -14)]
            thresholds2 = [(44, 71, -63, 25, 26, 61)]
            thresholds2 = [(25, 81, -20, 40, 15, 80)]
        thresholds =[(30, 76, 42, 87, 13, 65)]
    for blob in img.find_blobs(thresholds2, pixels_threshold=50, area_threshold=50):
        k=k+1
        x_blue=blob.cx()-154
        y_blue=blob.cy()-115
        if k==1:
            h_blue=blob.h()
            i_blue=blob.area()
        if h_blue <= blob.h() :
            if i_blue <= blob.area() :
                i_blue=blob.area()
                h_blue=blob.h()
                rem_1_blue=blob.rect()
                degree_blue=int(degree_1_blue)
                rem_2_blue=blob.cx()
                rem_3_blue=blob.cy()
    if k>0:
        degree_1_blue=math.atan2(rem_3_blue-115,rem_2_blue-154)*180/3.14+90
        if degree_1_blue<0:
            degree_1_blue+=360
        goal_cm_blue=math.sqrt((rem_3_blue-115)*(rem_3_blue-115)+(rem_2_blue-154)*(rem_2_blue-154))
        degree_blue=int(degree_1_blue)
    for blob in img.find_blobs(thresholds1, pixels_threshold=100, area_threshold=100):
        j=j+1
        x=blob.cx()-170
        y=blob.cy()-115
        if j==1:
            h=blob.h()
            i=blob.area()
        if h <= blob.h() :
            if i <= blob.area() :
                i=blob.area()
                h=blob.h()
                rem_1=blob.rect()
                degree=int(degree_1)
                rem_2=blob.cx()
                rem_3=blob.cy()
    if j>0:
        img.draw_rectangle(rem_1)
        img.draw_cross(rem_2, rem_3)
        degree_1=blob.cx()/2-3
        if degree_1<0:
            degree_1+=360
        goal_cm=math.sqrt((rem_3-115)*(rem_3-115)+(rem_2-154)*(rem_2-154))
    degree=int(degree_1)
    for blob in img.find_blobs(thresholds, pixels_threshold=3, area_threshold=3):
        x_data=blob.cx()-164
        y_data=blob.cy()-115
        h_rem=blob.h()
        cm_rem=math.atan2(y_data,x_data)*180/3.14+90
        cm_1=math.sqrt(y_data*y_data+x_data*x_data)
        if cm_rem<0 :
            cm_rem+=360
        if cm_1>100 :
            cm_rem+=5000
        else:
            if cm_1>95 :
                cm_rem+=5000
            else:
                if cm_1>85 :
                    cm_rem+=3000
                else:
                    if cm_1>65 :
                        cm_rem+=2000
                    else:
                        if cm_1>0 :
                            cm_rem+=1000
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
    h=h_rem
    cm=cm_rem
    if goal_cm>92:
        area=2
    elif goal_cm>88:
        area=1
    else:
        area=0
    degree+=area*1000
    print(p.value())
    uart.writechar(77)
    uart.writechar(int(cm) & 0b0000000011111111)
    uart.writechar((int(cm) & 0b1111111100000000)>>8)
    uart.writechar(int(degree) & 0b0000000011111111)
    uart.writechar((int(degree) & 0b1111111100000000)>>8)
