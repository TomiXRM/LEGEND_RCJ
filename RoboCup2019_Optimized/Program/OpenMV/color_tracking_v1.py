import sensor, image, time , math , pyb, micropython
from pyb import ExtInt
from pyb import UART
from pyb import LED

#床からopenMVの基盤まで11cm

#閾値 Thresholds　= [(L Min, L Max, A Min, A Max, B Min, B Max)]

#1 自分のゴール blue
thr_1 = [(21, 31, -3, 13, -42, -7)]

#2 相手のゴール yellow
thr_2 = [(71, 84, -8, 14, 15, 78)]


cen_x = 144 #カメラの中心ｘ座標
cen_y = 122 #カメラの中心ｙ座標


sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 1000)
sensor.set_contrast(0)
sensor.set_brightness(0)
sensor.set_saturation(0)#-3~3
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
micropython.alloc_emergency_exception_buf(100)
clock = time.clock()
usb = pyb.USB_VCP()
led = pyb.LED(1)
uart = UART(1, 9600)
uart.init(9600, bits = 8, parity = None, stop = 1)

#関数定義

def sear (thr):
    big_Area = 0
    big_x = 0
    big_y = 0
    for goal in img.find_blobs(thr, pixels_threshold = 500, area_threshold = 500):
        img.draw_rectangle(goal.rect())
        img.draw_cross(goal.cx(), goal.cy())
        Area = goal.h() * goal.w()
        if(Area > big_Area):
            big_Area = Area
            big_x = goal.cx() - cen_x
            big_y = cen_y - goal.cy()
    return big_x, big_y

def modi (cam_x, cam_y):
    dis = math.sqrt(cam_x ** 2 + cam_y ** 2)
    rad = math.atan2(cam_x, cam_y)
    deg = math.degrees(rad)
    if(dis == 0):
        mov_x = 0
        mov_y = 0
    elif(dis <= 82):
        far = 35 / 6724 * (dis ** 2)
        mov_x = far * cam_x / dis
        mov_y = far * cam_y / dis
    elif(dis > 82):
        far = 4 * dis - 293
        mov_x = far * cam_x / dis
        mov_y = far * cam_y / dis

    return mov_x, mov_y, deg

def calc (mov_1_x, mov_1_y, mov_2_x, mov_2_y):
    global cou_x
    global cou_y
    cou_x = 0
    cou_y = 0
    if(mov_1_y == 0 and mov_2_y == 0):
        cou_x = -100
        cou_y = -100
    elif(mov_1_y > 0 and mov_2_y > 0):
        tan_2 = mov_2_y / mov_2_x
        tan_1 = mov_1_y / mov_1_x
        cou_x = 61 + 184 / (tan_2 - tan_1)
        cou_y = -184 * tan_1 / (tan_2 - tan_1)
    elif(mov_1_y == 0 and mov_2_y > 0):
        cou_x = 61 + mov_2_x
        cou_y = 184 - mov_2_y
    elif(mov_1_y > 0 and mov_2_y == 0):
        cou_x = 61 + mov_1_x
        cou_y = -1 * mov_1_y

    return cou_x, cou_y

def callback(line):
    #print(x, y, degree_2)
    uart.write('C')
    uart.writechar(x)
    uart.writechar(y)
    uart.writechar(degree_2)
#usb.send(x, y, degree_2 \n)

#uart.writechar( n & 0b0000000011111111)
#uart.writechar((n & 0b1111111100000000)>>8)

#割り込み

exint = pyb.ExtInt("P4", ExtInt.IRQ_RISING, pyb.Pin.PULL_DOWN, callback)
pyb.enable_irq(True)


while(True):
    clock.tick()
    img = sensor.snapshot()
    cam_2_x, caS_2_y = sear(thr_2)
    cam_1_x, cam_1_y = sear(thr_1)
    mov_2_x, mov_2_y, degree_2 = modi(cam_2_x, cam_2_y)
    mov_1_x, mov_1_y, degree_1 = modi(cam_1_x, cam_1_y)
    x, y = calc (mov_1_x, mov_1_y, mov_2_x, mov_2_y)
    x = int(x)
    y = int(y)
    degree_2 = int(degree_2)

    print("%d," %x,end="")
    print("%d" %y)
