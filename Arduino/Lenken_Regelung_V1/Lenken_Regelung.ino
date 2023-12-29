#include <Servo.h>
#include <Wire.h>
#include <AS5047P.h>

#define sensorPin 3
#define spiBusSpeed 1000000
#define pumpPin 6
#define steeringPin 9
#define i2cAddress 0x40

Servo servo1;
Servo pumpCtrl;

AS5047P as5047p(sensorPin, spiBusSpeed);

int i2c_data[2];      //array stores two bytes (phi&theta values), to send them onto i2c
int phi;              //steering angle
int theta = 90;       //servo (&valve) position
int pwmMin = 0;       //pwm =[0,2500]

//measure phi and send onto I2C
void sendToi2c() {
Wire.write(phi);
Wire.write(theta);
}

void setup() {

  Serial.begin(9600);
  while (!as5047p.initSPI()) {
    Serial.println(F("no sensor found, check the connection"));
    delay(5000);
  }
  
//initialize i2c Comm
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveServoAngle);
  Wire.onRequest(sendToi2c);
  
//servo & pump setup
  servo1.attach(steeringPin);
  pumpCtrl.attach(pumpPin);

  servo1.write(theta);
  pumpCtrl.writeMicroseconds(pwmMin);

  delay(1000);

}

void loop() {
  
  phi = as5047p.readAngleDegree() - 180;
 
  pumpCtrl.writeMicroseconds(1800);

  servo1.write(theta);

  i2c_data[0] = phi;
  i2c_data[1] = theta;
  
  Serial.print("phi: ");
  Serial.print(i2c_data[0]);
  Serial.print("      ");
  Serial.print("theta: ");
  Serial.println(i2c_data[1]);

}
