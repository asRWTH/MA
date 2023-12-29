#include <Servo.h>
#include <AS5047P.h>

#define sensorPin 3
#define spiBusSpeed 1000000

Servo servo1;

const int steeringPin = 9;

AS5047P as5047p(sensorPin, spiBusSpeed);

void setup() {

  Serial.begin(9600);
  while (!as5047p.initSPI()) {
    Serial.println(F("no sensor found, check the connection"));
    delay(5000);
  }

}

void loop() {

  Serial.print("Angle: ");
  Serial.println(as5047p.readAngleDegree());
  delay(100);

}
