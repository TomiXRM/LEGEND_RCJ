import sensor, image, time
import math
import pyb
from pyb import UART
from pyb import Pin, ExtInt

thresholds2 = [(35, 50, -10, 20, -40, -10)]
thresholds1 = [(55, 75, -20, 16, -20, 10)]
thresholds1 = [(30, 55, -10, 20, -10, 30)]
thresholds =[(20, 79, 29, 105, -24, 92)]####

thresholds =[(20, 50, 20, 70, -10, 20)]#家

thresholds2 = [(35, 50, -10, 20, -40, -10)]
thresholds1 = [(45, 100, 55, -10, -14, 40)]
#thresholds1 = [(30, 55, -10, 20, -10, 30)]


#thresholds =[(55, 95, 26, 78, -24, 30)]####
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
first=4
p=pyb.Pin("P5",pyb.Pin.IN)
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_auto_gain(False,gain_db=19.785863) # must be turned off for color tracking
sensor.set_auto_whitebal(False,rgb_gain_db=(-6.02073,-4.99849,3.566933)) # must be turned off for color tracking
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
    if first==1:
        if p.value()==1:
            thresholds2 = [(20, 45, -12, 31, -54, -23)]
            thresholds1 = [(45, 91, 15, -36, 27, 66)]
        else:
            thresholds1 = [(20, 45, -12, 31, -54, -23)]
            thresholds2 = [(45, 91, 15, -36, 27, 66)]
        thresholds =[(49, 76, 42, 87, 13, 55)]
    if first==3:
        if p.value()==1:
            thresholds2 = [(20, 45, -12, 31, -54, -23)]
            thresholds1 = [(45, 91, 0, 40, 27, 66)]
        else:
            thresholds1 = [(20, 45, -12, 31, -54, -23)]
            thresholds2 = [(45, 91, 0, 40, 27, 66)]
        thresholds =[(49, 76, 42, 87, 13, 55)]
    if first==4:
        if p.value()==1:
            thresholds2 = [(26, 73, -43, 2, -69, -15)]
            thresholds1 = [(33, 81, -20, 32, 42, 80)]
        else:
            thresholds1 = [(26, 73, -43, 2, -69, -15)]
            thresholds2 = [(33, 81, -20, 32, 42, 80)]
        thresholds =[(21, 76, 40, 99, -2, 85)]
    if first==5:
        if p.value()==1:
            thresholds2 = [(26, 73, -43, 2, -69, -15)]
            thresholds1 = [(33, 81, -20, 32, 42, 80)]
        else:
            thresholds1 = [(26, 73, -43, 2, -69, -15)]
            thresholds2 = [(33, 81, -20, 32, 42, 80)]
        thresholds =[(21, 76, 40, 99, -2, 85)]
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
    for blob in img.find_blobs(thresholds1, pixels_threshold=50, area_threshold=50):
        j=j+1
        x=blob.cx()/2
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
                img.draw_rectangle(rem_1)
                img.draw_cross(rem_2, rem_3)
    if j>0:
        degree_1=math.atan2(rem_3-115,rem_2-143)*180/3.14-90
        if degree_1<0:
            degree_1+=360
        goal_cm=math.sqrt((rem_3-115)*(rem_3-115)+(rem_2-143)*(rem_2-143))
    else:
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
                    img.draw_rectangle(rem_1_blue)
                    img.draw_cross(rem_2_blue, rem_3_blue)
        if k>0:
            degree_1=math.atan2(rem_3_blue-115,rem_2_blue-154)*180/3.14+90
            degree_1*=(-1)
            if degree_1<0:
                degree_1+=360
            goal_cm=70
    for blob in img.find_blobs(thresholds, pixels_threshold=3, area_threshold=3):
        x_data=blob.cx()-146
        y_data=blob.cy()-115
        h_rem=blob.h()
        cm_rem=math.atan2(y_data,x_data)*180/3.14-90
        cm_1=math.sqrt(y_data*y_data+x_data*x_data)
        if cm_rem<0 :
            cm_rem+=360
        if cm_1>80 :
            cm_rem+=5000
        else:
            if cm_1>70 :
                cm_rem+=4000
            else:
                if cm_1>60 :
                    cm_rem+=3000
                else:
                    if cm_1>50 :
                        cm_rem+=2000
                    else:
                        if cm_1>0 :
                            cm_rem+=1000
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
    h=h_rem
    cm=cm_rem
    if goal_cm>75:
        area=2
    elif goal_cm>65:
        area=1
    else:
        area=0
    print(goal_cm)
    degree_1+=area*1000
    uart.writechar(77)
    uart.writechar(int(cm) & 0b0000000011111111)
    uart.writechar((int(cm) & 0b1111111100000000)>>8)
    uart.writechar(int(degree_1) & 0b0000000011111111)
    uart.writechar((int(degree_1) & 0b1111111100000000)>>8)
