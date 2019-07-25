/*
Author: kiyoshigawa
Start Date: 2018-03-18
Description:
Took an Otamatone, and replaced the pressure-sensitive variable resistor strip with some MCP4151 50kohm variable pots (2 in series) with a 220Ohm fixed resistor.
Then used a teensy LC MIDI library to control the resistance from the chips over SPI, and connected it to the otamatone, making a oMIDItone.

Update: 2019-03-23: converted the code into a class, rewrote everything to work with the class, can run up to 6 on a Teensy 3.2, only one on a Teensy LC due to lack of RAM.
Update: 2019-03-27: Got all the features working, should be a fully functional MIDI instrument, pending testing of analog read speeds keeping up with higher frequency notes when more than 2 oMIDItones are running. I only have 2 for testing atm, need to buy more.

*/

#include "oMIDItone.h"
#include <MIDI.h>

 //number of oMIDItones on this controller.
#define NUM_OMIDITONES 6

 //comment this to disable serial functions for testing notes.
#define DEBUG

 //defines to make the head selection code more readable:
#define AVAILABLE true
#define NOT_AVAILABLE false

 // Pin Definitions
int om1_se_pin = 15;
int om1_sd_pin = 26;
int om1_cs1_pin = 17;
int om1_cs2_pin = 16;
int om1_analog_feedback_pin = A10;
int om2_se_pin = 33;
int om2_sd_pin = 25;
int om2_cs1_pin = 22;
int om2_cs2_pin = 21;
int om2_analog_feedback_pin = A11;
int om3_se_pin = 24;
int om3_sd_pin = 30;
int om3_cs1_pin = 3;
int om3_cs2_pin = 23;
int om3_analog_feedback_pin = A13;
int om4_se_pin = 10;
int om4_sd_pin = 29;
int om4_cs1_pin = 9;
int om4_cs2_pin = 8;
int om4_analog_feedback_pin = A12;
int om5_se_pin = 32;
int om5_sd_pin = 28;
int om5_cs1_pin = 7;
int om5_cs2_pin = 6;
int om5_analog_feedback_pin = A14;
int om6_se_pin = 31;
int om6_sd_pin = 27;
int om6_cs1_pin = 5;
int om6_cs2_pin = 4;
int om6_analog_feedback_pin = A0;
 //Using SPI0 on board, MOSI0 = 11, MISO0 = 12, and SCK0 = 13, which will blink the LED as it sends.

 //declare the oMIDItone objects:
oMIDItone oms[] = {oMIDItone(om1_se_pin, om1_sd_pin, om1_cs1_pin, om1_cs2_pin, om1_analog_feedback_pin),
oMIDItone(om2_se_pin, om2_sd_pin, om2_cs1_pin, om2_cs2_pin, om2_analog_feedback_pin),
oMIDItone(om3_se_pin, om3_sd_pin, om3_cs1_pin, om3_cs2_pin, om3_analog_feedback_pin),
oMIDItone(om4_se_pin, om4_sd_pin, om4_cs1_pin, om4_cs2_pin, om4_analog_feedback_pin),
oMIDItone(om5_se_pin, om5_sd_pin, om5_cs1_pin, om5_cs2_pin, om5_analog_feedback_pin),
oMIDItone(om6_se_pin, om6_sd_pin, om6_cs1_pin, om6_cs2_pin, om6_analog_feedback_pin)
};

 //this is a variable storing the pitch shift value last received over MIDI:
 //Note the teensy library uses a signed pitch shift centered on 0 instead of the MIDI standard of centering on 8192.
int16_t current_pitch_shift = CENTER_PITCH_SHIFT;

 //this is an array that tracks whether a note is currently on or off.
uint8_t current_note_array[NUM_MIDI_NOTES];

 //this will note the velocity that corresponds to the note array above.
uint8_t current_velocity_array[NUM_MIDI_NOTES];

 //this tracks how many notes are in the array.
uint8_t num_current_notes = 0;

 //this is a bool ot let the controller know when it needs to reassign notes to the heads.
bool note_has_changed = false;

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

 //update the note_status_array to set the requested note as on.
void OnNoteOn(byte channel, byte note, byte velocity){
  if(velocity > 0){
    add_note(note, velocity);
  }
  else{
    remove_note(note);
  }
  note_has_changed = true;
  #ifdef DEBUG
    print_current_note_array();
  #endif
}

 //This updated the note_status_array to set the requested note to off:
void OnNoteOff(byte channel, byte note, byte velocity){
  remove_note(note);
  note_has_changed = true;
  #ifdef DEBUG
    print_current_note_array();
  #endif
}

 //this sets the pitch_shift value to be used by the oMIDItones:
void OnPitchChange(byte channel, int pitch){
  current_pitch_shift = pitch;
  #ifdef DEBUG
    Serial.print("Pitch Shift set to ");
    Serial.print(current_pitch_shift);
    Serial.println(".");
  #endif
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
void add_note(uint8_t note, uint8_t velocity){
  uint8_t note_position = check_note(note);
   //if the note isn't already in the array, put it at the end of the array.
  if(note_position == NO_NOTE){
    current_note_array[num_current_notes] = note;
    current_velocity_array[num_current_notes] = velocity;
    num_current_notes++;
  }
   //if it is in the array, put the note at the end and shift everything back down to where the note used to be.
  else{
    for(int i=note_position; i<num_current_notes; i++){
      current_note_array[i] = current_note_array[i+1];
      current_velocity_array[i] = current_velocity_array[i+1];
    }
    current_note_array[num_current_notes-1] = note;
    current_velocity_array[num_current_notes-1] = velocity;
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
}

 //this is a function to send commands to the oMIDItones connected to this controller based on the note_status_array and current_pitch_shift
void update_oMIDItones(){
   //First set the pitch for all the running oMIDItones.
   //this can just iterate through all of them in order, since it doesn't require load balancing like note_on.
  for(int i=0; i<NUM_OMIDITONES; i++){
    oms[i].set_pitch_shift(current_pitch_shift);
  }

   //only update if a note has changed state:
  if(note_has_changed){
    //we need a couple variables that we can use to iterate through all the heads in the right order.
    bool head_state[NUM_OMIDITONES];
    for(int i=0; i<NUM_OMIDITONES; i++){
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
            if(oms[h].can_play_note(current_note_array[n])){
              //tell the head to play the note.
              oms[h].note_on(current_note_array[n]);
              //remove the head from the unassigned heads array
              head_state[h] = NOT_AVAILABLE;
              #ifdef DEBUG
                Serial.print("Head ");
                Serial.print(h);
                Serial.print(" is playing note ");
                Serial.print(current_note_array[n]);
                Serial.println(".");
              #endif
              //break the head for loop so no other heads try to play the same note:
              break;
            } //can_play_note
          } //head_state == AVAILABLE
        } //head for loop
        #ifdef DEBUG
          Serial.print("No head found to play note ");
          Serial.print(current_note_array[n]);
          Serial.println(".");
        #endif
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

 //This will read the hardware MIDI and set or remove notes as needed.
void read_hardware_MIDI(){
  int type, note, velocity, channel, p1, p2, d1, d2;
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
           //Set the note on as usual:
          add_note(note, velocity);
          note_has_changed = true;
          #ifdef DEBUG
            print_current_note_array();
          #endif
        } 
        else {
           //Set the note off.
          remove_note(note);
          note_has_changed = true;
        #ifdef DEBUG
          print_current_note_array();
        #endif
        }
        break;
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
         //Set the note off as usual.
        remove_note(note);
        note_has_changed = true;
        #ifdef DEBUG
          print_current_note_array();
        #endif
        break;
      case midi::PitchBend:
        p1 = MIDI.getData1();
        p2 = MIDI.getData2();
         //shift the bits so it's a single number from -8192 to 8192.
        current_pitch_shift = (p2<<7) + p1 - 8192;
        #ifdef DEBUG
          Serial.print("Pitch Shift set to ");
          Serial.print(current_pitch_shift);
          Serial.println(".");
        #endif
        break;
      default:
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
    }
  } 
}

void setup(){
   //init the om objects - This is going to take a while - like several minutes:
  for(int i=0; i<NUM_OMIDITONES; i++){
    oms[i].init();
  }

   //call out MIDI functions
  usbMIDI.setHandleNoteOff(OnNoteOff);
  usbMIDI.setHandleNoteOn(OnNoteOn);
  usbMIDI.setHandlePitchChange(OnPitchChange);

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
}
