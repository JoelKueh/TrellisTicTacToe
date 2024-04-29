import serial
import time

postfix = b'\r\n'
def send(data):
    ser.write(data) # no need for postfix on hc-06
    time.sleep(1.0)
    print(ser.read(100))

print('Testing Baud Rates:')
bauds = [ 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1382400 ]
for baud in bauds:
    ser = serial.Serial('/dev/ttyUSB0', baud, timeout=0)
    print('Baud Rate: ', baud)
    print('TEST_WO:\t', end='')
    send(b'AT')
    print('TEST_W:\t\t', end='')
    send(b'AT\r\n')
    ser.close()

ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=0)

# Initial Stuff
# print('TEST:\t\t', end='')
# send(b'AT')
# print('VERSION:\t', end='')
# send(b'AT+VERSION')
# print('NAME:\t\t', end='')
# send(b'AT+NAME=J06')
# print('BAUD:\t\t', end='')
# send(b'AT+BAUD6') # 38400 baud
# print('PIN:\t\t', end='')
# send(b'AT+PIN1234')

ser.close()
