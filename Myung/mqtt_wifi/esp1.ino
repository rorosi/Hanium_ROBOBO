#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Set WiFi credentials
#define WIFI_SSID "Myung_2.4G"
#define WIFI_PASS "dlaudgus1!"
 
// Set AP credentials
#define AP_SSID "myESP01"
#define AP_PASS "12345678"

WiFiServerSecure server(80);

const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;


void setup()
{
  Serial.begin(9600);
  Serial.println();
  delay(10);
  
  wifi_setup();
  client.setServer(mqtt_server, 1883); // MQTT 서버에 연결합니다. 
  client.setCallback(callback);

}



 
void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  snprintf (msg, 50, "7 8 9"); // 메시지는 배열로 전송되기 때문에 배열을 생성합니다.
  client.publish("myung_esp_topic", msg); // inTopic 토픽으로 메시지를 전송합니다.
  delay(300); // 메시지가 전송될 때 까지 기다립니다.
}



void callback(char* topic, byte* payload, unsigned int length) {
  char aaa;
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    aaa +=(char)payload[i];
  }
  Serial.print(aaa);
  Serial.println();

}


void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP8266Client-"; // 클라이언트 ID를 설정합니다.
    clientId += String(random(0xffff), HEX); // 같은 이름을 가진 클라이언트가 발생하는것을 방지하기 위해, 렌덤 문자를 클라이언트 ID에 붙입니다.
    if (client.connect(clientId.c_str())) { // 앞서 설정한 클라이언트 ID로 연결합니다.
      client.subscribe("myung_esp_topic");
    } else {
      delay(5000);
    }
  }
}



void wifi_setup() {
  // Setup serial port
  // Begin Access Point
  
  // WiFi.mode(WIFI_AP_STA);
  // WiFi.softAP(AP_SSID, AP_PASS);
  WiFi.mode(WIFI_STA);
  // Begin WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
 
  // Connecting to WiFi...
  Serial.print("와이파이 연결중 : ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
 
  // Connected to WiFi
  Serial.println();
  Serial.println("와이파이 연결 성공");
  Serial.print("IP address for network ");
  Serial.print(WIFI_SSID);
  Serial.print(" : ");
  Serial.println(WiFi.localIP());
//  Serial.print("IP address for network ");
//  Serial.print(AP_SSID);
//  Serial.print(" : ");
//  Serial.print(WiFi.softAPIP());
  Serial.println();

  Serial.println("STA 모드 사용 시작");
  delay(500);
}

