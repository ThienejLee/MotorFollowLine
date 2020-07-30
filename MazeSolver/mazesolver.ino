 #include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "kmotor.h"
#include "stdlib.h"
#include "stdio.h"

#include <SharpIR.h>

#define IR1 A0 // define signal pin

#define model1 430
SharpIR SharpIR (IR1, model1);
LiquidCrystal_I2C lcd(0x27,16,2);
kmotor _kmotor(true);
//Khai báo chân của 3 cảm biến siêu âm 
 
 const int triggerL=9;
 const int echoL=10;  
 int toida=255;
 int last1=0;
 float distanceR;
 float distanceL;
 const int triggerM=4;
 const int echoM=5;
 float distanceM;
 const int triggerR=11;
 const int echoR=12;
//Khai báo dãy đường đi
char path[100] = "";
unsigned char path_length = 0;
//Khai bao chan
int sensor1 = 11;
int sensor2 = 13;
int steps1=0;
int steps2=0;
int b1=digitalRead(sensor1);
int b2=digitalRead(sensor2);
void setup(){
  lcd.init();
  lcd.backlight();
   pinMode(sensor1,INPUT_PULLUP);
   pinMode(sensor2,INPUT_PULLUP);
//Khai báo hàm động cơ
_kmotor.cauhinh();
//Khai báo hàm thị giá trị
 Serial.begin(9600);
//Khai báo 3 siêu âm 
pinMode(triggerL,OUTPUT);
pinMode(echoL,INPUT);
//pinMode(triggerM,OUTPUT);
//pinMode(echoM,INPUT);
pinMode(triggerR,OUTPUT);
pinMode(echoR,INPUT);  
}
void sieuamL() {
  digitalWrite(triggerL,LOW);
 delayMicroseconds(5);
 // Start Measurement
 digitalWrite(triggerL,HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerL,LOW);
 // Acquire and convert to mtrs
 distanceL=pulseIn(echoL,HIGH);
 distanceL=distanceL*0.01657;

}
void sieuamM() {
  digitalWrite(triggerM,LOW);
 delayMicroseconds(5);
 // Start Measurement
 digitalWrite(triggerM,HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerM,LOW);
 // Acquire and convert to mtrs
 distanceM=pulseIn(echoM,HIGH);
 distanceM=distanceM*0.01657;
}
void sieuamR() {
  digitalWrite(triggerR,LOW);
 delayMicroseconds(5);
 // Start Measurement
 digitalWrite(triggerR,HIGH);
 delayMicroseconds(10);
 digitalWrite(triggerR,LOW);
 // Acquire and convert to mtrs
 distanceR=pulseIn(echoR,HIGH);
 distanceR=distanceR*0.01657;
}
void loop(){
   solveMaze();
}
void dithang() {
  sieuamL();
  sieuamR();
  int dolech1=(distanceR-11)*10;
  int derivative1=dolech1-last1;
  last1=dolech1;
  int tocdo1=16/4*dolech1+49/4*derivative1;
 if (tocdo1>toida){
  tocdo1=toida;
 }
 if (tocdo1<-toida){
  tocdo1=-toida;
 }
  if (tocdo1 <0){
  _kmotor.tien(0,toida+tocdo1);
  _kmotor.tien(1,toida);
 }
 else {
  _kmotor.tien(0,toida);
  _kmotor.tien(1,toida-tocdo1);
}
}
void turn(char dir)
{
  switch(dir)
  {
    case 'L':
      // Turn left.
     lcd.setCursor(0,1);
     lcd.print("L");
     _kmotor.tien(0,80);
     _kmotor.tien(1,255);
     delay(1000);
     
    
    break;
    case 'R':
      // Turn right.
     lcd.setCursor(0,1);
     lcd.print("R");
     _kmotor.tien(0,255);
     _kmotor.tien(1,255);
     delay(500);
     _kmotor.tien(0,255);
     _kmotor.tien(1,120);
     delay(750);
 
//     _kmotor.tien(0,255);
//     _kmotor.tien(1,130);
//     delay(700);
//     _kmotor.tien(0,-10);
//     _kmotor.tien(1,-10);
//     delay(50);
    break;
    case 'S':
    //go straight
    lcd.setCursor(0,1);
    lcd.print("S");
    break;
    case 'B':
    //go straight
    lcd.setCursor(0,1);
    lcd.print("B");
    _kmotor.tien(0,0);
    _kmotor.tien(1,0);
    delay(1000);
    _kmotor.tien(0,255);
    _kmotor.tien(1,-255);
    delay(750);
      break;
    }
}
char selectTurn(unsigned char found_right, unsigned char found_straight,
  unsigned char found_left,unsigned char found_botton)
{
  // Make a decision about how to turn.  The following code
  // implements a left-hand-on-the-wall strategy, where we always
  // turn as far to the left as possible.
  if(found_right)
    return 'R';
  else if(found_straight)
    return 'S';
  else if(found_left)
    return 'L';
  else if(found_botton)
    return 'B';
}
void solveMaze()
{
    while(1)
    {
        // Navigate current line segment
        dithang();
        unsigned char found_left = 0;
        unsigned char found_straight = 0;
        unsigned char found_right = 0;
         unsigned char found_botton = 0;
        
      unsigned long pepe1=millis();  // takes the time before the loop on the library begins
  int dis=SharpIR.distance();   
      sieuamM();
      sieuamL();
      sieuamR();
         
        // Check for left and right exits.
        if(dis>20){
            found_right=1;
        }
        if(distanceL>20 && distanceR<30 && distanceM<30){
            found_left = 1;
        }
        if(distanceM>20 && distanceR<30){
            found_straight = 1;
        }
        if(distanceM<5 && distanceR<20 && distanceL<20){
            found_botton = 1;
        }
     
        // Intersection identification is complete.
        unsigned char dir = selectTurn(found_right, found_straight, found_left,found_botton);
    turn(dir);
        path[path_length] = dir;
        path_length++;       
    }
}
