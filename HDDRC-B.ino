///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///für Arduino pro mini
/*
PIN MAPPING

  PIN 2  SDA
  PIN 3  SCL
  PIN 4  --  LEG B   Dome Mot PWM  Move to 9
  PIN 5   TXsmc  DOME A
  PIN 6   RXsmc  DOME B 
  PIN 7  --- LEG A  frei  vorher LegMOt
  PIN 8  SENSOR_CENTER
  PIN 9  LEG_POTI                 Move to 4
  PIN 10 PWM_OUT HOLO
  PIN 14 Rx  Main
  PIN 15 Tx  Main
  PIN 16 STATUS_PIN
  PIN 18 ledPin1
  PIN 19 ledPin2
  PIN 20 ledPinC
  PIN 21 SENSOR_RC_IN

 */
#include <SoftwareSerial.h>        // Durch diesen Include können wir die Funktionen 
#include <Servo.h>

#include <Wire.h>  
#include "vars.h"
#include "Grove_Human_Presence_Sensor.h" // der SoftwareSerial Bibliothek nutzen.
SoftwareSerial MainInput(14, 15); // Pin D14 ist RX, Pin D15 ist TX.
  
Servo HoloV; ///PWM_OUT PIN 10
Servo ArmSrv; //PWM OUT PIN 16

//Servo LEG_A; // PWM
//Servo LEG_B; // PWM


AK9753 movementSensor;

int ir1, ir2, ir3, ir4;
float temp = 24;

#include "Mdriver.h"
#include "autodome.h"
#include "command.h"


void setup(){

  HoloV.attach(PWM_OUT);  // PIN 10
  ArmSrv.attach(STATUS_PIN);
  //LEG_A.attach(LMOT_A);
  //LEG_B.attach(LMOT_B);
  
  ArmSrv.write(ARM_IN);
   
  Serial.begin(115200);
  MainInput.begin(9600);
   // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPinC, OUTPUT);
  pinMode(DMOT_A, OUTPUT);
  pinMode(DMOT_B, OUTPUT); 
  pinMode(LMOT_A, OUTPUT);
  pinMode(LMOT_B, OUTPUT); 

  pinMode(SENSOR_CENTER,  INPUT_PULLUP);
 
  //digitalWrite(DMOT_L, 0);  
  //digitalWrite(DMOT_R, 0); 

   
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generateB
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  //delay(2000);
  Serial.println("DomeController_ Doc Tooldroide");
  Serial.println("...ready for Command_");
    Wire.begin();

   //Turn on sensor
    if (movementSensor.initialize() == false) {
        Serial.println("Device not found. Check wiring.");
        //while (1);
        delay(3000);
    }

  
  HoloV.write(90);
 
}


void loop() {

    Comand();  

  if (Mode == 0){
     autoDome();
     //durchlauf = durchlauf+1;
     
  }

  if (Mode == 1  ){
      rcMove();
    
  }

  if (Mode == 2  ){
     human(); 
    
  }

  if (Mode == 3  ){
    BodyRot(tPos);
    autoDome();
    rcMove();
      if (debug){ 
        Serial.println(analogRead(LEG_POTI));
    }
  }
  
  //BodyRot(tPos);
  if (Mode == 4  ){
    //BodyRot(tPos);
    for (int i=0; i<=255; i++) {
        analogWrite(DMOT_A, i);
        //analogWrite(LMOT_B, i);
        delay (100);
        Serial.println(i);
    }
    
  }



  
  if (durchlauf == 10 ) {
      center("L");
    } 
    
  
     
  
     
    
}
