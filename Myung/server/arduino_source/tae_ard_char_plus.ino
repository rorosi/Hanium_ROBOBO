#include <Servo.h>
#include <ArduinoBLE.h>

BLEService Service("19c10000-e8f2-537e-4f6c-d104768a1214");
//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic char1("19c10001-e8f2-537e-4f6c-d104768a1111", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char2("19c10001-e8f2-537e-4f6c-d104768a2222", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char3("19c10001-e8f2-537e-4f6c-d104768a3333", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char4("19c10001-e8f2-537e-4f6c-d104768a4444", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic char5("19c10001-e8f2-537e-4f6c-d104768a5555", BLERead|BLEWrite|BLENotify, 10);

Servo servo;

int servopin = 5;


byte read_data = 0;
String data = "mss daa ccc ddd";
String data2 = "";
int ch = 0;

int fin1, fin2, fin3, fin4, fin5;
int old_sum = 0;


void setup() {
  Serial.begin(115200);
  servo.attach(5);
  servo.write(150);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("tae nano 33 ble");
  BLE.setAdvertisedService(Service);
  Service.addCharacteristic(char1);
  Service.addCharacteristic(char2);
  Service.addCharacteristic(char3);
  Service.addCharacteristic(char4);
  Service.addCharacteristic(char5);
  BLE.addService(Service);

  BLE.advertise();
  Serial.println("nano33 ble peripheral");
  Serial.println(BLE.address());
  
}

void loop(){
  BLEDevice central = BLE.central();
  Serial.println("finding central device");
  delay(100);

  if(central){
    Serial.println("conn to central : ");
    Serial.println(central.address()); //MAC

    while(central.connected()){
        fin1 = char1.value().toInt();
        fin2 = char2.value().toInt();
        fin3 = char3.value().toInt();
        fin4 = char4.value().toInt();
        fin5 = char5.value().toInt();

        int sum_data = fin1 + fin2 + fin3 + fin4 + fin5;

        Serial.print(fin1);
        Serial.print(" ");
        Serial.print(fin2);
        Serial.print(" ");
        Serial.print(fin3);
        Serial.print(" ");
        Serial.print(fin4);
        Serial.print(" ");
        Serial.println(fin5);
    }
  }
  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
