# -*- coding: utf-8 -*-
"""
Embeddedx.net
Srec application downloader
"""

#!/usr/bin/env python
import time
import serial
import bincopy


ser = serial.Serial(
    port='COM4',
    baudrate = 19200  ,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1
)


with open('path to file') as f:
    content = f.readlines()
content = [x.strip()+'\r' for x in content]
ser.timeout = 10
reponse = ser.read().decode('utf-8')
print("reponse = "+ reponse)
if(reponse == 'S'):
    ser.timeout = .1

    for se in content:
        ser.write(se.encode())
        reponse = ser.read().decode('utf-8')
        print("reponse = "+ reponse)
        if(reponse != 'O'):
            
            if(reponse == 'C'):
                print('Recoded Checksum error');
            elif(reponse == 'H'):
                print('Recoded Header Error');
            elif(reponse == 'T'):
                print('Recoded Type Error');
            elif(reponse == 'Z'):
                print('Recoded Size Error');
            else:    
                print("Undefined Error")
            break
    if(reponse == 'O'):    
        print("Update software successfully")
else:
    print("Eroooor")    
ser.close()    