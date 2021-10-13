#include <ArduinoBLE.h>

BLEService Service("19b10000-e8f2-537e-4f6c-d104768a1214");

//BLEUnsignedCharCharacteristic FlexChar("19b10001-e8f2-537e-4f6c-d104768a1214", BLERead|BLEWrite|BLENotify);
BLEStringCharacteristic fin_1("19b10001-e8f2-537e-4f6c-d104768a1111", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic fin_2("19b10001-e8f2-537e-4f6c-d104768a2222", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic fin_3("19b10001-e8f2-537e-4f6c-d104768a3333", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic fin_4("19b10001-e8f2-537e-4f6c-d104768a4444", BLERead|BLEWrite|BLENotify, 10);
BLEStringCharacteristic fin_5("19b10001-e8f2-537e-4f6c-d104768a5555", BLERead|BLEWrite|BLENotify, 10);


int flex_1 = A0;
int flex_2 = A1;
int flex_3 = A2;
int flex_4 = A3;
int flex_5 = A4;

int flex_1_val;
int flex_2_val;
int flex_3_val;
int flex_4_val;
int flex_5_val;

unsigned long prev1 = 0;
unsigned long prev2 = 0;
unsigned long prev3 = 0;
unsigned long prev4 = 0;
unsigned long prev5 = 0;

long delay_time = 200;

void setup() {
  Serial.begin(115200);
  while(!Serial);

  if(!BLE.begin()){
    Serial.println("start ble fail");
    while(1);
  }

  BLE.setLocalName("myung nano 33 ble");
  BLE.setAdvertisedService(Service);
  Service.addCharacteristic(fin_1);
  Service.addCharacteristic(fin_2);
  Service.addCharacteristic(fin_3);
  Service.addCharacteristic(fin_4);
  Service.addCharacteristic(fin_5);
  BLE.addService(Service);

  BLE.advertise();
  Serial.println("nano33 ble peripheral");
  Serial.println(BLE.address());
  delay(10);
}

void loop(){
  BLEDevice central = BLE.central();
  Serial.println("finding central device");
  delay(100);

  if(central){
    Serial.println("conn to central : ");
    Serial.println(central.address()); //MAC

    while(central.connected()){
      unsigned long current = millis();

      if(current - prev1 >= delay_time){
        flex_1_val = int(analogRead(flex_1));
        //flex_1_val = map(flex_1_val, 0, 800, 10, 99);
        prev1 = current;

        fin_1.writeValue(String(flex_1_val));
      }

      if(current - prev2 >= delay_time+1){
        flex_2_val = int(analogRead(flex_2));
        //flex_2_val = map(flex_2_val, 0, 800, 10, 99);
        prev2 = current;

        fin_2.writeValue(String(flex_2_val));
      }

      if(current - prev3 >= delay_time+2){
        flex_3_val = int(analogRead(flex_3));
        //flex_3_val = map(flex_3_val, 0, 800, 10, 99);
        prev3 = current;

        fin_3.writeValue(String(flex_3_val));
      }
      
      if(current - prev4 >= delay_time+3){
        flex_4_val = int(analogRead(flex_4));
        //flex_4_val = map(flex_4_val, 0, 800, 10, 99);
        prev4 = current;

        fin_4.writeValue(String(flex_4_val));
      }
      
      if(current - prev5 >= delay_time+4){
        flex_5_val = int(analogRead(flex_5));
        //flex_5_val = map(flex_5_val, 0, 800, 10, 99);
        prev5 = current;

        fin_5.writeValue(String(flex_5_val));
      }

      delay(10);
     }
  }

  else{
    Serial.print(F("disconn from central"));
    Serial.println(central.address());
  }
}