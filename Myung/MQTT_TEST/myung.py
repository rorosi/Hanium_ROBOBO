import time
import json
from multiprocessing import Process, Queue
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient


def recv_message(self, topic, packet):
	d = json.loads(packet.payload)
	print(d['message'])
	#read


#--------------------------------------------aws_iot와 연결---------------------------------------------------------------------------------------------------------------------
myMQTTClient = AWSIoTMQTTClient("myung")


myMQTTClient.configureEndpoint("awts3jg7w6w7y-ats.iot.us-east-1.amazonaws.com", 8883)

myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/MQTT_TEST/root-ca.pem", "/home/pi/ROBOBO/Myung/MQTT_TEST/private.pem.key", "/home/pi/ROBOBO/Myung/MQTT_TEST/certificate.pem.crt")

myMQTTClient.configureOfflinePublishQueueing(-1)
myMQTTClient.configureDrainingFrequency(2)
myMQTTClient.configureConnectDisconnectTimeout(10)
myMQTTClient.configureMQTTOperationTimeout(5)

print("starting ")
myMQTTClient.connect()

#---------------------------------------------------------------------------------------------------------------------------------------------------------------------

#my_talk = str(input("aws_iot에 전송할 말 입력 : "))

i=0
while True:
	myMQTTClient.subscribe("home/helloworld/write", 1, recv_message) #read
	myMQTTClient.publish(
		topic = "home/helloworld",		
		QoS = 0,
		payload = "myung hello"+str(i)
	) #write


#
