#include <Arduino.h>       

bool inRange(int val, int minimum, int maximum){
  return ((minimum <= val)&&(val <= maximum));
}

void DomeMot( int Direction, int speed){

    if (Direction == 1) {  //LEFT
      analogWrite(DMOT_A, speed);
      analogWrite(DMOT_B, 0);
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, HIGH);
    }

    if (Direction == 0) {
      analogWrite(DMOT_A, 0);
      analogWrite(DMOT_B, speed);
      digitalWrite(ledPin2, HIGH); 
      digitalWrite(ledPin1, LOW);
    }

    if ((speed == 0) ||(Direction == STOP)) {
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      analogWrite(DMOT_A, 0);
      analogWrite(DMOT_B, 0);
      
    }
}
          /// LegMot(RIGHT,255);
void LegMot( int Direction, int speed){

    if (Direction == 1) {
      //analogWrite(LMOT_A, 0);
      //analogWrite(LMOT_B, speed);
      SoftPWMSet(LMOT_A, 0);
      SoftPWMSet(LMOT_B, speed);
      //LEG_A.detach();
      //LEG_B.attach(LMOT_B);
      //LEG_B.write(180);
      
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, HIGH);
    }

    if (Direction == 0) {
      //analogWrite(LMOT_A, speed);
      //analogWrite(LMOT_B, 0);
      SoftPWMSet(LMOT_A, speed);
      SoftPWMSet(LMOT_B, 0);

      //LEG_B.detach();
      //LEG_A.attach(LMOT_A);
      //LEG_A.write(180);
      
      digitalWrite(ledPin2, HIGH); 
      digitalWrite(ledPin1, LOW);
    }

    if ((speed == 0)||(Direction == STOP) ) {
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      //analogWrite(LMOT_A, 0);
      //analogWrite(LMOT_B, 0);
      SoftPWMSet(LMOT_A, 0);
      SoftPWMSet(LMOT_B, 0);

      //LEG_A.write(90);
      //LEG_B.write(90);
      //LEG_B.detach();
      //LEG_A.detach();
    }
}



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
      //STOP
      LegMot(STOP,0);
      mov = false;
      
      
   
   }    else {
    mov = true;    
    
   }

  
   if ( (potPos >= tPos) && (mov == true) ){
      if (debug) {   
      Serial.print(" left ");   
      } 
      
      LegMot(LEFT,Ltemp_L);
   }

    if ((potPos <= tPos) && (mov == true)) {
      if (debug) {   
      Serial.print(" right ");
      }
     
      LegMot(RIGHT,Ltemp_R);
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
    Serial.print(centerState);
    Serial.print(F("Dir "));
    Serial.println(dir);
   }
    
     DomeMot(STOP,0);
      
      while ( centerState == 0){

                 centerState = digitalRead(SENSOR_CENTER);  
                
                if (dir == "L" ) {
                  
                  DomeMot(LEFT,CenterSpeed);
                                 
                } 
                
                if (dir == "R") {
                  
                  DomeMot(RIGHT,CenterSpeed);
                 
                }
                
                if (debug){Serial.println(F("try to get center"));Serial.print(SENSOR_CENTER);}

      }

   DomeMot(STOP,0);
   delay(200);
   durchlauf = 0;
   
   
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


void rotateR( int Rposi) {
    
     DomeMot(RIGHT,NormSpeed);   
     delay (Rposi); 
     DomeMot(STOP,0);
    
          
}

void rotateL( int Rpos) {

     DomeMot(LEFT,NormSpeed);
     delay (Rpos);
     DomeMot(STOP,0);
  
}


int rcMove() {

    int sensorValue = pulseIn(SENSOR_RC_IN,HIGH);

    if (debug){ 
       Serial.print(F("Rechts"));
       Serial.print(F("Sensor IN "));Serial.println(sensorValue);
      } 
    //return;
    //int tempo = map (sensorValue, 530, 2000,0,180);
    
    if (sensorValue >=800){ ///Check if Sensor is Connectet an RC on

    
    if (sensorValue < 1450){
      
     
      int tempoR = map (sensorValue, 1460, 1150,RC_SPEED_MIN,RC_SPEED_MAX);
     
     // if (tempoR >= 180) {tempoR=255;}
         
     //digitalWrite(ledPin2, HIGH); 
     //DomeMot.write(tempoR);
     DomeMot(LEFT,tempoR);
      //LegMot(LEFT,tempoR);
     
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, tempoR); 

     if (debug){ 
        //Serial.print(F("Rechts"));
         Serial.print(F("  tempoR "));Serial.println(tempoR);
      } 
    
    
    } else if (sensorValue > 1550) {
          
     // set the LED with the ledState of the variable:
      //digitalWrite(ledPin1, HIGH); 
      //tempo = sensorValue ;////6;
      int tempoL = map(sensorValue, 1450,1850,RC_SPEED_MIN,RC_SPEED_MAX);
      //tempo = tempo /5;
      //DomeMot.write(tempoL);
      DomeMot(RIGHT,tempoL);
      //LegMot(RIGHT,tempoL);
      //analogWrite(DMOT_R, 0);  
      //analogWrite(DMOT_L, tempoL); 
      if (debug) {
      Serial.print(F("Links"));
      Serial.print(F("tempoL "));Serial.println(tempoL);
      }   
     
    }  else {
      //analogWrite(DMOT_L, 0);  
      //digitalWrite(ledPin1, LOW); 
      //analogWrite(DMOT_R, 0); 
      //digitalWrite(ledPin2, LOW); 
      DomeMot(STOP,0);
      //DomeMot.write(90);    
      //delay (zeit);    
    }

    }///End Sensor Check
    
  if (debug) { 
    //Serial.print(F("TempoL "));Serial.print(tempoL);
    //Serial.print(F("  TempoR "));Serial.print(tempoR);
    //Serial.print(F("   Value "));Serial.println(sensorValue);
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
        //Serial.print(F(" 2:RH["));
        //Serial.print(ir2);
        //Serial.print(F("], 3:TOP["));
        //Serial.print(ir3);
        //Serial.print(F("], 4:LH["));
        //Serial.print(ir4);
        //Serial.print(F("], temp["));
        //Serial.print(temp);
        //Serial.print(F("], diff["));
        //Serial.print(Sdiff);    
        //Serial.print(F("]"));
        //Serial.print(F("], millis["));
        //Serial.print(millis());
        //Serial.print(F("]"));

        //Serial.print("----range[");
        //Serial.print(range);
        //Serial.print("]----gap");
        //Serial.print(gap);
        //Serial.println();
        }        
        ///////////////////////////////////////
        //Bewegungs Kalkulation
        ///////////////////////////////////////
        //Bei RH Impulse nach rechts drehen bis LH Impuls gleich ist
           
        byte diff = false; 
   
        if ((Sdiff >= gap)||(Sdiff <= -gap)){ diff = true;}

        if (diff){

           if (ir4 > ir2){ ////turn left
                int Lpulse = map(ir4,-255,1600,70,90);
                //int Lpulse = 70;
            
            if (debug){
               Serial.print("Dreh nach Links");
               Serial.println(Lpulse);
            }
           
                DomeMot(LEFT,Htempo);
              
                
           }    
           
            if (ir2 > ir4){ ////turn right
              int Rpulse = map(ir2,-255,1600,140,90);
              //int Rpulse = 120;
              
              if (debug){
                Serial.print("Dreh nach Rechts");
                Serial.println(Rpulse);
              }

                DomeMot(RIGHT,Htempo);
               
                
           } 
        } else {
          
          DomeMot(STOP,0);
         
          
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
        //Serial.print ("action ");
        //Serial.print (Hdiff);
        
        //Serial.print ("Temp Pos ");
        //Serial.print (tempPos);
       // Serial.print ("  S 3  ");
        //Serial.print (vpos1);
        //Serial.print (" raw O ");
        //Serial.print (ir3);
         
        //Serial.print ("  S 3 unten ");
        //Serial.print(ir3);
        //Serial.print (vpos2);
        //Serial.print (" raw U ");
        //Serial.print(ir1);
        //Serial.print("   Vdiff: ");
        //Serial.println(Vdiff);
        }
    }
    
  
}
