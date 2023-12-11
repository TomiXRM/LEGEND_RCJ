import sensor, image, time , math , pyb, micropython
from pyb import ExtInt
from pyb import UART
from pyb import LED

#床からopenMVの基盤まで11cm

#閾値 Thresholds　= [(L Min, L Max, A Min, A Max, B Min, B Max)]

#1 自分のゴール
#2 相手のゴール
thr_2 = [(19, 36, -42, 19, -35, -3)]#blue
thr_1 = [(52, 100, -36, 12, 17, 58)]#yellow


cen_x = 155 #カメラの中心ｘ座標
cen_y = 125 #カメラの中心ｙ座標
x_1 = 0
y_1 = 0
x_2 = 0
y_2 = 0


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
uart = UART(1, 115200)
uart.init(115200, bits = 8, parity = None, stop = 1)

#関数定義

def sear (thr):
    big_Area = 0
    big_x = 0
    big_y = 0
    for goal in img.find_blobs(thr, pixels_threshold = 100, area_threshold = 100):
        img.draw_rectangle(goal.rect())
        img.draw_cross(goal.cx(), goal.cy())
        Area = goal.h() * goal.w()
        if(Area > big_Area):
            big_Area = Area
            big_x = goal.cx() - cen_x
            big_y = cen_y - goal.cy()
    return big_x, big_y

def callback(line):
    print(x_1, y_1 ,x_2, y_2)
    uart.write('A')
    uart.writechar(x_1)
    uart.writechar(y_1)
    uart.writechar(x_2)
    uart.writechar(y_2)

#uart.writechar( n & 0b0000000011111111)
#uart.writechar((n & 0b1111111100000000)>>8)

#割り込み
exint = pyb.ExtInt("P4", ExtInt.IRQ_RISING, pyb.Pin.PULL_DOWN, callback)
pyb.enable_irq(True)


while(True):
    clock.tick()
    img = sensor.snapshot()
    x_1, y_1 = sear(thr_1)
    x_2, y_2 = sear(thr_2)
    #print(x_1, y_1 ,x_2, y_2)
