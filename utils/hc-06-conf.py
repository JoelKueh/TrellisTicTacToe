import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0)
postfix = b'\r\n'

def send(data):
    ser.write(data) # no need for postfix on hc-06
    time.sleep(2.0)
    print(ser.read(100))

# Initial Stuff
print('TEST:\t\t', end='')
send(b'AT')
print('VERSION:\t', end='')
send(b'AT+VERSION')
print('NAME:\t\t', end='')
send(b'AT+NAME=J06')
print('BAUD:\t\t', end='')
send(b'AT+BAUD6') # 38400 baud
print('PIN:\t\t', end='')
send(b'AT+PIN1234')

ser.close()
