#include <ArduinoBLE.h>

BLEService Service("19c10000-e8f2-537e-4f6c-d104768a1214");

//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic FlexChar("19c10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite, 50);

byte read_data = 0;
String data = "mss daa ccc ddd";
String data2 = "";
int ch = 0;



void setup() {
  Serial.begin(115200);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("tae nano 33 ble");
  BLE.setAdvertisedService(Service);
  Service.addCharacteristic(FlexChar);
  BLE.addService(Service);

  BLE.advertise();
  Serial.println("nano33 ble peripheral");
  Serial.println("");

}

void loop(){
  BLEDevice central = BLE.central();
  Serial.println("finding central device");
  delay(100);

  if(central){
    Serial.println("conn to central : ");
    Serial.println(central.address()); //MAC

    while(central.connected()){
        data2 = FlexChar.value();
        Serial.println(data2);
        delay(250);
     }
  }

  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
