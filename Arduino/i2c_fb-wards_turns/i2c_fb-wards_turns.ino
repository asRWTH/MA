#include <Wire.h>
#include <Servo.h>

Servo servo1;
Servo motorESC;
Servo pumpCtrl;

const int steeringPin = 9;
const int escPin = 5;
const int pumpPin = 6;

int address = 0x40;
int i2c_data = 0;

int initialPosition = 90;
float motorSpeed = 0;
float pumpLevel = 0;
int pwmMin = 500;
int pwmMax = 2500;

void setup() {

  servo1.attach(steeringPin);
  motorESC.attach(escPin);
  pumpCtrl.attach(pumpPin);

  servo1.write(initialPosition);
  motorESC.writeMicroseconds(map(motorSpeed, -5, 5, pwmMin, pwmMax));
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));  

  Serial.begin(9600);
  
  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);

  delay(1000);
  
}

//den Wert von i2c auf dem seriellen Monitor ausgeben
void receiveEvent(int bytes) {
  
  i2c_data = Wire.read();
  Serial.print("Received: ");
  Serial.println(i2c_data);

}

void sendData() {
  
    Wire.write(i2c_data);
    
}

void loop() {

  switch (i2c_data) {
    //Motor&Pumpe off
    case 0: {
      executeCommand0();
      break;
    }
    //links Lenken
    case 1: {
      executeCommand1();
      i2c_data = 255;
      break;
    }
    //Pumpen-Stop
    case 3: {
      executeCommand3();
      break;
    }
    //rechts Lenken
    case 5: {
      executeCommand5();
      i2c_data = 255;
      break;
    }
    //rückwerts
    case 7: {
      executeCommand7();
      break;
    }
    //Motor-Stop
    case 8: {
      executeCommand8();
      break;
    }
    //forwärts
    case 9: {
      executeCommand9
      ();
      break;
    }
  }
    
}

void executeCommand0() {
  
  motorSpeed = 0;
  motorESC.writeMicroseconds(map(motorSpeed, -5, 5, pwmMin, pwmMax));
  pumpLevel = 0;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  servo1.write(90);
  
}

void executeCommand1() {
  
  servo1.write(40);
  pumpLevel = 1.25;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  delay(1000);
  pumpLevel = 0;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  servo1.write(90);
  delay(1000);
  
}

void executeCommand3() {
  
  pumpLevel = 0;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  
}

void executeCommand5() {
  
  servo1.write(130);
  pumpLevel = 1.25;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  delay(1000);
  pumpLevel = 0;
  pumpCtrl.writeMicroseconds(map(pumpLevel, -5, 5, pwmMin, pwmMax));
  servo1.write(90);
  
}

void executeCommand7() {
  
  motorSpeed = -2;
  motorESC.writeMicroseconds(map(motorSpeed, -5, 5, pwmMin, pwmMax));
  
}

void executeCommand8() {
  
  motorSpeed = 0;
  motorESC.writeMicroseconds(map(motorSpeed, -5, 5, pwmMin, pwmMax));
  
}

void executeCommand9() {
  
  motorSpeed = 3;
  motorESC.writeMicroseconds(map(motorSpeed, -5, 5, pwmMin, pwmMax));
  
}
