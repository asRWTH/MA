#include <Servo.h>
#include <AS5047P.h>

#define steeringPin 9
#define pumpPin 6
#define sensorPin 3
#define spiBusSpeed 1000000

Servo servo1;
Servo pumpCtrl;
AS5047P as5047p(sensorPin, spiBusSpeed);

int initialPosition = 90;
int pwmMin = 0;
int pwmMax = 2500;
float steeringAngle = 0;
int servoAngle = 90;

void setup() {

  servo1.attach(steeringPin);
  pumpCtrl.attach(pumpPin);

  servo1.write(initialPosition);
  pumpCtrl.writeMicroseconds(pwmMin);  

  Serial.begin(9600);

  while (!as5047p.initSPI()) {
    Serial.println(F("Can't connect to the AS5047P sensor! Please check the connection..."));
    delay(5000);
  }

  delay(1000);

}


//Function continuously reads and displays steeringAngle from the sensor in the serial monitor
//If a serial input is available and followed by a newline charakter, 
//it sets the the servoAngle to this value 
void loop() {
  
  steeringAngle = as5047p.readAngleDegree() - 180;
  Serial.print("Steering Angle: ");
  Serial.println(steeringAngle);
  Serial.print("Servo Angle: ");
  Serial.println(servoAngle);

  pumpCtrl.writeMicroseconds(1800);

  if (Serial.available() > 0) {
    int serialInput = Serial.parseInt();

    if (Serial.read() == '\n') {
      servoAngle = serialInput;
      servo1.write(servoAngle);
      Serial.print("Servo set to: ");
      Serial.println(servoAngle);
    }
  }
}
