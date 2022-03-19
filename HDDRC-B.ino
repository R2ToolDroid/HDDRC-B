///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///für Arduino pro mini
/*
PIN MAPPING

  PIN 2  SDA
  PIN 3  SCL
  PIN 4  Dome Mot PWM
  PIN 5   TXsmc
  PIN 6   RXsmc
  PIN 7  frei  vorher LegMOt
  PIN 8  SENSOR_CENTER
  PIN 9  LEG_POTI
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
SoftwareSerial smcSerial = SoftwareSerial(rxPin, txPin);

                                   // Die Funktion softSerial() kann nun wie Serial() genutzt werden.     
Servo HoloV; ///PWM_OUT PIN 10
Servo ArmSrv; //PWM OUT PIN 16
Servo DomeMot; // PWM OUT PIN 4



AK9753 movementSensor;

int ir1, ir2, ir3, ir4;
float temp = 24;

#include "Mdriver.h"
#include "autodome.h"
#include "command.h"


void setup(){

   // Initialize software serial object with baud rate of 19.2 kbps.
  smcSerial.begin(19200);
 
  // The Simple Motor Controller must be running for at least 1 ms
  // before we try to send serial data, so we delay here for 5 ms.
  delay(5);
 
  // If the Simple Motor Controller has automatic baud detection
  // enabled, we first need to send it the byte 0xAA (170 in decimal)
  // so that it can learn the baud rate.
  smcSerial.write(0xAA);
 
  // Next we need to send the Exit Safe Start command, which
  // clears the safe-start violation and lets the motor run.
  exitSafeStart();

  HoloV.attach(PWM_OUT);  // PIN 10
  DomeMot.attach(DMOT_L); // PIN 4
  
  ArmSrv.attach(STATUS_PIN);
  ArmSrv.write(ARM_IN);
   
  Serial.begin(115200);
  MainInput.begin(9600);
   // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPinC, OUTPUT);

  //pinMode(DMOT_L, OUTPUT);
  //pinMode(DMOT_R, OUTPUT); 

  //pinMode(STATUS_PIN, INPUT_PULLUP); 
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
  
  //startseq();

//   SetupEvent::ready();
   Wire.begin();

   //Turn on sensor
    if (movementSensor.initialize() == false) {
        Serial.println("Device not found. Check wiring.");
        //while (1);
        delay(3000);
    }

  
  HoloV.write(90);
  DomeMot.write(90);
  //ArmSrv.write(ARM_IN);
  //ServoTouch(false);
  //center("L");
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
  
  if (durchlauf == 10 ) {
      center("L");
    } 
    
  
     
  
     
    
}
