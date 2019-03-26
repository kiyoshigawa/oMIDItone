/*
Author: kiyoshigawa
Start Date: 2018-03-18
Description: 
Took an Otamatone, and replaced the pressure-sensitive variable resistor strip with some MCP4151 50kohm variable pots (2 in series) with a 220Ohm fixed resistor.
Then used a teensy LC MIDI library to control the resistance from the chips over SPI, and connected it to the otamatone, making a oMIDItome.

Update: 2019-03-23: converted the code into a class, rewrote everything to work with the class, can run up to 6 on a Teensy LC.

THINGS TO DO IN THE FUTURE:
Interpret MIDI velocity signals and PWM the relay_pin for volume control
Interpret pitch shift messages and adjust frequency accordingly

*/

#include "oMIDItone.h"

#define NUM_OMIDITONES 1

//comment this to disable serial functions for testing notes.
#define DEBUG

// Pin Definitions
int om1_relay_pin = 26;
int om1_cs1_pin = 14;
int om1_cs2_pin = 15;
int om1_analog_feedback_pin = A2;
int om2_relay_pin = 10;
int om2_cs1_pin = 17;
int om2_cs2_pin = 18;
int om2_analog_feedback_pin = A5;
int om3_relay_pin = 9;
int om3_cs1_pin = 8;
int om3_cs2_pin = 7;
int om3_analog_feedback_pin = A6;
int om4_relay_pin = 6;
int om4_cs1_pin = 25;
int om4_cs2_pin = 24;
int om4_analog_feedback_pin = A7;
int om5_relay_pin = 4;
int om5_cs1_pin = 5;
int om5_cs2_pin = 2;
int om5_analog_feedback_pin = A8;
int om6_relay_pin = 3;
int om6_cs1_pin = 1;
int om6_cs2_pin = 0;
int om6_analog_feedback_pin = A9;
//Using SPI0 on board, MOSI0 = 11, MISO0 = 12, and SCK0 = 13, which will blink the LED as it sends.

//declare the oMIDItone objects:
oMIDItone om1(om1_relay_pin, om1_cs1_pin, om1_cs2_pin, om1_analog_feedback_pin);
//oMIDItone om2(om2_relay_pin, om2_cs1_pin, om2_cs2_pin, om2_analog_feedback_pin);
//oMIDItone om3(om3_relay_pin, om3_cs1_pin, om3_cs2_pin, om3_analog_feedback_pin);
//oMIDItone om4(om4_relay_pin, om4_cs1_pin, om4_cs2_pin, om4_analog_feedback_pin);
//oMIDItone om5(om5_relay_pin, om5_cs1_pin, om5_cs2_pin, om5_analog_feedback_pin);
//oMIDItone om6(om6_relay_pin, om6_cs1_pin, om6_cs2_pin, om6_analog_feedback_pin);

//this is a variable storing the pitch shift value last received over MIDI:
int16_t current_pitch_shift = CENTER_PITCH_SHIFT;

//This function will set the resistors to the note, the pwm on the relay_pin to a velocity (not sure if this will work).
//The most recent note will always override, since the otamatone only has one output.
//the channel should be 1 for low mode, 2 for mid mode, and 3 for high mode.
void OnNoteOn(byte channel, byte note, byte velocity){
  om1.note_on(note);
  #ifdef DEBUG
    Serial.print("Note ");
    Serial.print(note);
    Serial.println(" playing.");
  #endif
}

//This resets everything so it is ready for the next note:
void OnNoteOff(byte channel, byte note, byte velocity){
  om1.note_off(note);
  #ifdef DEBUG
    Serial.println("Current note ended.");
  #endif
}

//this sets the pitch_shift value to be used by the oMIDItones:
void myPitchChange(byte channel, int pitch){
  current_pitch_shift = pitch;
  om1.set_pitch_shift(current_pitch_shift);
  #ifdef DEBUG
    Serial.print("Pitch Shift set to ");
    Serial.print(current_pitch_shift);
    Serial.println(".");
  #endif
}

void setup(){
  //init the om objects:
  om1.init();
  //om2.init();
  //om3.init();
  //om4.init();
  //om5.init();
  //om6.init();

  //call out MIDI functions
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandlePitchChange(myPitchChange);
}

void loop(){
  //This will read for MIDI notes and start/stop the notes based on MIDI input:
  usbMIDI.read();

  //This will update the oMIDItone objects:
  om1.update();
  //om2.update();
  //om3.update();
  //om4.update();
  //om5.update();
  //om6.update();
}
