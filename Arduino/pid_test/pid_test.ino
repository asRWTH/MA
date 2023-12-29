#include <Servo.h>
#include <PID_v1.h>
#include <AS5047P.h>

//define consts
#define sensorPin 3
#define spiBusSpeed 1000000
#define pumpPin 6
#define steeringPin 9

//create servo objects
Servo servo1;
Servo pumpCtrl;

//create sensor object
AS5047P as5047p(sensorPin, spiBusSpeed);

double theta;               // servo position = valve position
double Setpoint;            // phi_soll
double Input;               // phi_ist
double Output;              // theta unmapped
double Output_max = 35, Output_min = -35; //saturation limit
double Error;
double Kp=2, Ki=0, Kd=0;    // PID parameters
 
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M, REVERSE);   
//Proportional on measurement P_ON_M will make the output move more smoothly when the setpoint is changed
                                                                      
void setup() {

  //Initialize Serial Comm
  Serial.begin(9600);
  while (!as5047p.initSPI()) {
    Serial.println(F("no sensor found, check the connection"));
    delay(5000);
  }
  
  pumpCtrl.writeMicroseconds(0);
  
  //Turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(45,135);    //limiting the output to avoid damaging the valve

  delay(1000);

  servo1.attach(steeringPin);
  pumpCtrl.attach(pumpPin);

  servo1.write(90);
  pumpCtrl.writeMicroseconds(0);

}

void loop() {

  pumpCtrl.writeMicroseconds(1800);
  
  Setpoint = 0;

  Serial.print("phi_soll:  ");
  Serial.print(Setpoint);
  Serial.print("      ");
  
  Input = as5047p.readAngleDegree() - 142;
  Serial.print("phi_ist: ");
  Serial.print(Input);
  Serial.print("      ");

  Error = Setpoint - Input;
  Serial.print("error:  ");
  Serial.print(Error);
  Serial.print("      ");
  
  myPID.Compute();

  Serial.print("output: ");
  Serial.print(Output);
  Serial.println("      ");

  Serial.print("theta:  ");
  Serial.print(theta);
  Serial.println("      ");

  servo1.write(Output);
  
}
