#include <ArduinoBLE.h>

BLEService Service("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic FlexCharacteristic1("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic2("19B10002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic3("19B10003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic4("19B10004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEIntCharacteristic FlexCharacteristic5("19B10005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);


void setup() {
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // 디바이스 이름 설정
  BLE.setLocalName("Arduino Nano 33 BLE (Peripheral)");

  // service를 광고할 service 목록에 추가한다.
  BLE.setAdvertisedService(Service);

  // 사전에 정의한 characteristic을 service에 추가한다.
  Service.addCharacteristic(FlexCharacteristic1);
  Service.addCharacteristic(FlexCharacteristic2);
  Service.addCharacteristic(FlexCharacteristic3);
  Service.addCharacteristic(FlexCharacteristic4);
  Service.addCharacteristic(FlexCharacteristic5);
  
  // 이제 server를 BLE device에 등록한다.
  BLE.addService(Service);
  
  // start advertising
  BLE.advertise();

  Serial.println("Nano 33 BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  // 연결 대기
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);
  
  // 성공적으로 연결되었다면
  if (central) {
    Serial.print("Connected to central: ");
    // 연결된 central 디바이스의 MAC 주소를 출력
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
        if (FlexCharacteristic1.value() && FlexCharacteristic2.value() && FlexCharacteristic3.value() &&
        FlexCharacteristic4.value() && FlexCharacteristic5.value()) {   // any value other than 0
          int data1 = FlexCharacteristic1.value();
          int data2 = FlexCharacteristic2.value();
          int data3 = FlexCharacteristic3.value();
          int data4 = FlexCharacteristic4.value();
          int data5 = FlexCharacteristic5.value();
          Serial.println(data1);
          delay(1000);
          Serial.println(data2);
          delay(1000);
          Serial.println(data3);
          delay(1000);
          Serial.println(data4);
          delay(1000);
          Serial.println(data5);
          delay(1000);
        }
     }

    // 연결 실패 시 출력
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
