int flexpin_num1 = A6; //엄지
int flexpin_num2 = A5; //검지
int flexpin_num3 = A4; //약지

void setup() {
  Serial.begin(9600);
}

void loop() {
  int flexVal1, flexVal2, flexVal3;

  flexVal1 = analogRead(flexpin_num1);
  flexVal2 = analogRead(flexpin_num2);
  flexVal3 = analogRead(flexpin_num3);

  Serial.print("1");
  Serial.print(flexVal1);
  Serial.print("2");
  Serial.print(flexVal2);
  Serial.print("3");
  Serial.println(flexVal3);
  delay(100);

}
