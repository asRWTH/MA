#include <Wire.h>
#include <Servo.h>

int i2cAddress = 0x40;

Servo servo1;

void setup() {
  Wire.begin(i2cAddress);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);
  
  Serial.begin(9600);
  servo1.attach(7);
  servo1.write(90);
}

int servoState = 0;

void receiveEvent(int bytes) {
  servoState = Wire.read();    // ein Zeichen vom I2C lesen
  Serial.print("Received: ");
  Serial.println(servoState);

}

void loop()
{
  if(servoState > 0) {  
    if(servoState == 2){
        servo1.write(90);
        servoState = 0;
    }
    else if(servoState == 3){    
        servo1.write(180);
        servoState = 0;                    
    }
  }
}

void sendData(){
    Wire.write(servoState);
}
