#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>


char ap_ssid[] = "test";        // AP모드의 SSID
char ap_pass[] = "12345678";        // AP모드의 PASS
char st_ssid[] = "myung";        // ST모드의 SSID
char st_pass[] = "31963196";        // ST모드의 PASS
const char* mqtt_server = "broker.mqtt-dashboard.com"; // MQTT Broker 주소, 오픈 borker 주소.


const char* pub_topic = "myung_esp_topic2";
const char* sub_topic = "myung_esp_topic1";
int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient n_client;
PubSubClient client(n_client);

int count = 0;

#define qos_level_2 2
#define qos_level_1 1


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {;}
                  
  if (WiFi.status() == WL_NO_MODULE) {     
    Serial.println("Communication with WiFi module failed!");  
    while (true);
  } //와이파이 모듈 체크

  String fv = WiFi.firmwareVersion();     
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  } //버전 체크
  
  
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(st_ssid);

    status = WiFi.begin(st_ssid, st_pass);
    
    delay(1000);
  } // 와이파이 연결시도(연결이 되어 있지 않을때만 실행)

  Serial.println("공유기와 와이파이 연결은 잘 됩니다.");
  printWifiStatus();


  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다.
  client.setCallback(callback);
  server.begin();
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  send_mqtt();
  
}


void send_mqtt(){
    String aa = "a b c " + (String)count;
    char send_buff[32] = {0};
    aa.toCharArray(send_buff, aa.length());
    client.beginPublish(pub_topic, 50, false);
    client.publish(pub_topic, send_buff);
    delay(10);
    count++;

}




void callback(char* topic, byte* payload, unsigned int length) {
  // Topic에 메시지가 도착하면 실행되는 콜백입니다.
  String mqtt_come_talk = "";
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    mqtt_come_talk +=(char)payload[i];
  }
  char msgbuff[50];
  mqtt_come_talk.toCharArray(msgbuff, mqtt_come_talk.length());
  Serial.print(msgbuff);
  Serial.println();
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void reconnect() {
  while (!client.connected()) {
    String clientId = "ArduinoNANO33IoTClinet-"; // 클라이언트 ID를 설정합니다.
    clientId += String(random(0xffff), HEX); // 같은 이름을 가진 클라이언트가 발생하는것을 방지하기 위해, 렌덤 문자를 클라이언트 ID에 붙입니다.
    if (client.connect(clientId.c_str())) { // 앞서 설정한 클라이언트 ID로 연결합니다.
      client.subscribe(sub_topic, qos_level_1); // inTopic 토픽을 듣습니다.
    } else {
      delay(5000);
    }
  }
}