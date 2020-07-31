#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "kmotor.h"
#include "stdlib.h"
#include "stdio.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);
int state;
kmotor _kmotor(true);


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

int speedL = 250;
int speedR = 250;

void setup() {
  Serial.begin(9600);
    _kmotor.cauhinh();
  lcd.init();
  lcd.backlight();
    pinMode(PIN_DO1, INPUT);
  pinMode(PIN_DO2, INPUT);
  pulses1 = 0;
  pulses2 = 0;
  
  attachInterrupt(digitalPinToInterrupt(PIN_DO1), counter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_DO2), counter2, FALLING);
}

// F go forward
// L left
// S stop
// R Right
// B reserve
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
  state = 'o';
  if (Serial.available() > 0) {
    state = Serial.read();
    Serial.print(state);
  }
  switch (state) {
   case 'F': {
        turn('U');
      }
  break;
  case 'L': {
     turn('L');
     delay(280);
    _kmotor.tien(0, 0);
    _kmotor.tien(1, 0);     
  }
  break;
  case 'R': {
      turn('R');
     delay(280);
     _kmotor.tien(0, 0);
    _kmotor.tien(1, 0);    
  }
  break;
  case 'B': {
     turn('B');
     delay(520);
    _kmotor.tien(0, 0);
    _kmotor.tien(1, 0);     
  }
  break;
    case 'S': {
        turn('S');
  }
  break;
  }
}
void turn(int _move){
  switch(_move){
    case 'L': 
    lcd.setCursor(0, 1);   
    lcd.print("Left");   
    _kmotor.tien(0, -speedL);
    _kmotor.tien(1, speedR);
    lcd.clear();
    break;
    case 'R': 
    lcd.setCursor(0, 1);
    lcd.print("Right");
    _kmotor.tien(0, speedL);
    _kmotor.tien(1, -speedR);
    lcd.clear();
    break;
        case 'U': 
    lcd.setCursor(0, 1);
    lcd.print("Up");
    _kmotor.tien(0, speedL);
    _kmotor.tien(1, speedR);
    break;
        case 'B': 
    lcd.setCursor(0, 1);
    lcd.print("Reserve");
    _kmotor.tien(0, -speedL);
    _kmotor.tien(1, speedR);
    break;
        case 'S': 
    lcd.setCursor(0, 1);
    lcd.print("Stop");
    _kmotor.tien(0, 0);
    _kmotor.tien(1, 0);
    break;
  }
}
