#include <Arduino.h>                           

String data; //Consohlen Input

String VERSION = "23-04-22-HDDRC-B";

String output;

//#define DEBUG
byte debug = false;
byte Ping = false;
byte dir = false;  //false = left  true == right 
int Sdiff = 0;

///DOME MOTOR L298
#define DMOT_A 5
#define DMOT_B 6    //6  mit 7

// Leg Motor L298 oder bt2
#define LMOT_A 4   // 4 before
#define LMOT_B 7   //7 tausche mit 6

#define STATUS_PIN 16  // Body Sensor or Input Ping
#define PWM_OUT 10 // 10 PWM Holo V Movement
#define SENSOR_CENTER 8  //Center Sensor
#define SENSOR_RC_IN 21  //PWM INPUT Sensor Dome Rotation
#define LEG_POTI 9 // 9 Before Position Reading Leg


//Body Positionen

#define B_TOP  300 //Body Hoch
#define C_DRIVEPOS 120//
#define B_DOWN  700 //Body Down
#define B_CENT 500 //Boby Center
#define D_ZONE 20 // Death Zone
bool mov = true;

//int MO = 0;
int tPos = B_CENT;   // Target Position Body Roll
/* 
###############################
MOTOR SPEED AND MOVE DEFINITONS 
###############################
*/
#define LEFT 0
#define RIGHT 1

// Gegenrichtung
//#define LEFT 1
//#define RIGHT 0

#define STOP 3

int gap = 200;  /// 200 LOW 300 HIGH Lücke wo nichts verfolgt wird

/// Speed Definitions usualy between 0 and 255 max

int Htempo = 180; //Human Tracking Tempo
byte domeAutoSpeed = 220;     // Speed used when dome automation is active - Valid Values: 50 - 100

int Ltemp_R = 100;          //Leg Speed rechts
int Ltemp_L = 100;         //Leg Motor Speed links

int CenterSpeed = 240;       // Speed for getting Center
int NormSpeed = 240;         // Speed for usual Point Rotation

int RC_SPEED_MIN = 150;    
int RC_SPEED_MAX = 255;



/// GRIP ARM Positions
#define ARM_IN  40
#define ARM_OUT 150



int faktor = 100;
unsigned long Rpos = 13;  //Zeit von Center to Zielposition in Millisec
int durchlauf = 0;
int sensorValue = 1500; ///Umschalt Pulse

int rotime;
unsigned long zeit1, zeit2;
//int sensorRC = 21; ///von 2 auf 21 geänder wg I2C  Dome Rotation

//int sensorRC_out = 10;
int Mode = 0;// 0=RandMove // 1=RCMove  //2=human //3=Service // 4= Debug
int moving = 0;

//#### Status Signal fom 16  ACTUAL INACTIVE

//#define USE_STATUS_PIN



int stat=0;

int vpos1;
int vpos2;
int Vdiff;
byte Hdiff;

int pos = 0;    // variable to store the servo position
int tempPos ;

int centerState = 0;

const int ledPinC =  20;  
const int ledPin1 =  18;  
const int ledPin2 =  19;  

int ledStateC = LOW;
int ledState1 = LOW; 
int ledState2 = LOW; 
