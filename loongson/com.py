import serial

def com_receive(
    device = '/dev/ttyS1',
    ):
    ser = serial.Serial(device)
    msg = ser.read_until()
    msg = msg.decode('ascii')
    msg = msg[0:-1]
    ser.close()

    return msg
