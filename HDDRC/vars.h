#include <Arduino.h>                           

String data; //Consohlen Input
String VERSION = "2-04-21-HDDRC";

byte debug = false;
long randNumber;
long zeit;
unsigned long previousMillis = 0;        // will store last time LED was updated
// constants won't change:
const long interval = 6000;           // interval at which to blink (milliseconds)

//Move Sensor
byte NachL = false;
byte NachR = false;
byte Mitte = false;
byte Ping = false;

int Sdiff = 0;

///PIN ARRANGEMENT

#define DOME_PULSE_IN 21 //RC Signal DomeRotation
//#define BODY_PULSE_IN 16 //RC Signal Body Move

#define DMOT_L 5  //L298 MotorIN1
#define DMOT_R 6  //L298 MotorIN2

#define BMOT_L 7  //L298 MotorIN3
#define BMOT_R 4  //L298 MotorIN4

#define STATUS_PIN 16  // Body Sensor or Input Ping
#define PWM_OUT 10 // PWM Holo V Movement

#define SENSOR_CENTER 8  //Center Sensor
#define SENSOR_RC_IN 21  //PWM INPUT Sensor Dome Rotation

#define LEG_POTI 9 // Position Reading Leg

#define M_LEFT 1
#define M_RIGHT 2
#define M_STOP 3

//Body Positionen

#define B_TOP  450 //Body Hoch
#define B_DOWN  800 //Body Down
#define B_CENT 620 //Boby Center
#define D_ZONE 20 // Death Zone
bool mov = true;
int MO = 0;
int tPos = B_CENT;   // Target Position Body Roll


//int tempo = 200;
int Htempo = 170; //Human Tracking Tempo
int Rtempo = 200; // Autorotation Tempo
int tempo = 200; // kalkuliertes Tempo




int faktor = 100;
unsigned long Rpos = 35;  //Zeit von Center to Zielposition in Millisec
int durchlauf = 0;
int sensorValue = 1500; ///Umschalt Pulse

int rotime;
unsigned long zeit1, zeit2;
//int sensorRC = 21; ///von 2 auf 21 geänder wg I2C  Dome Rotation

//int sensorRC_out = 10;
int Mode = 2;// 0=RandMove // 1=RCMove  //2=human //3=Service
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
