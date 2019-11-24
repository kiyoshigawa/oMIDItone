/*
Author: kiyoshigawa
Start Date: 2018-03-18
Description:
Took an Otamatone, and replaced the pressure-sensitive variable resistor strip with some MCP4151 50kohm variable pots (2 in series) with a 220Ohm fixed resistor.
Then used a teensy LC MIDI library to control the resistance from the chips over SPI, and connected it to the otamatone, making a oMIDItone.

Update: 2019-03-23: converted the code into a class, rewrote everything to work with the class, can run up to 6 on a Teensy 3.2, only one on a Teensy LC due to lack of RAM.
Update: 2019-03-27: Got all the features working, should be a fully functional MIDI instrument, pending testing of analog read speeds keeping up with higher frequency notes when more than 2 oMIDItones are running. I only have 2 for testing atm, need to buy more.
Update: 2019-09-26: This is now configured (at least initially) for the fully functional v3 PCB, which includes servo face grabbers, LED lighting, and 6 heads. I also added support for per-channel pitch bending, so you can have a channel bend but not all the others with it. Any notes on the bent chanel will be bent as well, though. Working on timing for animation updates, and then I will need to improve the lighting and servo animations. Once those two things are done, I think we can call it stable and mark it as the official release.
Update 2019-11-10: Added MIDI functionality to control various aspects of the oMIDItone using CC messages, and offloaded the MIDI handling to the MIDI_control.h file.

*/

#include "oMIDItone.h"
#include <MIDI.h>
#include <Adafruit_NeoPixel.h>
#include "lighting_control.h"

//this allows me to reset the teensy when it receives a MIDI CC121 reset command.
#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location

void _softRestart() 
{
  Serial.end();  //clears the serial monitor  if used
  SCB_AIRCR = 0x05FA0004;  //write value for restart
}

//comment this to disable serial functions for testing notes.
#define DEBUG

//defines to make the head selection code more readable:
#define AVAILABLE true
#define NOT_AVAILABLE false

//This is the pulse offset in microseconds from the min value of the servo position to the max (i.e. fully open)
#define SERVO_MAX_OFFSET 220

//The next few #defines are for the Adafruit_Neopixel.h library stuff:
#define LED_TOTAL_NUM NUM_LEDS_PER_HEAD*NUM_OMIDITONES
#define LED_DATA_PIN 2
#define LED_COLOR_ORDER NEO_GRB
#define LED_SPEED NEO_KHZ800
#define LED_BRIGHTNESS 255

//This controls the lighting animation timing. 16667 ~= 60Hz, 33333 ~= 30Hz
#define TIME_BETWEEN_LIGHTING_UPDATES 33333

//these are default lighting info for the heads:
#define DEFAULT_BG_MODE LC_BG_RAINBOW_SLOW_ROTATE
#define DEFAULT_FG_MODE LC_FG_NONE
#define DEFAULT_TRIGGER_MODE LC_TRIGGER_FLASH

#define DEFAULT_OM1_BG_RAINBOW 16
#define DEFAULT_OM2_BG_RAINBOW 17
#define DEFAULT_OM3_BG_RAINBOW 18
#define DEFAULT_OM4_BG_RAINBOW 19
#define DEFAULT_OM5_BG_RAINBOW 20
#define DEFAULT_OM6_BG_RAINBOW 21
#define DEFAULT_OM1_FG_RAINBOW 1
#define DEFAULT_OM2_FG_RAINBOW 1
#define DEFAULT_OM3_FG_RAINBOW 1
#define DEFAULT_OM4_FG_RAINBOW 1
#define DEFAULT_OM5_FG_RAINBOW 1
#define DEFAULT_OM6_FG_RAINBOW 1
#define DEFAULT_OM1_TRIGGER_RAINBOW 3
#define DEFAULT_OM2_TRIGGER_RAINBOW 5
#define DEFAULT_OM3_TRIGGER_RAINBOW 9
#define DEFAULT_OM4_TRIGGER_RAINBOW 13
#define DEFAULT_OM5_TRIGGER_RAINBOW 7
#define DEFAULT_OM6_TRIGGER_RAINBOW 11

// Pin and other head-specific Definitions
//om#_leds[] arrays are per head ordered from left to right, the first 6 are front leds, the next 6 are the back top, and the final 6 are the back bottom leds
//Red Head:
uint16_t om1_se_pin = 15;
uint16_t om1_sd_pin = 26;
uint16_t om1_cs1_pin = 17;
uint16_t om1_cs2_pin = 16;
uint16_t om1_analog_feedback_pin = A10;
uint16_t om1_l_min = 1390;
uint16_t om1_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om1_r_min = 1340;
uint16_t om1_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om1_l_channel = 0;
uint16_t om1_r_channel = 1;
uint16_t om1_leds[NUM_LEDS_PER_HEAD] = {12, 13, 14, 15, 16, 17, 95, 94, 93, 92, 91, 90, 84, 85, 86, 87, 88, 89};
//Yellow Head:
uint16_t om2_se_pin = 33;
uint16_t om2_sd_pin = 25;
uint16_t om2_cs1_pin = 22;
uint16_t om2_cs2_pin = 21;
uint16_t om2_analog_feedback_pin = A11;
uint16_t om2_l_min = 1475;
uint16_t om2_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om2_r_min = 1430;
uint16_t om2_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om2_l_channel = 2;
uint16_t om2_r_channel = 3;
uint16_t om2_leds[NUM_LEDS_PER_HEAD] = {23, 22, 21, 20, 19, 18, 59, 58, 57, 56, 55, 54, 48, 49, 50, 51, 52, 53};
//White Head:
uint16_t om3_se_pin = 24;
uint16_t om3_sd_pin = 30;
uint16_t om3_cs1_pin = 3;
uint16_t om3_cs2_pin = 23;
uint16_t om3_analog_feedback_pin = A13;
uint16_t om3_l_min = 1280;
uint16_t om3_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om3_r_min = 1450;
uint16_t om3_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om3_l_channel = 4;
uint16_t om3_r_channel = 5;
uint16_t om3_leds[NUM_LEDS_PER_HEAD] = {6, 7, 8, 9, 10, 11, 101, 100, 99, 98, 97, 96, 78, 79, 80, 81, 82, 83};
//Black Head:
uint16_t om4_se_pin = 10;
uint16_t om4_sd_pin = 29;
uint16_t om4_cs1_pin = 9;
uint16_t om4_cs2_pin = 8;
uint16_t om4_analog_feedback_pin = A12;
uint16_t om4_l_min = 1475;
uint16_t om4_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om4_r_min = 1310;
uint16_t om4_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om4_l_channel = 6;
uint16_t om4_r_channel = 7;
uint16_t om4_leds[NUM_LEDS_PER_HEAD] = {29, 28, 27, 26, 25, 24, 65, 64, 63, 62, 61, 60, 42, 43, 44, 45, 46, 47};
//Green Head:
uint16_t om5_se_pin = 32;
uint16_t om5_sd_pin = 28;
uint16_t om5_cs1_pin = 7;
uint16_t om5_cs2_pin = 6;
uint16_t om5_analog_feedback_pin = A14;
uint16_t om5_l_min = 1475;
uint16_t om5_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om5_r_min = 1420;
uint16_t om5_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om5_l_channel = 8;
uint16_t om5_r_channel = 9;
uint16_t om5_leds[NUM_LEDS_PER_HEAD] = {0, 1, 2, 3, 4, 5, 107, 106, 105, 104, 103, 102, 72, 73, 74, 75, 76, 77};
//Blue Head:
uint16_t om6_se_pin = 31;
uint16_t om6_sd_pin = 27;
uint16_t om6_cs1_pin = 5;
uint16_t om6_cs2_pin = 4;
uint16_t om6_analog_feedback_pin = A0;
uint16_t om6_l_min = 1335;
uint16_t om6_l_max = om1_l_min + SERVO_MAX_OFFSET;
uint16_t om6_r_min = 1410;
uint16_t om6_r_max = om1_r_min - SERVO_MAX_OFFSET;
uint16_t om6_l_channel = 14;
uint16_t om6_r_channel = 15;
uint16_t om6_leds[NUM_LEDS_PER_HEAD] = {35, 34, 33, 32, 31, 30, 71, 70, 69, 68, 67, 66, 36, 37, 38, 39, 40, 41};

//this array controls the type of notes being sent to each head on note_trigger events when they are active:
uint16_t note_trigger_type[NUM_OMIDITONES] = {DEFAULT_TRIGGER_MODE, 
                                              DEFAULT_TRIGGER_MODE, 
                                              DEFAULT_TRIGGER_MODE, 
                                              DEFAULT_TRIGGER_MODE, 
                                              DEFAULT_TRIGGER_MODE, 
                                              DEFAULT_TRIGGER_MODE};

//these are the Animation objects for the heads above - they will control the animation per-head.
Animation om1_animation = Animation(om1_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM1_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM1_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM1_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);
Animation om2_animation = Animation(om2_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM2_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM2_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM2_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);
Animation om3_animation = Animation(om3_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM3_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM3_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM3_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);
Animation om4_animation = Animation(om4_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM4_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM4_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM4_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);
Animation om5_animation = Animation(om5_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM5_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM5_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM5_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);
Animation om6_animation = Animation(om6_leds, NUM_LEDS_PER_HEAD, 
                                    (DEFAULT_BG_MODE | DEFAULT_FG_MODE), 
                                    rb_array[DEFAULT_OM6_BG_RAINBOW], 
                                    rb_array[DEFAULT_OM6_FG_RAINBOW], 
                                    rb_array[DEFAULT_OM6_TRIGGER_RAINBOW], 
                                    LC_DEFAULT_REFRESH_RATE);

//This is the LED strip object to control all the lighting on the board
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_TOTAL_NUM, LED_DATA_PIN, LED_COLOR_ORDER + LED_SPEED);

//this is the lighting controller that will control the lighting updates on the above Adafruit_NeoPixel object's pixels:
LightingControl lc = LightingControl(&strip, LED_BRIGHTNESS);

//this is a timing variable to track lighting updates
elapsedMicros last_lighting_update;

//Using SPI0 on board, MOSI0 = 11, MISO0 = 12, and SCK0 = 13, which will blink the LED as it sends.

//declare the oMIDItone objects:
oMIDItone oms[NUM_OMIDITONES] = {
  oMIDItone(om1_se_pin, om1_sd_pin, om1_cs1_pin, om1_cs2_pin, om1_analog_feedback_pin, om1_l_channel, om1_r_channel, om1_l_min, om1_l_max, om1_r_min, om1_r_max, om1_leds, &om1_animation),
  oMIDItone(om2_se_pin, om2_sd_pin, om2_cs1_pin, om2_cs2_pin, om2_analog_feedback_pin, om2_l_channel, om2_r_channel, om2_l_min, om2_l_max, om2_r_min, om2_r_max, om2_leds, &om2_animation),
  oMIDItone(om3_se_pin, om3_sd_pin, om3_cs1_pin, om3_cs2_pin, om3_analog_feedback_pin, om3_l_channel, om3_r_channel, om3_l_min, om3_l_max, om3_r_min, om3_r_max, om3_leds, &om3_animation),
  oMIDItone(om4_se_pin, om4_sd_pin, om4_cs1_pin, om4_cs2_pin, om4_analog_feedback_pin, om4_l_channel, om4_r_channel, om4_l_min, om4_l_max, om4_r_min, om4_r_max, om4_leds, &om4_animation),
  oMIDItone(om5_se_pin, om5_sd_pin, om5_cs1_pin, om5_cs2_pin, om5_analog_feedback_pin, om5_l_channel, om5_r_channel, om5_l_min, om5_l_max, om5_r_min, om5_r_max, om5_leds, &om5_animation),
  oMIDItone(om6_se_pin, om6_sd_pin, om6_cs1_pin, om6_cs2_pin, om6_analog_feedback_pin, om6_l_channel, om6_r_channel, om6_l_min, om6_l_max, om6_r_min, om6_r_max, om6_leds, &om6_animation)
};

//this is a variable storing the pitch shift value last received over MIDI:
//Note the teensy library uses a signed pitch shift centered on 0 instead of the MIDI standard of centering on 8192.
int16_t current_pitch_shift[NUM_MIDI_CHANNELS];

//this is an array that tracks whether a note is currently on or off.
uint8_t current_note_array[NUM_MIDI_NOTES];

//this will note the velocity that corresponds to the note array above.
uint8_t current_velocity_array[NUM_MIDI_NOTES];

//this will note the channel that corresponds to the note array above.
uint8_t current_channel_array[NUM_MIDI_NOTES];

//this tracks how many notes are in the array.
uint8_t num_current_notes = 0;

//this is a bool to let the controller know when it needs to reassign notes to the heads.
bool note_has_changed = false;

//this is a bool to let the controller know when it needs to reassign pitch shift to the heads.
bool pitch_has_changed = false;

//This tracks the current head so the iteration isn't always in the same place.
int head_offset = 0;

//this controls whether lighting is enabled or not
bool lighting_is_enabled = true;

//this controls whether note on messages send triggers to the head that plays the note:
bool note_trigger_is_enabled = true;

//this is an array that is sorted in the order of the least recently used heads to the most recently used heads
//it allows for the least recently used heads to be selected first when deciding whish head will play a note
uint8_t head_order_array[NUM_OMIDITONES];
//this stores the state of the head order array until all notes are off, then it is pushed into the actual head order array.
uint8_t pending_head_order_array[NUM_OMIDITONES];

//Start the hardware MIDI:
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

//this is just a quick function to print the note array with some formatting:
void print_current_note_array(){
  if(num_current_notes > 0){
    Serial.print("Notes: [");
    if(num_current_notes > 0){
      for(int i=0; i<num_current_notes-1; i++){
        Serial.print(current_note_array[i]);
        Serial.print(", ");
      }
    }
    Serial.print(current_note_array[num_current_notes-1]);
    Serial.println("]");
  }
  else{
    Serial.println("Notes: []");
  }
}

//This will check to see if a note is in the current_note_array and return the position of the note if it finds one, or return NO_NOTE if it does not.
int check_note(uint8_t note){
  //Iterate through the array, and return the note position if it is found.
  if(num_current_notes == 0){
    return NO_NOTE;
  }
  else{
    for(int i=0; i<num_current_notes; i++){
      if(current_note_array[i] == note){
        return i;
      }
    }
    //if no note is found, return NO_NOTE;
    return NO_NOTE;
  }
}

//This will add a new note to the end of the current_note_array, or relocate the note to the end if it is already in the array.
//It will also add a velocity value corresponding to that note to the current_velocity_array.
void add_note(uint8_t note, uint8_t velocity, uint8_t channel){
  uint8_t note_position = check_note(note);
  //if the note isn't already in the array, put it at the end of the array.
  if(note_position == NO_NOTE){
    current_note_array[num_current_notes] = note;
    current_velocity_array[num_current_notes] = velocity;
    current_channel_array[num_current_notes] = channel;
    num_current_notes++;
  }
  //if it is in the array, put the note at the end and shift everything back down to where the note used to be.
  else{
    for(int i=note_position; i<num_current_notes; i++){
      current_note_array[i] = current_note_array[i+1];
      current_velocity_array[i] = current_velocity_array[i+1];
      current_channel_array[i] = current_channel_array[i+1];
    }
    current_note_array[num_current_notes-1] = note;
    current_velocity_array[num_current_notes-1] = velocity;
    current_channel_array[num_current_notes-1] = channel;
  }
}

//If a note is in the current_note_array, this will remove it, and the corresponding velocity will be removed from the current_velocity_array 
//and then it will shift the rest of the values down to fill in the gap.
void remove_note(uint8_t note){
  uint8_t note_position = check_note(note);
  //if the note is in the note array, remove it and shift down any other notes.
  if(note_position != NO_NOTE){
    for(int i=note_position; i<num_current_notes; i++){
      current_note_array[i] = current_note_array[i+1];
      current_velocity_array[i] = current_velocity_array[i+1];
      current_channel_array[i] = current_channel_array[i+1];
    }
    num_current_notes--;
  }
  else{
    //Do nothing
  }
  //need to get the head to immediately stop playing the note as well:
  for(int i=0; i<NUM_OMIDITONES; i++){
    if(oms[i].currently_playing_note() == note){
      oms[i].note_off(note);
    }
  }

  //and finally change the head_order_array to match the pending_head_order_array when all notes are off
  if(num_current_notes <= 0){
    for(int i=0; i<NUM_OMIDITONES; i++){
      head_order_array[i] = pending_head_order_array[i];
    }
  }
}

//this function moves the head in question to the end of the head_order_array, and shuffles the remaining heads down into its place.
void pending_head_order_to_end(uint8_t head_number){
  //don't need to iterate to the last one, because if the head_number is in the last position already, we're good
  for(int i=0; i<NUM_OMIDITONES-1; i++){
    if(pending_head_order_array[i] == head_number){
      //set the current position to the next head in line
      pending_head_order_array[i] = pending_head_order_array[i+1];
      //and move the head in question down the line until it's in the last place
      pending_head_order_array[i+1] = head_number;
    }
  }
}

//this is a function to send commands to the oMIDItones connected to this controller based on the note_status_array and current_pitch_shift
void update_oMIDItones(){
  //First set the pitch for all the running oMIDItones.
  //this can just iterate through all of them in order, since it doesn't require load balancing like note_on.
  if(pitch_has_changed){
    for(int i=0; i<NUM_OMIDITONES; i++){
      for(int channel=0; channel<NUM_MIDI_CHANNELS; channel++){
        oms[i].set_pitch_shift(current_pitch_shift[channel], channel);
      }
    }
    pitch_has_changed = false;
  }

  //only update if a note has changed state:
  if(note_has_changed){
    //we need a couple variables that we can use to iterate through all the heads in the right order.
    bool head_state[NUM_OMIDITONES];
    for(int i=0; i<NUM_OMIDITONES; i++){
      //mark all heads as available initially
      head_state[i] = AVAILABLE;
    }
    //only do this if there are notes to be played.
    if(num_current_notes > 0){
      //iterate through the active notes so that we can see if any heads are available to play them.
      //we need to go from the end of the list backwards.
      for(int n=num_current_notes-1; n>=0; n--){
        //iterate through the array of unassigned heads.
        for(int h=0; h<NUM_OMIDITONES; h++){
          //only check the notes if the head hasn't been assigned.
          if(head_state[h] == AVAILABLE){
            if(oms[head_order_array[h]].can_play_note(current_note_array[n])){
              //tell the head to play the note.
              oms[head_order_array[h]].note_on(current_note_array[n], current_velocity_array[n], current_channel_array[n]);
              //trigger a lighting update on the head
              if(note_trigger_is_enabled){
                oms[head_order_array[h]].animation->trigger_event(note_trigger_type[head_order_array[h]]);
              }
              //remove the head from the unassigned heads array
              head_state[h] = NOT_AVAILABLE;
              //move the head to the end of the head_order_array
              pending_head_order_to_end(head_order_array[h]);
              #ifdef DEBUG
                Serial.print("Head ");
                Serial.print(head_order_array[h]);
                Serial.print(" is playing note ");
                Serial.print(current_note_array[n]);
                Serial.println(".");
              #endif
              //break the head for loop so no other heads try to play the same note:
              break;
            } //can_play_note
          } //head_state == AVAILABLE
        } //head for loop
      } //note for loop
      //reset the state variable once everything has been tried/assigned.
      note_has_changed = false;
    } //num_current_notes > 0
  } //note has changed

  //Finally, this will update the oMIDItone objects:
  for(int i=0; i<NUM_OMIDITONES; i++){
    oms[i].update();
  }
}

void setup(){
  #ifdef DEBUG
    Serial.begin(9600);
    delay(5000); //wait for serial
    Serial.println("Welcome to oMIDItone.");
    Serial.println("Beginning initialization - this may take several minutes...");
  #endif

  //initialize the head order array and pending_head_order_array to be 0-5 in order:
  for(int i=0; i<NUM_OMIDITONES; i++){
    head_order_array[i] = i;
    pending_head_order_array[i] = i;
  }

  //set up the lighting controller with the animations
  lc.init();
  for(int i=0; i<NUM_OMIDITONES; i++){
    lc.add_animation(oms[i].animation);
  }
  lc.update();

  //init the om objects - This is going to take a while - like several minutes:
  for(int i=0; i<NUM_OMIDITONES; i++){
    //init the head
    oms[i].init();
  }

  //initialize the pitch shift to the default value of no pitch shift:
  for(int i=0; i<NUM_MIDI_CHANNELS; i++){
    current_pitch_shift[i] = CENTER_PITCH_SHIFT;
  }

  //call out MIDI functions
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandlePitchChange(OnPitchChange);
  usbMIDI.setHandleControlChange(OnControlChange);

  //Hardware MIDI init.
  MIDI.begin(MIDI_CHANNEL_OMNI);

  #ifdef DEBUG
    Serial.println("Init Complete, awaiting MIDI input.");
  #endif
}

void loop(){
  
  //This will read for MIDI notes and start/stop the notes based on MIDI input:
  usbMIDI.read();
  //This will check the hardware MIDI channels and start/stop the notes based on MIDI input.
  read_hardware_MIDI();
  //This will decide which oMIDItone to use for which note, and update the status.
  update_oMIDItones();
  //this will update all lighting functions on a regular basis
  //update_lighting(); //old - delete once lc is working
  if(lighting_is_enabled){
    int lighting_data_was_sent = lc.update();
    if(lighting_data_was_sent == LC_STRIP_WRITTEN){
      for(int h=0; h<NUM_OMIDITONES; h++){
        oms[h].reset_pitch_correction();
      }
    }
  }
}