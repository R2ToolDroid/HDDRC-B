#include <Arduino.h>       

bool inRange(int val, int minimum, int maximum){
  return ((minimum <= val)&&(val <= maximum));
}

void resetM(){
   digitalWrite(ledPin1, LOW); 
   digitalWrite(ledPin2, LOW); 
   ledState1 = LOW;
   ledState2 = LOW;
   analogWrite(DMOT_L, 0);  
   analogWrite(DMOT_R, 0);  

   
}

void LMotor(int MO){   
    tempo = 200;    
    
    switch (MO){
      case M_LEFT:
      if (debug) {   
      Serial.print(" left ");   
      } 
      digitalWrite(ledPin2, HIGH); 
      analogWrite(BMOT_R, 0); 
      analogWrite(BMOT_L, tempo); 
      break;

      case M_RIGHT:
      if (debug) {   
      Serial.print(" right ");
      }
      digitalWrite(ledPin1, HIGH); 
      analogWrite(BMOT_L, 0); 
      analogWrite(BMOT_R, tempo);
      break; 

      case M_STOP:
      if (debug) {   
      Serial.print(" stop ");
      }
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      analogWrite(BMOT_L, 0); 
      analogWrite(BMOT_R, 0);
      break;

      default:
      ///do nothing
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      analogWrite(BMOT_L, 0); 
      analogWrite(BMOT_R, 0);
      break;
      
    }
  
}


void BodyRot(int tPos) {   // Rotiert zu tPos

    int potPos = analogRead(LEG_POTI);

    //inRange (x, 0, 200)
    

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

   
   
    LMotor(MO);
    if (debug) {   
    Serial.print(" Poti Pos = ");
    //Serial.print();  
    Serial.print(potPos);
    Serial.print(" | Tpos = ");
    Serial.println(tPos);
    }
/*
    if ((potPos >= GAB_A)&&(potPos <= B_TOP)) {
      Serial.print("left");
      MO = M_LEFT;   
      //LMotor(MO); 
      //tempo = map(potPos, 600,1050,100,255); 
    } 
    if ((potPos <= GAB_B)&&(potPos >= B_DOWN)){
      Serial.print("right");
      MO = M_RIGHT;  
      //LMotor(MO);   
     // tempo = map(potPos, 400,0,100,255);
    }
    if ((potPos >= GAB_B)&&(potPos <= GAB_A)){
      Serial.print("stop");
      MO = M_STOP;
      tempo = 0;
    }  
    if ((potPos >= B_TOP)||(potPos <= B_DOWN)){
      Serial.print("stop");
      MO = M_STOP;
      tempo = 0;
    }
    LMotor(MO);
*/
    
}




int center(String dir) {
    /// Fuert den Dome in die Ausgangsposition //
   centerState = digitalRead(SENSOR_CENTER);  
   if (debug) {   
    Serial.print(F("CenterMode "));
    Serial.println(centerState);
   }
    analogWrite(DMOT_R, 0); 
    analogWrite(DMOT_L, 0); 
      
      while ( centerState == 0){

                 centerState = digitalRead(SENSOR_CENTER);  
                
                if (dir == "L" ) {
                  analogWrite(DMOT_L, Rtempo); 
                  digitalWrite(ledPin2, HIGH);                  
                } 
                
                if (dir == "R") {
                  analogWrite(DMOT_R, Rtempo); 
                  digitalWrite(ledPin1, HIGH); 
                }
                
                if (debug){Serial.println(F("try to get center"));Serial.print(SENSOR_CENTER);}
              
 
      }
   digitalWrite(ledPin1, LOW); 
   digitalWrite(ledPin2, LOW); 
   analogWrite(DMOT_L, 0);  
   analogWrite(DMOT_R, 0);  
   
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
     analogWrite(DMOT_L, 0); 
     analogWrite(DMOT_R, Rtempo);
     delay (Rpos);
     digitalWrite(ledPin1, LOW); 
     analogWrite(DMOT_L, 0); 
     analogWrite(DMOT_R, 0);
          
}

void rotateL( int Rpos) {

     digitalWrite(ledPin2, HIGH); 
     analogWrite(DMOT_R, 0); 
     analogWrite(DMOT_L, Rtempo);
     delay (Rpos);
     digitalWrite(ledPin2, LOW); 
     analogWrite(DMOT_L, 0); 
     analogWrite(DMOT_R, 0);
    
  
}


int rcMove() {

    int sensorValue = pulseIn(SENSOR_RC_IN,HIGH);
    
    if (sensorValue >=800){ ///Check if Sensor is Connectet an RC on

    
    if (sensorValue < 1350){
      
     
      int tempoR = map (sensorValue, 1460, 530,50,254);
     
      if (tempoR >= 180) {tempoR=255;}
         
     digitalWrite(ledPin2, HIGH); 
     analogWrite(DMOT_L, 0); 
     analogWrite(DMOT_R, tempoR); 

     if (debug){ 
        Serial.println(F("Rechts"));
         Serial.print(F("tempoR "));Serial.println(tempoR);
      } 
    
    
    } else if (sensorValue > 1650) {
          
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      //tempo = sensorValue ;////6;
      int tempoL = map(sensorValue, 1450,2400,50,254);
      //tempo = tempo /5;
      
      analogWrite(DMOT_R, 0);  
      analogWrite(DMOT_L, tempoL); 
      if (debug) {
      Serial.println(F("Links"));
      Serial.print(F("tempoL "));Serial.println(tempoL);
      }   
     
    }  else {
      analogWrite(DMOT_L, 0);  
      digitalWrite(ledPin1, LOW); 
      analogWrite(DMOT_R, 0); 
      digitalWrite(ledPin2, LOW);     
      //delay (zeit);    
    }

    }///End Sensor Check
    
  if (debug) { 
    //Serial.print(F("Tempo "));Serial.println(tempo);
    //Serial.print(F("Value "));Serial.println(sensorValue);
    }
  
}

void randomMove() {
 
   // print a random number from 0 to 299
  zeit = random(3000, 6000)+(temp-20)*faktor;
  
  // print a random number from 10 to 19
  randNumber = random(10, 40);
  //Speed
  int tempo = random(90,120);
  //Moving länge
  moving = random(500,1500);
 
  if (randNumber < 20){     ///Drehung Links
    if (debug){
    Serial.println(F("Links"));
     Serial.println(randNumber);
    }
     // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);
     analogWrite(DMOT_L, tempo);  
     delay(moving);
     analogWrite(DMOT_L, 0);  
     digitalWrite(ledPin2, LOW);
     delay(500);
    
    } else if (randNumber > 20 && randNumber <= 30) {  ///Rechts Drehung

     if (debug) {
      Serial.println(F("rechts"));
      Serial.println(randNumber);
     }
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH);   
      analogWrite(DMOT_R, tempo); 
      delay(moving);      
      digitalWrite(ledPin1, LOW); 
      analogWrite(DMOT_R, 0); 
      delay(500);
      
    }  else {
      
      analogWrite(DMOT_L, 0);  
      analogWrite(DMOT_R, 0); 
      digitalWrite(ledPin1, LOW); 
      digitalWrite(ledPin2, LOW); 
      delay (zeit);  
    }

  if (debug) {
      Serial.print(F("Zeit "));
      Serial.println(zeit);
      Serial.print(F("Druchlauf "));
      Serial.println(durchlauf);
      Serial.print(F("Temp0 "));
      Serial.println(tempo);
      Serial.print(F("Temp "));
      Serial.println(temp);
      Serial.print(F("Faktor "));
      Serial.println(temp*faktor);
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
        //Serial.print(F("], temp["));
        //Serial.print(temp);
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
               // Serial.println("Dreh nach Rechts");
                digitalWrite(ledPin2, HIGH);  //Dreh nach R
                analogWrite(DMOT_L,Htempo); 
                //servoDispatch.moveTo(0,50,0,500);
                
           }    
           
            if (ir2 > ir4){ ////turn left
                //Serial.println("Dreh nach DMOT_L");
                digitalWrite(ledPin1, HIGH);  //Dreh nach L
                analogWrite(DMOT_R,Htempo); 
                //servoDispatch.moveTo(0,50,0,1200);
                
           } 
        } else {
          analogWrite(DMOT_R, 0); 
          analogWrite(DMOT_L, 0); 
          digitalWrite(ledPin2, LOW);  //Dreh nach L
          digitalWrite(ledPin1, LOW);  //Dreh nach R
          
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
