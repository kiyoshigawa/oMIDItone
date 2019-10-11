//This is a quick program to test PCA9685 servo controls before implementation in the oMIDItone class for the face-grabbers.
//I'm using the Adafruit libraries available on github here:
// https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library

#include <Wire.h>
#include "Adafruit_PWMServoDriver.h"

uint8_t address = 0x41;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(address);

//The MG90S Servo I am using has a pulse range of 400us to 2400us.
//running at 240Hz, this is 4,167 us per cycle, subdivided into 4069 steps.
//This translated to ~1 us per pulse length count.
#define SERVOMIN  400 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  2400 // this is the 'maximum' pulse length count (out of 4096)

int l_min = 1450;
int l_max = l_min+220;
int r_min = 1280;
int r_max = r_min-220;

void setup(){
  //enable OE pin
  pinMode(20, OUTPUT);
  digitalWrite(20, LOW);

  pwm.begin();
  
  pwm.setPWMFreq(240);

  delay(10);

  Serial.begin(9600);

  pwm.setPWM(4, 0, l_min);
  pwm.setPWM(5, 0, r_min);
}

void loop(){
    /*
    delay(1000);
    pwm.setPWM(5, 0, l_max);
    pwm.setPWM(4, 0, r_max);

    delay(1000);
    pwm.setPWM(5, 0, l_min);
    pwm.setPWM(4, 0, r_min);
    delay(1000);
    for(int i=0; i<127; i = i+10){
      pwm.setPWM(5, 0, map(i, 0, 127, l_min, l_max));
      pwm.setPWM(4, 0, map(i, 0, 127, r_min, r_max));
      delay(50);
    }
    for(int i=127; i>=0; i = i-10){
      pwm.setPWM(5, 0, map(i, 0, 127, l_min, l_max));
      pwm.setPWM(4, 0, map(i, 0, 127, r_min, r_max));
      delay(50);
    }
    */
    int servo_pulse_length = r_min;
    while(Serial.available()){
      servo_pulse_length = Serial.parseInt(0);
      Serial.print("Setting servo pulse length to ");
      Serial.println(servo_pulse_length);
      pwm.setPWM(5, 0, servo_pulse_length);
      delay(1000);
    }
}