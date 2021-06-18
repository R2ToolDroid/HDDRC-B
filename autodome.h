
// Dome Automation Variables
boolean domeAutomation = true;
int domeTurnDirection = 1;  // 1 = positive turn, -1 negative turn
float domeTargetPosition = 0; // (0 - 359) - degrees in a circle, 0 = home
unsigned long domeStopTurnTime = 0;    // millis() when next turn should stop
unsigned long domeStartTurnTime = 0;  // millis() when next turn should start
int domeStatus = 0;  // 0 = stopped, 1 = prepare to turn, 2 = turning
int time360DomeTurn = 2500;  // milliseconds for dome to complete 360 turn at domeAutoSpeed - Valid Values: 2000 - 8000 (2000 = 2 seconds)
byte domeAutoSpeed = 50;     // Speed used when dome automation is active - Valid Values: 50 - 100

byte isCenter = false;
byte findCenter = true;
byte domePos = false;
unsigned long domeFindCenterTime = 0;
unsigned long CurDomeFindTime = 0;
unsigned long maxFindTime = 3000;
///Position Find
int DomeGyroPos = 0;
bool DomeTurnPos = false;
int TargetPos = 0;




void autoDome() {

    long rndNum;
    int domeSpeed;
    isCenter = false;

    //output += "\r\n - Dome Status=";
    //output +=    domeStatus      ; 
    
    if (domeStatus == 0)  // Dome is currently stopped - prepare for a future turn
    {
      
        if (domeTargetPosition == 0)  // Dome is currently in the home position - prepare to turn away
        {
          
            domeStartTurnTime = millis() + (random(3, 10) * 1000);          
            rndNum = random(5,354);
            domeTargetPosition = rndNum;  // set the target position to a random degree of a 360 circle - shaving off the first and last 5 degrees
            
            if (domeTargetPosition < 180)  // Turn the dome in the positive direction
            {
              
                domeTurnDirection = 1;
                
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);
              
            } else  // Turn the dome in the negative direction
            {
                    
                domeTurnDirection = -1;
                
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);
              
            }
          
        } else  // Dome is not in the home position - send it back to home
        {
          
            domeStartTurnTime = millis() + (random(3, 10) * 1000);
            
            if (domeTargetPosition < 180)
            {
              
                domeTurnDirection = -1;
                
                domeStopTurnTime = domeStartTurnTime + ((domeTargetPosition / 360) * time360DomeTurn);
              
            } else
            {
                    
                domeTurnDirection = 1;
                
                domeStopTurnTime = domeStartTurnTime + (((360 - domeTargetPosition) / 360) * time360DomeTurn);
              
            }
            
            domeTargetPosition = 0;
          
        }
      
        domeStatus = 1;  // Set dome status to preparing for a future turn
               
        #ifdef DEBUG
          output += "Dome Automation: Initial Turn Set";
          output +=  "Current Time: ";
          output +=  millis();
          output += "- Next Start Time: ";
          output += domeStartTurnTime;
          output += " - ";
          output += "Next Stop Time: ";
          output += domeStopTurnTime;
          output += " - ";          
          output += "Dome Target Position: ";
          output += domeTargetPosition;
          output += "\r\n ";
        #endif

    }
    
    
    if (domeStatus == 1)  // Dome is prepared for a future move - start the turn when ready
    {
      
        if (domeStartTurnTime < millis())
        {
          
             domeStatus = 2; 
             
             #ifdef DEBUG
                output += "Dome Automation: Ready To Start Turn\r\n";
             #endif
          
        }
    }
    
    if (domeStatus == 2) // Dome is now actively turning until it reaches its stop time
    {
      
        if (domeStopTurnTime > millis())
        {
          
              domeSpeed = domeAutoSpeed * domeTurnDirection;

              int DomePulseSpeed = map(domeSpeed,-100,100,70,110);
          
              //SyR->motor(domeSpeed);
             DomeMot.write(DomePulseSpeed);
              
             #ifdef DEBUG
               output += "Turning Now!!\r\n";
                
             #endif
          
          
        } else  // turn completed - stop the motor
        {
              domeStatus = 0;
              //SyR->stop();
              DomeMot.write(90);
              
              #ifdef DEBUG
               output += "STOP TURN!!\r\n";
              #endif
        }
      
    }

  if (output !=""){
    Serial.println(output);
    output="";
  }

}
