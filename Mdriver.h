#include <Arduino.h>       

bool inRange(int val, int minimum, int maximum){
  return ((minimum <= val)&&(val <= maximum));
}

////SMC Controler Stuff /////
// Based On following Skipt
// required to allow motors to move
// must be called when controller restarts and after any error
void exitSafeStart()
{
  smcSerial.write(0x83);
}
 
// speed should be a number from -3200 to 3200
void setMotorSpeed(int speed)
{
  if (speed < 0)
  {
    smcSerial.write(0x86);  // motor reverse command
    speed = -speed;  // make speed positive
  }
  else
  {
    smcSerial.write(0x85);  // motor forward command
  }
  smcSerial.write(speed & 0x1F);
  smcSerial.write(speed >> 5 & 0x7F);
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
      digitalWrite(ledPin2, LOW); 
      digitalWrite(ledPin1, LOW); 
      STOP
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
      //heute LegMot.write(Ltemp_L);  // Dreh Left
      FORWARD
   }

    if ((potPos <= tPos) && (mov == true)) {
      if (debug) {   
      Serial.print(" right ");
      }
      digitalWrite(ledPin1, HIGH); 
      //heute LegMot.write(Ltemp_R);  // Dreh Right
      REVERSE
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
    //analogWrite(DMOT_R, 0); 
    //analogWrite(DMOT_L, 0); 
    DomeMot.write(90);
      
      while ( centerState == 0){

                 centerState = digitalRead(SENSOR_CENTER);  
                
                if (dir == "L" ) {
                  //analogWrite(DMOT_L, Rtempo); 
                  //DomeMot.write(70);
                  DomeMot.write(40);
                  digitalWrite(ledPin2, HIGH);                  
                } 
                
                if (dir == "R") {
                  //analogWrite(DMOT_R, Rtempo); 
                  //DomeMot.write(120);
                  DomeMot.write(140);
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


void rotateR( int Rposi) {
  
     digitalWrite(ledPin1, HIGH); 
     DomeMot.write(DOME_PWM_R);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, Rtempo);
     delay (Rposi);
     digitalWrite(ledPin1, LOW); 
     DomeMot.write(90);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, 0);

     if (debug){
      Serial.print("PWM ");
      Serial.print(DOME_PWM_R);
      Serial.print(" Time ");
      Serial.println(Rposi);
     }
     
          
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
      
     
      int tempoR = map (sensorValue, 1460, 1200,90,0);
     
     // if (tempoR >= 180) {tempoR=255;}
         
     digitalWrite(ledPin2, HIGH); 
     DomeMot.write(tempoR);
     //analogWrite(DMOT_L, 0); 
     //analogWrite(DMOT_R, tempoR); 

     if (debug){ 
        Serial.print(F("Rechts"));
         Serial.print(F("tempoR "));Serial.println(tempoR);
      } 
    
    
    } else if (sensorValue > 1550) {
          
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      //tempo = sensorValue ;////6;
      int tempoL = map(sensorValue, 1450,1900,90,160);
      //tempo = tempo /5;
      DomeMot.write(tempoL);
      //analogWrite(DMOT_R, 0);  
      //analogWrite(DMOT_L, tempoL); 
      if (debug) {
      Serial.print(F("Links"));
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
            digitalWrite(ledPin2, HIGH);  //Dreh nach L
                //analogWrite(DMOT_L,Htempo); 
                //DomeMot.write(Htempo_L);
                DomeMot.write(Lpulse);
                //servoDispatch.moveTo(0,50,0,500);
                
           }    
           
            if (ir2 > ir4){ ////turn right
              int Rpulse = map(ir2,-255,1600,140,90);
              //int Rpulse = 120;
              
              if (debug){
                Serial.print("Dreh nach Rechts");
                Serial.println(Rpulse);
              }
                digitalWrite(ledPin1, HIGH);  //Dreh nach R
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
