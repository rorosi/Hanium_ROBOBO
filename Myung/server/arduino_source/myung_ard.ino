#include <ArduinoBLE.h>

BLEService Service("19b10000-e8f2-537e-4f6c-d104768a1214");

//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite, 20);

byte read_data = 0;
String data = "mss daa ccc ddd";
String data2 = "";
int ch = 0;
byte *temp = new byte[data.length()+1];


void setup() {
  Serial.begin(115200);
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
        data2 = data + " " + ch;
        FlexChar.writeValue(data2);
        Serial.println(data2);
        ch++;
        delay(250);
     }
  }

  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}
