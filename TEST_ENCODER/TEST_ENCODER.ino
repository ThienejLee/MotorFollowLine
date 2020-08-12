#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "kmotor.h"
#include "stdlib.h"
#include "stdio.h"
using namespace std;

 #define outputA A3 
 #define outputB 12
 
 #define outputC A0   
 #define outputD 2
 int errorP=0;
 int errorD=0;
 int counterR = 0; 
 int aState;
 int aLastState;  
 int tocdo=0;
 int counterL = 0; 
 int _aState;
 int _aLastState;  
 int oldP=0;
kmotor _kmotor(true);
void setup() {
  Serial.begin(9600);
 _kmotor.cauhinh();
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);
   
   pinMode (outputC,INPUT);
   pinMode (outputD,INPUT);
   
   aLastState = digitalRead(outputA);
   
   _aLastState = digitalRead(outputC);  
}

void Encode(){
     aLastState = aState;
   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputB) != aState) { 
       counterR ++;
     } else {
       counterR ++;
     }
//     Serial.print("PositionLeft: ");
//     Serial.println(counter);
   } 
      _aLastState = _aState; 
    _aState = digitalRead(outputC); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (_aState != _aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(outputD) != _aState) { 
       counterL ++;
     } else {
       counterL ++;
     }
//     Serial.print("PositionRight: ");
//     Serial.println(_counter);
   } 
   _aLastState = _aState; 

   //Xuat ra counter va _counter  
}


void loop() {
  
  
  Encode();
  Serial.println(counterR-counterL);
 errorP=(counterR-counterL);
 errorD=errorP-oldP;
 int tocdo=16*6*errorP+49*6*errorD;
 oldP=errorP;
 if(tocdo>255){
  tocdo=255;
 }
 if(tocdo<-255){
  tocdo=-255;
 }
 if(tocdo>0){
  _kmotor.tien(0,255);
  _kmotor.tien(1,255-tocdo);
 }
 else{
  _kmotor.tien(0,255+tocdo);
  _kmotor.tien(1,255);
 }

}
