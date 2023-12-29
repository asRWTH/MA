#include <Servo.h>
#include<Wire.h>
#include <AS5047P.h>

#define sensorPin 3
#define spiBusSpeed 1000000
#define steeringPin 9
#define address 0x40

Servo servo1;
AS5047P as5047p(sensorPin, spiBusSpeed);

float theta = 90;


void setup() {
  //initialize i2c port
  Wire.begin(address);
  Wire.onReceive(receiveTheta);

  //initilize serial
  Serial.begin(9600);
  while (!as5047p.initSPI()) {
    Serial.println(F("Can't connect to the AS5047P sensor! Please check the connection..."));
    delay(5000); 
  }
  
  //setup servo
  servo1.attach(steeringPin);
  servo1.write(theta);

  delay(1000);
}

void loop() { 
  servo1.write(theta);
  Serial.println(theta);
}

void receiveTheta(int byteCount) {
  if (Wire.available() >= sizeof(theta)) {
    byte* bytes = (byte*)(&theta);
    for (int i = 0; i < sizeof(theta); i++) {
      bytes[i] = Wire.read();
    }
  }
}
