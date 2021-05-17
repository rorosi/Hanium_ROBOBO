import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient

def helloworld(self, params, packet):
	print("received message from aws")
	print("topic: " + packet.topic)
	print("payload: ", (packet.payload))

myMQTTClient = AWSIoTMQTTClient("myung")
myMQTTClient.configureEndpoint("???", 8883)

myMQTTClient.configureCredentials("/home/pi/ROBOBO/Myung/aws_iot/root-ca.pem", "/home/pi/ROBOBO/Myung/aws_iot/private.pem.key", "/home/pi/ROBOBO/Myung/aws_iot/certificate.pem")

myMQTTClient.configureOfflinePublishQueueing(-1)
myMQTTClient.configureDrainingFrequency(2)
myMQTTClient.configureConnectDisconnectTimeout(10)
myMQTTClient.configureMQTTOperationTimeout(5)

print("starting....")
myMQTTClient.connect()

#myMQTTClient.subscribe("home/hellomyung", 1, helloworld)
#while True:
#	time.sleep(3)



print("message from aws")
myMQTTClient.publish(
	topic = "home/helloworld"
	QoS = 1
	payload = "myung hello"
)
