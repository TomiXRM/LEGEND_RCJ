import sensor, image, time , math , pyb, micropython
from pyb import ExtInt
from pyb import UART
from pyb import LED

#床からopenMVの基盤まで11cm

#閾値 Thresholds　= [(L Min, L Max, A Min, A Max, B Min, B Max)]

#42.72-2.53-9.68

Ops_threshold = [(66, 98, -32, 10, 24, 98)]    #yellow
#Ops_threshold = [(45, 64, -16, 19, 24, 63)]    #yellowhotel
Own_threshold= [(28, 78, -37, 48, -88, -37)]     #blue


cen_x = 161 #カメラの中心ｘ座標
cen_y = 124 #カメラの中心ｙ座標
OwnX = 0
OwnY = 0
OpsX = 0
OpsY = 0

OwnW = 0    #自ゴールの幅
OpsW = 0    #相手ゴールの幅
OwnGoalRightEnd = 0 #自ゴールの右端
OwnGoalLeftEnd = 0  #自ゴールの左端
CenterPerOwnGoal = 0 #自ゴールに置ける自分の位置


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
#aboud skip_frames([n,time])
#Takes n number of snapshots to let the camera image stabilize after changing camera settings. n is passed as normal argument, e.g. skip_frames(10) to skip 10 frames. You should call this function after changing camera settings.

#Alternatively, you can pass the keyword argument time to skip frames for some number of milliseconds, e.g. skip_frames(time = 2000) to skip frames for 2000 milliseconds.

#If neither n nor time is specified this method skips frames for 300 milliseconds.

#If both are specified this method skips n number of frames but will timeout after time milliseconds.
#skip_frames(10)は10回分のフレームをスキップ
#skip_frames(time = 10)は10ミリ秒分の時間スキップ
#time = 1000の場合は1秒分スキップすることになるので俊敏性にかけるかと思われます。
#データシート読めマジで大会終わってから気づくことじゃないぞ (https://docs.openmv.io/library/omv.sensor.html)

sensor.skip_frames(time = 1000)
sensor.set_contrast(2)
sensor.set_brightness(0)
sensor.set_saturation(3)#-3~3
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
micropython.alloc_emergency_exception_buf(100)
clock = time.clock()
usb = pyb.USB_VCP()
led = pyb.LED(1)
uart = UART(1, 230400)
uart.init(230400, bits = 8, parity = None, stop = 1)

#関数定義

def sear (thr):
    big_Area = 0
    big_x = 0
    big_y = 0
    big_w = 0
    for goal in img.find_blobs(thr, pixels_threshold = 100, area_threshold = 100,marge = True):
        img.draw_rectangle(goal.rect())
        img.draw_cross(goal.cx(), goal.cy())
        Area = goal.h() * goal.w()
        if(Area > big_Area):
            big_Area = Area
            big_x = (goal.cx() - cen_x)
            big_y = -(cen_y - goal.cy())
            big_w = goal.w()
    if(big_x == 0 and big_y == 0):
        big_x = -1
        big_y = -1
    return big_x, big_y ,big_w

#def callback(line):
    #print(OwnX, OwnY ,OpsX, OpsY)

    #uart.write('A')
    #uart.writechar(OwnX)
    #uart.writechar(OwnY)
    #uart.writechar(OwnW)
    #uart.writechar(OpsX)
    #uart.writechar(OpsY)
    #uart.writechar(OpsW)


#uart.writechar( n & 0b0000000011111111)
#uart.writechar((n & 0b1111111100000000)>>8)

#割り込み
#exint = pyb.ExtInt("P4", ExtInt.IRQ_RISING, pyb.Pin.PULL_DOWN, callback)
pyb.enable_irq(True)


while(True):
    clock.tick()
    img = sensor.snapshot()#.rotation_corr(y_rotation = 180)
    img.draw_cross(cen_x, cen_y)
    OwnX, OwnY ,OwnW = sear(Own_threshold)
    OpsX, OpsY ,OpsW = sear(Ops_threshold)


    OwnGoalRightEnd = OwnX + (OwnW/2)
    OwnGoalLeftEnd  = OwnX - (OwnW/2)
    #if(int(OwnX) != 0):
    #    CenterPerOwnGoal = (OwnGoalRightEnd + OwnGoalLeftEnd)/2
    #print(OwnX, OwnY,OwnW,int(OwnGoalRightEnd),int(OwnGoalLeftEnd),int(CenterPerOwnGoal))
    print(OpsX,OpsY,OpsW,clock.fps())

    uart.write('A')
    uart.writechar(OwnX)
    uart.writechar(OwnY)
    uart.writechar(OwnW)
    uart.writechar(OpsX)
    uart.writechar(OpsY)
    uart.writechar(OpsW)
