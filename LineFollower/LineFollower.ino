/*
   Demo line-following code for the Pololu Zumo Robot

   This code will follow a black line on a white background, using a
   PID-based algorithm.  It works decently on courses with smooth, 6"
   radius curves and has been tested with Zumos using 30:1 HP and
   75:1 HP motors.  Modifications might be required for it to work
   well on different courses or with different motors.

   http://www.pololu.com/catalog/product/2506
   http://www.pololu.com
   http://forum.pololu.com
*/
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>



#include "kmotor.h"
#include "stdlib.h"
#include "stdio.h"
using namespace std;



#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2




U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

QTRSensorsRC qtrrc((unsigned char[]) {
  4,5,9,10,11,12},
NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];



ZumoBuzzer buzzer;
ZumoReflectanceSensorArray reflectanceSensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
int last = 0;

// This is the maximum speed the motors will be allowed to turn.
// (400 lets the motors go at top speed; decrease to impose a speed limit)
const int MAX_SPEED = 255;

#define HOLES_DISC 20


kmotor _kmotor(true);






void setup()
{
  u8g2.begin();
  Serial.begin(9600);
  _kmotor.cauhinh();
  // Play a little welcome song
  //  buzzer.play(">g32>>c32");

  // Initialize the reflectance sensors module
  reflectanceSensors.init();
  //
  //  // Wait for the user button to be pressed and released
  //  button.waitForButton();

  // Turn on LED to indicate we are in calibration mode
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  int i;
  for (i = 0; i < 80; i++)
  {
    if ((i > 10 && i <= 30) || (i > 50 && i <= 70)) {
      _kmotor.tien(0, -100);
      _kmotor.tien(1, -100);
    }
    else {
      _kmotor.tien(0, 100);
      _kmotor.tien(1, 100);
    }
    reflectanceSensors.calibrate();

    // Since our counter runs to 80, the total delay will be
    // 80*20 = 1600 ms.
    delay(40);
  }
  _kmotor.tien(0, 0);
  _kmotor.tien(1, 0);

  // Turn off LED to indicate we are through with calibration
  digitalWrite(13, LOW);
  //  buzzer.play(">g32>>c32");

  //  // Wait for the user button to be pressed and released
  //  button.waitForButton();
  //
  //  // Play music and wait for it to finish before we start driving.
  //  buzzer.play("L16 cde33333greg4");
  //  while(buzzer.isPlay3ing());
}

void loop()
{
  unsigned int sensors[6];
  unsigned int position1 = qtrrc.readLine(sensorValues);

  // print the sensor values as numbers from 0 to 1000, where 0 means maximum reflectance and
  // 1000 means minimum reflectance, followed by the line position
  for (unsigned char i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  //Serial.println(); // uncomment this line if you are using raw values
  Serial.println(position1); // comment this line out if you are using raw values

  // Get the position of the line.  Note that we *must* provide the "sensors"
  // argument to readLine() here, even though we are not interested in the
  // individual sensor readings
  int position = reflectanceSensors.readLine(sensors);
  int dolech = position - 2500;
  int derivative = dolech - last;
  int integral=+dolech;
  last = dolech;
  int tocdo = dolech*1/12+derivative*0.2 ;
  if ( tocdo > MAX_SPEED) {
    tocdo = MAX_SPEED;
  }
  if (tocdo < -MAX_SPEED) {
    tocdo = -MAX_SPEED;
  }
  if (tocdo < 0) {
    _kmotor.tien(0, MAX_SPEED);
    _kmotor.tien(1, MAX_SPEED+tocdo);
  }
  else {
    _kmotor.tien(0, MAX_SPEED-tocdo );
    _kmotor.tien(1, MAX_SPEED);
  }

}
