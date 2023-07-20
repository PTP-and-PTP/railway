#!/usr/bin/env python

import serial
from datetime import datetime

def com_send2(
        content,
        device = '/dev/ttyUSB0',
        ):
    ser = serial.Serial(device)
    ser.write((datetime.now().strftime("%H:%M:%S") + (': '+content+'\n')).encode('ascii'))
    ser.close()

def com_send(
        content,
        device = '/dev/ttyUSB0',
        ):
    ser = serial.Serial(device)
    ser.write((content+'\n').encode('ascii'))
    ser.close()

def com_receive(
        device = '/dev/ttyUSB0',
        ):
    ser = serial.Serial(device)
    msg = ser.read_until()
    msg = msg.decode('ascii')
    msg = msg[0:-1]
    ser.close()

    return msg
