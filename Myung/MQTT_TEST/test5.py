import serial
import json 
ser = serial.Serial('/dev/ttyACM0', 9600)
while 1:
	talk = ser.readline()
	talk = int(talk)
	talk0 = str(talk)
	talk0 = talk0[1:]
	#talk2 = talk0[2:]
	print(talk0)
