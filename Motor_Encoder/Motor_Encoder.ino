#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "kmotor.h"
#include "stdlib.h"
#include "stdio.h"
using namespace std;

//KHAI BAO DO LINE

int F[20];




//ENCODER INIT
float rpm1;
float rpm2;


#define PIN_DO1 2
#define PIN_DO2 A0


volatile unsigned int pulses1;

volatile unsigned int pulses2;

unsigned long timeOld1;
unsigned long timeOld2;



#define HOLES_DISC 20





int speedL = 200;
int speedR = 200;


kmotor _kmotor(true);
void setup() {
  Serial.begin(9600);
  _kmotor.cauhinh();
  
  pinMode(PIN_DO1, INPUT);
  pinMode(PIN_DO2, INPUT);
  pulses1 = 0;
  pulses2 = 0;
  
  attachInterrupt(digitalPinToInterrupt(PIN_DO1), counter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_DO2), counter2, FALLING);



}

// ENCODE COUNTER for not miss some disc
void counter1()
{
  pulses1++;
}

void counter2()
{
  pulses2++;
}

int time_now = 0;

int demSoLo1 = 0;
int demSoLo2 = 0;

int item1 = 0;
int item2 = 0;

int oldItem1 = 0;
int oldItem2 = 0;

void XuLySoLo1() {
  if (millis() - timeOld1 >= 0){
    detachInterrupt(digitalPinToInterrupt(PIN_DO1));
    rpm1 = (pulses1 * 60) / (HOLES_DISC);
    item1 = digitalRead(PIN_DO1);
    if (item1 != oldItem1  ) {
      demSoLo1 ++;
      oldItem1 = item1;
    }
    timeOld1 = millis();
    attachInterrupt(digitalPinToInterrupt(PIN_DO1), counter1, FALLING);
  }
}

void XuLySoLo2() {
  if (millis() - timeOld2 >= 0){
    detachInterrupt(digitalPinToInterrupt(PIN_DO2));
    rpm2 = (pulses2 * 60) / (HOLES_DISC);
    item2 = digitalRead(PIN_DO2);
    if (item2 != oldItem2  ) {
      demSoLo2 ++;
      oldItem2 = item2;
    }
    timeOld2 = millis();
    attachInterrupt(digitalPinToInterrupt(PIN_DO2), counter2, FALLING);
  }
}
void loop() {
  XuLySoLo1();
//  XuLySoLo2();
//  if(demSoLo1/2 >= 100){
//    _kmotor.tien(0, -speedL);
//    _kmotor.tien(1, speedR);
//  }
int tam = 0;
  Serial.println(demSoLo1/2);
//  
    if(demSoLo1/2 <140){
      turn('U');
    } 
    else if (demSoLo1/2 == 140){
    turn('R');
            delay(300);
    demSoLo1++;
    tam = demSoLo1;
    } 
    else if (demSoLo1/2 -tam <= 50){
      turn('U');
      if(demSoLo1 -tam == 50){
        demSoLo1 = 170;
      }
    }
    else if(demSoLo1/2 == 190){
      turn('R');
              delay(260);
      demSoLo1++;
      tam = demSoLo1;
    } 
    else if(demSoLo1/2 >= tam){
      turn('U');
    }
    
}
