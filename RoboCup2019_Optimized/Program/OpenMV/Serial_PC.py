import time,pyb
usb = pyb.USB_VCP()
RED = pyb.LED(1)#blue
GREEN = pyb.LED(2)#blue
BLUE = pyb.LED(3)#blue

x = 0
y = 0
while(True):
    #x = x + 5
    y = y + 8
    print("%d," %x,end="")
    print("%d" %y)
    time.sleep(100)
    if(usb.isconnected()):
        BLUE.on()
        GREEN.on()
        RED.on()
    else:
        BLUE.off()
        GREEN.off()
        RED.off()
