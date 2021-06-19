#include <Arduino.h>       

bool inRange(int val, int minimum, int maximum){
  return ((minimum <= val)&&(val <= maximum));
}

void CeckSens(){

    if (SENSOR_RC_IN == 0){
       Serial.println("No PWM Signal on 21");
       delay(3000);
    }
  
}
/*
void resetM(){
   digitalWrite(ledPin1, LOW); 
   digitalWrite(ledPin2, LOW); 
   ledState1 = LOW;
   ledState2 = LOW;
   HoloV.write(90);
   //LegMot.write(90);    //PIN 7
   DomeMot.write(90);
   ArmSrv.write(ARM_IN); 

}
*/
void LMotor(int MO){   
    //tempo = 200;    
    
    switch (MO){
      case M_LEFT:
      if (debug) {   
      Serial.print(" left ");   
      } 
      digitalWrite(ledPin2, HIGH); 
      LegMot.write(Ltemp_L);
      //analogWrite(BMOT_R, 0); 
      //analogWrite(BMOT_L, tempo); 
      break;

      case M_RIGHT:
      if (debug) {   
      Serial.print(" right ");
      }
      digitalWrite(ledPin1, HIGH); 
      LegMot.write(Ltemp_R);
      //analogWrite(BMOT_L, 0); 
      //analogWrite(BMOT_R, tempo);
      break; 

      case M_STOP:
      if (debug) {   
      Serial.print(" stop ");
      }
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      LegMot.write(90);
      //analogWrite(BMOT_L, 0); 
      //analogWrite(BMOT_R, 0);
      break;

      default:
      ///do nothing
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      //analogWrite(BMOT_L, 0); 
      //analogWrite(BMOT_R, 0);
      break;
      
    }
  
}
/*
void BodyRot2(int tPos) {   // Rotiert zu tPos

    int potPos = analogRead(LEG_POTI);


  if (mov = true){
    

   if (inRange(tPos, potPos-D_ZONE, potPos+D_ZONE)){
    if (debug) {   
      Serial.print(" match "); // Alles klar so bleiben
    }
      potPos = tPos;
      MO = M_STOP; 
      mov = false;   
      
   
   }    else {
    mov = true;    
              
   }

    if ( potPos >= B_TOP ) {mov = false;}
    if ( potPos <= B_DOWN) {mov = false;}

   if ( (potPos >= tPos) && (mov == true) ){
      MO = M_LEFT;
   }

    if ((potPos <= tPos) && (mov == true)) {
      MO = M_RIGHT;
   }

   } /// end if MOV = true
   
    LMotor(MO);
    if (debug) {   
    Serial.print(" Poti Pos = ");
    //Serial.print();  
    Serial.print(potPos);
    Serial.print(" | Tpos = ");
    Serial.println(tPos);
    }

  
}
**/


void BodyRot(int tPos) {   // Rotiert zu tPos

    int potPos = analogRead(LEG_POTI);

    

    if (inRange(tPos, potPos-D_ZONE, potPos+D_ZONE)){
    if (debug) {   
      Serial.print(" match "); // Alles klar so bleiben
    }
      potPos = tPos;
      
      if (debug) {   
      Serial.print(" stop ");
      }
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      LegMot.write(90);
      
      mov = false;
      
      
   
   }    else {
    mov = true;    
    
   }

    //if ( potPos >= B_TOP ) {mov = false;}
    //if ( potPos <= B_DOWN) {mov = false;}

   if ( (potPos >= tPos) && (mov == true) ){
      if (debug) {   
      Serial.print(" left ");   
      } 
      digitalWrite(ledPin2, HIGH); 
      LegMot.write(Ltemp_L);
   }

    if ((potPos <= tPos) && (mov == true)) {
      if (debug) {   
      Serial.print(" right ");
      }
      digitalWrite(ledPin1, HIGH); 
      LegMot.write(Ltemp_R);
   }

    
   
    //LMotor(MO);
    if (debug) {   
    Serial.print(" Poti Pos = ");
    //Serial.print();  
    Serial.print(potPos);
    Serial.print("| mov = ");
    Serial.print(mov);
    Serial.print(" | Tpos = ");
    Serial.println(tPos);
    }
    
   
    
}


int center(String dir) {
    /// Fuert den Dome in die Ausgangsposition //
   centerState = digitalRead(SENSOR_CENTER);  
   if (debug) {   
    Serial.print(F("CenterMode "));
    Serial.println(centerState);
   }
    //analogWrite(DMOT_R, 0); 
    //analogWrite(DMOT_L, 0); 
    DomeMot.write(90);
      
      while ( centerState == 0){

                 centerState = digitalRead(SENSOR_CENTER);  
                
                if (dir == "L" ) {
                  //analogWrite(DMOT_L, Rtempo); 
                  DomeMot.write(DOME_PWM_L);
                  digitalWrite(ledPin2, HIGH);                  
                } 
                
                if (dir == "R") {
                  //analogWrite(DMOT_R, Rtempo); 
                  DomeMot.write(DOME_PWM_R);
                  digitalWrite(ledPin1, HIGH); 
                }
                
                if (debug){Serial.println(F("try to get center"));Serial.print(SENSOR_CENTER);}
              
 
      }
   digitalWrite(ledPin1, LOW); 
   digitalWrite(ledPin2, LOW); 
   //analogWrite(DMOT_L, 0);  
   //analogWrite(DMOT_R, 0);  
   DomeMot.write(90);
   delay(200);
   durchlauf = 0;
   
   //Mode = 0; 
}

void FindRoTime(){
    //int rotime;
    center("L");
    if (debug){Serial.println(F("Dome ist Center"));}  
    delay(1000);
    zeit1 = millis();
    center("L");
    zeit2 = millis();
    if (debug){Serial.print(F("Zeit1_"));Serial.println(zeit1); Serial.print(F("Zeit2_")); Serial.println(zeit2);}
    rotime = zeit2-zeit1;
    if (debug){Serial.print(F("rotime_")); Serial.println(rotime);}
    Rpos = rotime/360; ///Rpos ist dann Winkel in Zeit Variable  90° also SUM*90;
    center("R");
    if (debug){Serial.print(F("Rpos_")); Serial.println(Rpos);}
}


void rotateR( int Rpos) {
  
     digitalWrite(ledPin1, HIGH); 
     DomeMot.write(DOME_PWM_R);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, Rtempo);
     delay (Rpos);
     digitalWrite(ledPin1, LOW); 
     DomeMot.write(90);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, 0);
          
}

void rotateL( int Rpos) {

     digitalWrite(ledPin2, HIGH); 
     //analogWrite(DMOT_R, 0); 
     //analogWrite(DMOT_L, Rtempo);
     DomeMot.write(DOME_PWM_L);
     
     delay (Rpos);
     digitalWrite(ledPin2, LOW); 
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, 0);
     DomeMot.write(90);
  
}


int rcMove() {

    int sensorValue = pulseIn(SENSOR_RC_IN,HIGH);

    if (debug){ 
      //  Serial.println(F("Rechts"));
      //   Serial.print(F("Sensor IN "));Serial.println(sensorValue);
      } 
    //return;
    int tempo = map (sensorValue, 530, 2000,0,180);
    
    if (sensorValue >=800){ ///Check if Sensor is Connectet an RC on

    
    if (sensorValue < 1450){
      
     
      int tempoR = map (sensorValue, 1460, 530,90,160);
     
      if (tempoR >= 180) {tempoR=255;}
         
     digitalWrite(ledPin2, HIGH); 
     DomeMot.write(tempoR);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, tempoR); 

     if (debug){ 
        Serial.println(F("Rechts"));
         Serial.print(F("tempoR "));Serial.println(tempoR);
      } 
    
    
    } else if (sensorValue > 1550) {
          
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      //tempo = sensorValue ;////6;
      int tempoL = map(sensorValue, 1450,2400,70,0);
      //tempo = tempo /5;
      DomeMot.write(tempoL);
      //analogWrite(DMOT_R, 0);  
      //analogWrite(DMOT_L, tempoL); 
      if (debug) {
      Serial.println(F("Links"));
      Serial.print(F("tempoL "));Serial.println(tempoL);
      }   
     
    }  else {
      //analogWrite(DMOT_L, 0);  
      digitalWrite(ledPin1, LOW); 
      //analogWrite(DMOT_R, 0); 
      digitalWrite(ledPin2, LOW); 
      DomeMot.write(90);    
      //delay (zeit);    
    }

    }///End Sensor Check
    
  if (debug) { 
    Serial.print(F("Tempo "));Serial.print(tempo);
    Serial.print(F("   Value "));Serial.println(sensorValue);
    }
  
}



void SendPing(){
  
    unsigned long currentMillis = millis();


       if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    if (Ping == true){
    Serial.println("Ping");
    Ping = false;
    }

    
    }
       
    
}


void human(){

  
     if (movementSensor.dataReady()) {
        ir1 = movementSensor.getRawIR1();
        ir2 = movementSensor.getRawIR2();
        ir3 = movementSensor.getRawIR3();
        ir4 = movementSensor.getRawIR4();
        temp = movementSensor.getTMP();
        movementSensor.startNextSample();
        Sdiff = ir2 - ir4 ; ///Differenz

        int gap = 200;  /// Lücke wo nichts verfolgt wird

        int range = ir2+ir4;  /// Abstand

        if (range >= 1000){
          // gap = 250;
          digitalWrite(ledPinC, HIGH);
          Ping = true;
            } else {
            digitalWrite(ledPinC, LOW);
            }

      // if (range > 3000) { //here}
        
        if (debug)
        {  
        //Serial.print(F("1:BOT["));
        //Serial.print(ir1);
        Serial.print(F(" 2:RH["));
        Serial.print(ir2);
        //Serial.print(F("], 3:TOP["));
        //Serial.print(ir3);
        Serial.print(F("], 4:LH["));
        Serial.print(ir4);
        Serial.print(F("], temp["));
        Serial.print(temp);
        Serial.print(F("], diff["));
        Serial.print(Sdiff);    
        Serial.print(F("]"));
        //Serial.print(F("], millis["));
        //Serial.print(millis());
        //Serial.print(F("]"));

        Serial.print("----range[");
        Serial.print(range);
        Serial.print("]----gap");
        Serial.print(gap);
        Serial.println();
        }        
        ///////////////////////////////////////
        //Bewegungs Kalkulation
        ///////////////////////////////////////
        //Bei RH Impulse nach rechts drehen bis LH Impuls gleich ist
           
        byte diff = false; 
   
        if ((Sdiff >= gap)||(Sdiff <= -gap)){ diff = true;}

        if (diff){

           if (ir4 > ir2){ ////turn right
                int Lpulse = map(ir4,-255,1600,70,90);
            
            if (debug){
               Serial.print("Dreh nach Links");
               Serial.println(Lpulse);
            }
            digitalWrite(ledPin2, HIGH);  //Dreh nach R
                //analogWrite(DMOT_L,Htempo); 
                //DomeMot.write(Htempo_L);
                DomeMot.write(Lpulse);
                //servoDispatch.moveTo(0,50,0,500);
                
           }    
           
            if (ir2 > ir4){ ////turn left
              int Rpulse = map(ir2,-255,1600,110,90);
              if (debug){
                Serial.print("Dreh nach Rechts");
                Serial.println(Rpulse);
              }
                digitalWrite(ledPin1, HIGH);  //Dreh nach L
                //analogWrite(DMOT_R,Htempo);
                //DomeMot.write(Htempo_R); 
                DomeMot.write(Rpulse); 
                //servoDispatch.moveTo(0,50,0,1200);
                
           } 
        } else {
          //analogWrite(DMOT_R, 0); 
          //analogWrite(DMOT_L, 0); 
          digitalWrite(ledPin2, LOW);  //Dreh nach L
          digitalWrite(ledPin1, LOW);  //Dreh nach R
          DomeMot.write(90);
          //Serial.println("--Mitte--");
         // tempPos = 90;
          
        }

      /*****Vertikal system Holo ***/
       
        vpos1 = map(ir3, -1300, 800, 180, 90); ///verstellung nach oben
        vpos2 = map(ir1, -1300, 800, 0, 90);   /// Verstellung nach unten

        Vdiff = ir3 - ir1 ; ///Differenz
        
        Hdiff = false;
        
        if ((Vdiff >= 200)||(Vdiff <= -200)){ Hdiff = true;}

        if (Hdiff){

           if (ir3 > ir1){ ////turn top
               //Serial.println("Dreh nach oben");
               tempPos = vpos1;
           }    
           
            if (ir1 > ir3){ ////turn bot
                //Serial.println("Dreh nach unten");
                tempPos = vpos2;
           } 
        } else {
          
          //Serial.println("--Mitte--");
          //tempPos = 90;
          
        }
           
        HoloV.write(tempPos); 
        delay(50);
        if (debug){
        Serial.print ("action ");
        Serial.print (Hdiff);
        
        Serial.print ("Temp Pos ");
        Serial.print (tempPos);
       // Serial.print ("  S 3  ");
        //Serial.print (vpos1);
        Serial.print (" raw O ");
        Serial.print (ir3);
         
        //7Serial.print ("  S 3 unten ");
        //Serial.print(ir3);
        //Serial.print (vpos2);
        Serial.print (" raw U ");
        Serial.print(ir1);
        Serial.print("   Vdiff: ");
        Serial.println(Vdiff);
        }
    }
    
  
}
