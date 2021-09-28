import time
import json
import serial
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

from bluetooth import *
from bluepy import btle
from signal import signal, SIGPIPE, SIG_DFL


def conn_aws(client_name):
	
	if client_name == "write_client":
		myMQTTClient = AWSIoTMQTTClient("tae_write")
	if client_name == "read_client":
		myMQTTClient = AWSIoTMQTTClient("tae_read")

	myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)

	myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/CLIENT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/CLIENT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/CLIENT_TEST/certificate.pem.crt")

	myMQTTClient.configureOfflinePublishQueueing(-1)
	myMQTTClient.configureDrainingFrequency(2)
	myMQTTClient.configureConnectDisconnectTimeout(10)
	myMQTTClient.configureMQTTOperationTimeout(5)

	print("starting aws",client_name)
	myMQTTClient.connect()

	return myMQTTClient


def send_to_ard():
	myAWS = conn_aws("write_client")
	global MAC
	global SERVICE_UUID
	global CHARACTERISTIC_UUID
	global dev



	MAC = "CA:D5:18:96:C7:16"
	SERVICE_UUID = "19c10000-e8f2-537e-4f6c-d104768a1214"
	CHARACTERISTIC_UUID = "19c10001-e8f2-537e-4f6c-d104768a1214"
	print('아두이노 연결 시도중')
	dev = btle.Peripheral(MAC)

	service_uuid = btle.UUID(SERVICE_UUID)
	service = dev.getServiceByUUID(service_uuid)
	characteristics = dev.getCharacteristics()

	for char in characteristics:
		print('conn check ...')
		if(char.uuid == CHARACTERISTIC_UUID):
			print('==== UUID match ====')
			print(char)
			global nano
			nano = char

	while True:
		try:
			myAWS.subscribe("server/client", 1, recv_message_to_ard)


		except Exception as e:
			if e.errno == errno.EPIPE:
				signal(SIGPIPE, SIG_DFL)
			print('에러발생 : ', e)
			time.sleep(0.5)
			dev.disconnect()
			service_uuid = btle.UUID(SERVICE_UUID)
			service = dev.getServiceByUUID(service_uuid)
			characteristics = dev.getCharacteristics()
			for char in characteristics:
				print('------')
				if(char.uuid == CHARACTERISTIC_UUID):
					print('uuid rematching')
					print(char)
					nano = char
			pass

def recv_message_to_ard(self, topic, packet):

	ard_client = myARD
	come_message = json.loads(packet.payload)

	sensor_data = come_message['data'][0]['finger']+" "+come_message['data'][1]['finger']+" "+come_message['data'][2]['finger']+" "+come_message['data'][3]['finger']+" "+come_message['data'][4]['finger']+"\n"
	
	sensor_data = str(sensor_data)

	nano.write(sensor_data.encode())
	print(sensor_data.encode())


def call_subscribe():
	myAWS = conn_aws("read_client")
	while True:
		myAWS.subscribe("server/client", 1, recv_message_test) #read

def recv_message_test(self, topic, packet):
	come_message = json.loads(packet.payload)
	print("엄지 : ", come_message['finger_1'])
	print("검지 : ", come_message['finger_2'])
	print("약지 : ", come_message['finger_3'])

	#read #라즈베리파이에 메시지가 잘오는지 검사하기 위한 것

	


#--------------------------------------------aws_iot와 연결---------------------------------------------------------------------------------------------------------------------



#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

#my_talk = str(input("aws_iot에 전송할 말 입력 : "))


if __name__ == '__main__':

	print("message from aws")
	procs = []

	proc1 = Process(target=send_to_ard)
	procs.append(proc1)
	proc1.start()

	#proc2 = Process(target=call_subscribe)
	#procs.append(proc2)
	#proc2.start()

	#for proc in procs:
	#	proc.join()





