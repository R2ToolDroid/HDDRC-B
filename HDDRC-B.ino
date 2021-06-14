///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///für Arduino pro mini

#include <SoftwareSerial.h>        // Durch diesen Include können wir die Funktionen 
#include <Servo.h>

#include <Wire.h>  
#include "Grove_Human_Presence_Sensor.h" // der SoftwareSerial Bibliothek nutzen.
SoftwareSerial MainInput(14, 15); // Pin D14 ist RX, Pin D15 ist TX.
                                   // Die Funktion softSerial() kann nun wie Serial() genutzt werden.     
Servo HoloV; 

#include "vars.h"

AK9753 movementSensor;

int ir1, ir2, ir3, ir4;
float temp = 24;

#include "Mdriver.h"
#include "command.h"


void setup(){

  HoloV.attach(PWM_OUT);

   
  Serial.begin(9600);
  MainInput.begin(9600);
   // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPinC, OUTPUT);

  pinMode(DMOT_L, OUTPUT);
  pinMode(DMOT_R, OUTPUT); 

  //pinMode(STATUS_PIN, INPUT_PULLUP); 
  pinMode(SENSOR_CENTER,  INPUT_PULLUP);
 
  digitalWrite(DMOT_L, 0);  
  digitalWrite(DMOT_R, 0); 

   
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
   
   
}


void loop() {

    Comand();  
    SendPing();
   
    
  if (Mode == 0){
     randomMove();
     durchlauf = durchlauf+1;
  }

  if (Mode == 1  ){
      rcMove();
      BodyRot(tPos);
  }

  if (Mode == 2  ){
     human(); 
  }

  if (Mode == 3  ){
      BodyRot(tPos);
      if (debug){
     Serial.print(analogRead(LEG_POTI));
    }
  }

  if (durchlauf == 10 ) {
    center("L");
    } 
    
  //Serial.println(durchlauf);
  
   stat = digitalRead(STATUS_PIN);
    //Serial.print(stat);
    if (stat == 0) {
      Mode++;
      digitalWrite(ledPinC, HIGH);  
      delay(1000);
      digitalWrite(ledPinC, LOW);  
      }
     if (Mode >= 4){Mode=0;}
     
  
     
    
}
