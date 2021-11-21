#include <ESP8266WiFi.h>

// Update these with values suitable for your network.

const char* ssid = "test";
const char* password = "12345678";
int32_t channel = 0;

const char* host = "192.168.4.1";
const uint16_t port = 17;

int status = WL_IDLE_STATUS;
WiFiClient client;


long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); // 앞서 설정한 ssid와 페스워드로 Wi-Fi에 연결합니다.
  while (WiFi.status() != WL_CONNECTED) { // 연결될 때 까지 0.5초 마다 Wi-Fi 연결상태를 확인합니다.
    delay(500);
  }
}


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  setup_wifi();
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());;
}


void loop() {
    byte b_val[5] = {1, 2, 3, 4, 5};
//    ++value;
//    snprintf (msg, 75, "hello myung #%ld", value);
//    Serial.print("send message: ");
//    Serial.println(msg);
    client.write(b_val, 5);
    delay(400);

}



