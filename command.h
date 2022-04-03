#include <Arduino.h>    

//// Commandos ///////
void ProzessComando() {

       
      
      Serial.print("Data =");
      Serial.println(data);
    if(data == "GH"){      
      gap = 300;
    }

    if(data == "GM"){      
      gap = 250;
    }
    
    if(data == "GL"){
      gap = 200;
    }  

    if(data == "T"){
      //BodyRot(B_TOP);
      tPos = B_TOP;
    }

    if(data == "D"){
      //LMotor(B_DOWN);
      tPos = B_DOWN;
    }

    if(data == "DRIVE"){
      //LMotor(B_DOWN);
      tPos = C_DRIVEPOS;
    }
    
    if(data == "C"){
      //LMotor(M_STOP);
      tPos = B_CENT;
    }


    if(data == "O"){
      //LMotor(M_STOP);
      //tPos = B_CENT;
     Serial.println("Arm out");
     
     ArmSrv.write(ARM_OUT);
    }
    
    if(data == "I"){
      //LMotor(M_STOP);
      //tPos = B_CENT;
     Serial.println("ARM_IN");
     ArmSrv.write(ARM_IN);
    }

    

    if (data == "hi-r2" )
      {             
       Serial.println("test gefunden");
       
      }
      
    if (data == "stop" )
      {         
       Serial.println("reset player");
       
       
      }

    if (data == "find")
    {
      FindRoTime();
    }
    
    if (data == "debug on")
      {
        Serial.println("Debug Mode ON");
        debug = true;      
      }

    if (data == "debug off")
      {
        Serial.println("Debug Mode off");
        debug = false;
      }
    if (data == "vers")   
      {
        Serial.println(F("-----   Versionnsnummer   -----"));
        Serial.println(VERSION);     
        /*
         * Vorher 73% 77%
         */
        Serial.println(F("|    Komandoreferenz"));
        Serial.println(F("| debug on"));
        Serial.println(F("| debug off"));
        Serial.println(F("| stop  : resetAll"));
        Serial.println(F("| hi-r2 : test"));
        Serial.println(F("| vers  : Print Info"));
        Serial.println(F("| find  : calc Position"));      
        Serial.println(F("| mode0 : RAND Move"));
        Serial.println(F("| mode1 : RC Move"));
        Serial.println(F("| mode2 : Human detect"));
        Serial.println(F("| mode3 : Service"));    
        Serial.println(F("| usb  : Turn to USB"));
        Serial.println(F("| p1  : Turn to Panel 1"));
        Serial.println(F("| p2  : Turn to Panel 2"));
        Serial.println(F("| p3  : Turn to Panel 3"));
        Serial.println(F("| p4  : Turn to Panel 4"));
        Serial.println(F("| tool1"));
        Serial.println(F("| tool2"));
        Serial.println(F("| tool3")); 
        Serial.println(F("| nono"));
        Serial.println(F("| yea"));
        Serial.println(F("| dance"));
        Serial.println(F("| D  : Body Down"));
        Serial.println(F("| T  : Body UP"));
        Serial.println(F("| C  : Body Center"));
        Serial.println(F("| center: Turn Dome to Center"));
        Serial.println(F("---Rpos---"));
        Serial.println(Rpos);
        Serial.println(F("---Temparatur---"));
        Serial.println(temp);
        Serial.print("#___Actual Mode:");
        Serial.println(Mode);
        Serial.println(F("...ready for Command_"));

      }

    if (data == "mode1"){
      Serial.println(F("Mode 1 RC"));
      //center("L");
      //delay(1000);
      Mode = 1;
      }
    if (data == "mode0"){
      Serial.println(F("Mode 0 Random"));
      //delay(1000);
      Mode = 0;
      }
    if (data == "mode3"){
      Serial.println(F("Mode 3 Service"));
      //delay(1000);
      Mode = 3;
      }
    if (data == "mode4"){
      Serial.println(F("Mode 4 Service"));
      //delay(1000);
      Mode = 4;
      }

      
    if (data == "mode2"){
      Serial.println(F("Mode 2 Human"));
      //delay(1000);
      Mode = 2;
      }  
     
    if (data == "usb"){
      Serial.println(F("Rotation zu Position USB")); rotateR(Rpos*80);  }
    
    if (data == "CB0"){Serial.println(F("Rotation zu Position USB")); rotateR(Rpos*90);}
    
    if (data == "p1"){Serial.println(F("Rotation zu Position P1")); rotateR(Rpos*95);}
    
    if (data == "p2"){Serial.println(F("Rotation zu Position P2")); rotateR(Rpos*100);}
    
    if (data == "p3"){Serial.println(F("Rotation zu Position P3")); rotateR(Rpos*105);}
    
    if (data == "p4"){Serial.println(F("Rotation zu Position P4")); rotateR(Rpos*110);}
    
    if (data == "tool1"){Serial.println(F("Rotation zu Position tool1"));  rotateR(Rpos*110); }
    
    if (data == "tool2"){Serial.println(F("Rotation zu Position tool2")); rotateR(Rpos*140); }
    
    if (data == "tool3"){Serial.println(F("Rotation zu Position tool3")); rotateR(4000); } 
     
    if (data == "nono"){Serial.println(F("Rotation nono"));
    rotateR(Rpos*50);
    rotateL(Rpos*50);
    }
    
    if (data == "yea"){Serial.println("Rotation yea");}

    if (data =="dance") { 
      
      rotateL(70);
      delay(1000);
      rotateR(110);
      
      }

    if (data =="center") {


        if (dir == true)
        {
          center("L");
          dir = false;

          
        } else {
          center("R");
          dir = true;
        }
      
      
      }
    data = "";
}



void Comand() {
///Comando Pruefung////
   if(Serial.available() > 0)
    {
        
        data = Serial.readStringUntil('\n');         
        ProzessComando();       
    }
  ////Prüfung 2///
  MainInput.listen();
  
   if (MainInput.available() > 0)
    {
        data = MainInput.readStringUntil('\r');
        Serial.println("main");
        Serial.println(data);
        ProzessComando();
        
    }
}
