#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <MPU9250.h>

MPU9250 mpu;

char ap_ssid[] = "test";        // AP모드의 SSID
char ap_pass[] = "12345678";        // AP모드의 PASS
char st_ssid[] = "myung";        // ST모드의 SSID
char st_pass[] = "31963196";        // ST모드의 PASS
const char* mqtt_server = "broker.mqtt-dashboard.com"; // MQTT Broker 주소, 오픈 borker 주소.
 
int status = WL_IDLE_STATUS;

WiFiServer server(80);
WiFiClient n_client;
PubSubClient client(n_client);

String msg = "";
String dohang_count = "0";
byte b_val[5] = {1, 2, 3, 4, 5};

int fin[5];


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  while (!Serial) {;}
  Wire.begin();
  delay(1000);

  if(!mpu.setup(0x68)){
    while(1){
      Serial.println("자이로 센서 연결이 안됐어요");
      delay(1000);
    }
  }
                  
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
  reconnect();
  Serial.println("mqtt 접속 대기");
  dohang_count="0";
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()) {
    reconnect();
  }
  client.loop();
  if(dohang_count=="1"){
    wifiAPMode();
  }
}

void wifiAPMode()
{
  dohang_count="2";
  Serial.println("AP모드 변경");
  status = WL_IDLE_STATUS;
  // WiFi 드라이버 다시 초기화
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();
  
  status = WiFi.beginAP(ap_ssid, ap_pass);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  
  while (true) {
    if (status != WiFi.status()) {
      // it has changed update the variable
      status = WiFi.status();
  
      if (status == WL_AP_CONNECTED) {
        // a device has connected to the AP
        Serial.println("Device connected to AP");
        break;
      }
    }
  }
  server.begin();
  Serial.println("로봇팔 접속 대기");
  sendToData();
}




void sendToData(){
  while(true){
    n_client = server.available();   // 로봇 팔 Client의 접속을 기다림   
    if (n_client) {                             // 로봇 팔측 Client 접속했다면
      Serial.println("로봇팔이 접속했다.");           // 연결되었다고 시리얼 모니터에 띄움
      Serial.println("3초간 손을 펴시오");
      delay(3000);
      int max0 = analogRead(A0);
      int max1 = analogRead(A1);
      int max2 = analogRead(A2);
      int max3 = analogRead(A3);
      int max4 = analogRead(A6);
      delay(300);
      Serial.print(max0);
      Serial.print(" ");
      Serial.print(max1);
      Serial.print(" ");
      Serial.print(max2);
      Serial.print(" ");
      Serial.print(max3);
      Serial.print(" ");
      Serial.println(max4);
      
      delay(1000);

      Serial.println("3초간 주먹 쥐시오");
      delay(3000);
      int min0 = analogRead(A0);
      int min1 = analogRead(A1);
      int min2 = analogRead(A2);
      int min3 = analogRead(A3);
      int min4 = analogRead(A6);
      delay(300);
      Serial.print(min0);
      Serial.print(" ");
      Serial.print(min1);
      Serial.print(" ");
      Serial.print(min2);
      Serial.print(" ");
      Serial.print(min3);
      Serial.print(" ");
      Serial.println(min4);
      
      delay(1000);
      
      while (n_client.connected()) {            // 로봇 팔에 데이터를 전송
        fin[0] = analogRead(A0);
        fin[1] = analogRead(A1);
        fin[2] = analogRead(A2);
        fin[3] = analogRead(A3);
        fin[4] = analogRead(A6);

        fin[0] = constrain(map(fin[0], min0, max0, 0, 150), 0, 150);
        fin[1] = constrain(map(fin[1], min1, max1, 0, 150), 0, 150);
        fin[2] = constrain(map(fin[2], min2, max2, 0, 150), 0, 150);
        fin[3] = constrain(map(fin[3], min3, max3, 0, 150), 0, 150);
        fin[4] = fin[3];

        if(mpu.update()){
          static uint32_t prev_ms = millis();
          if(millis() > prev_ms + 25){
            print_gyro();
            prev_ms = millis();
          }
        }

        for(int i=0; i<sizeof(fin)/sizeof(fin[0]); i++){
          b_val[i] = fin[i];
        }
        n_client.write(b_val,5);

        delay(150);
      }
    }
  }
}




void callback(char* topic, byte* payload, unsigned int length) {
  // Topic에 메시지가 도착하면 실행되는 콜백입니다.
  msg="";
  Serial.print(topic);
  Serial.print(" ");
  for (int i = 0; i < length; i++) {
    msg +=(char)payload[i];
  }
  Serial.println(msg);
  dohang_count=(String)msg;
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
      client.subscribe("dohang_topic2"); // inTopic 토픽을 듣습니다.
    } else {
      delay(5000);
    }
  }
}


void print_gyro(){
  int gy_x = (int)mpu.getPitch();
  gy_x = constrain(map(gy_x, 0, 90, 596, 1369), 596, 1369);
  Serial.println(gy_x);
}
