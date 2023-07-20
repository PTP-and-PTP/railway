#!/usr/bin/env python3

from com import com_receive
import datetime

filename = "data.csv"
f = open(filename, "w")
head = "detected,time\n"
f.write(head)
f.close()

while True:
    f = open(filename, "a")
    msg = com_receive('/dev/ttyS1')
    msg += ","
    msg += datetime.datetime.now().strftime("%x-%X") + '\n'
    if msg.split(',')[0] == '1':
        print("Obstacle detected at " + msg.split(',')[1])
    else:
        print("No obstacle detected at " + msg.split(',')[1])
    f.write(msg)

f.close()
