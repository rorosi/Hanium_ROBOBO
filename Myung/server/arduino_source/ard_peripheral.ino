#include <ArduinoBLE.h>

BLEService Service("19B10000-E8F2-537E-4F6C-D104768A1214");

BLEUnsignedCharCharacteristic FlexChar("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead|BLEWrite|BLENotify);

byte read_data = 0;
unsigned char write_data[] = "hello";

void setup() {
  Serial.begin(9600);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("Ard nano 33 ble (peripheral)");
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
      if(FlexChar.written()){
        read_data = FlexChar.value();
        Serial.print((char)read_data);
      }
    }
  }

  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
