/*
This is the oMIDItone library, to send commands to a pair of MCP4151 50k digital 
pots to change the tone on an otamatone. 
It also has a pin for turning the sound on and off, and a pin for 
analog readings to correct the frequency being played.
*/

#include <Arduino.h>
#include <SPI.h>
#include "oMIDItone.h"

// constructor function
oMIDItone::oMIDItone(uint16_t relay, uint16_t cs1, uint16_t cs2, uint16_t feedback){
  //Declare default values for variables:
  check_note_value = false;
  current_note = NO_NOTE;
  min_note = 0;
  max_note = NUM_MIDI_NOTES;
  freq_reading_index = 0;
  current_freq = 0;
  last_analog_read = 1024;
  current_pitch_shift = CENTER_PITCH_SHIFT;
  current_resistance = 0;

  // set pin variables based on constructor inputs:
  relay_pin = relay;
  cs1_pin = cs1;
  cs2_pin = cs2;
  analog_feedback_pin = feedback;
}

// this will init the pin modes and set up Serial if it's not already running.
void oMIDItone::init(){
  //start a timer for tracking rising edges.
  last_rising_edge = 0;

  #ifdef DEBUG
    //init Serial to allow for manual note setting in debug mode:
    Serial.begin(9600);
    delay(5000); //wait for serial
    Serial.println("Debug is enabled, printing note values.");
  #endif

  //set pin modes
  pinMode(cs1_pin, OUTPUT);
  pinMode(cs2_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(analog_feedback_pin, INPUT);

  //turn off relay and all CS pins
  digitalWrite(cs1_pin, HIGH);
  digitalWrite(cs2_pin, HIGH);
  digitalWrite(relay_pin, LOW);

  //init SPI  
  SPI.begin();

  //Play startup tone and save initial resistance to note values.
  bool startup_succeeded = startup_test();
  if(!startup_succeeded){
    #ifdef DEBUG
      Serial.print("Init for oMIDItone on relay pin ");
      Serial.print(relay_pin);
      Serial.println(" failed.");
    #endif
  }
}

//This should be called during the loop, and it will update the note frequencies 
//and play notes as needed based on what current_note is set to.
void oMIDItone::update(){
  //if no note is set, disable the relay and stop checking the current frequency.
  if(!can_play_note(current_note)){
    //turn off the noise.
    digitalWrite(relay_pin, LOW);
  }
  //Otherwise, play the note.
  else{
    //turn on the noise.
    if(note_start_time > NOTE_WAIT_TIME){
      digitalWrite(relay_pin, HIGH);
    }
    //continuously measure the current frequency and adjust the resistance as needed.
    measure_frequency();
    adjust_frequency();
    //and set the resistance to a jittered value based on the adjusted current_resistance.
    set_jitter_resistance(current_resistance, JITTER);
  }
}

//This returns the note number that is currently playing or NO_NOTE if no note is currently playing.
uint16_t oMIDItone::currently_playing_note(){
  return current_note;
}

//this returns the current pitch shift value.
uint16_t oMIDItone::currently_playing_pitch_shift(){
  return current_pitch_shift;
}

//This will return true if init was successful AND there is no current note playing.
bool oMIDItone::is_running(){
  if(had_successful_init){
    return true;
  }
  else{
    return false;
  }
}

//This will check if a frequency can be played by an initialized oMIDItone object. freq in us.
bool oMIDItone::can_play_note(uint16_t note){
  if( !had_successful_init ){
    return false;
  }
  else if( (note <= max_note) && (note >= min_note) ){
    return true;
  }
  else{
    return false;
  }
}

//This will tell the oMIDItone to play the note. The note will continue to play until changed or until set to off.
void oMIDItone::note_on(uint16_t note){
  if(can_play_note(note)){
    //set the current_note:
    note_start_time = 0;
    current_note = note;
    //set the averaging function to the new frequency in anticipation of the change:
    for(int i=0; i<NUM_FREQ_READINGS; i++){
      recent_freqs[i] = pitch_adjusted_frequency(current_note, current_pitch_shift);
    }
    //set the current_resistance to a value that was previously measured as close to the desired note's frequency.
    current_resistance = frequency_to_resistance(midi_freqs[current_note]);
  }
  else{
    current_note = NO_NOTE;
  }
}

//this will set the pitch shift value. This will apply to any notes played on the oMIDItone.
void oMIDItone::set_pitch_shift(int16_t pitch_shift_value){
  current_pitch_shift = pitch_shift_value;
}

//This will set the oMIDItone to stop playing all notes.
void oMIDItone::note_off(uint16_t note){
  if(current_note == note){
    current_note = NO_NOTE;
  }
}

//this will set the CS_pin digital pot's wiper to a value based on byte 1 and byte 2
void oMIDItone::set_pot(uint16_t CS_pin, uint16_t command_byte){
  digitalWrite(CS_pin, LOW); //select chip
  uint16_t byte_high = command_byte >> 8;
  uint16_t byte_low = command_byte & 0xff;
  SPI.transfer(byte_high); //send command first
  SPI.transfer(byte_low); //send value second
  digitalWrite(CS_pin, HIGH); //de-select chip when done
}

//this will take a uint16_t number and set the total resistance value to between 0 and 512 on the board.
void oMIDItone::set_resistance(uint16_t resistance){
  if(resistance >= 0 && resistance <= NUM_RESISTANCE_STEPS/2){
    set_pot(cs1_pin, resistance);
    set_pot(cs2_pin, 0);
  }
  else if(resistance > NUM_RESISTANCE_STEPS/2 && resistance <= NUM_RESISTANCE_STEPS){
    set_pot(cs1_pin, NUM_RESISTANCE_STEPS/2);
    set_pot(cs2_pin, resistance-NUM_RESISTANCE_STEPS/2);
  }
  else if(resistance < 0){
    set_pot(cs1_pin, 0);
    set_pot(cs2_pin, 0);
  }
  else{
    set_pot(cs1_pin, NUM_RESISTANCE_STEPS/2);
    set_pot(cs2_pin, NUM_RESISTANCE_STEPS/2);
  }
}

//This will constantly read the analog input and return true when it detects a rising edge signal.
//It also updates the current and last rising edge time values.
bool oMIDItone::is_rising_edge(){
  uint16_t current_analog_read = analogRead(analog_feedback_pin);
  if(last_rising_edge > MIN_TIME_BETWEEN_RISING_EDGE_MEASUREMENTS){
    if( current_analog_read > RISING_EDGE_THRESHOLD && last_analog_read < RISING_EDGE_THRESHOLD){
      last_analog_read = current_analog_read;
      return true;
    }
    else{
      last_analog_read = current_analog_read;
      return false;
    }
  }
  else{
    return false;
  }
}

//a simple averaging function:
uint32_t oMIDItone::average(uint32_t * array, uint16_t num_elements){
  uint32_t total = 0;
  for(uint16_t i=0; i<num_elements; i++){
    total = total + array[i];
  }
  return total/num_elements;
}

//this introduces jittered resistance settings, and should be called every loop to keep the jitter working:
void oMIDItone::set_jitter_resistance(uint16_t resistance, uint16_t jitter){
  uint16_t current_jitter = random(jitter);
  uint16_t positive = random(1);
  if(positive){
    set_resistance(resistance + current_jitter);
  }
  else{
    set_resistance(resistance - current_jitter);
  }
}

//this function will find the first resistance value that was measured as being very near the desired frequency.
uint16_t oMIDItone::frequency_to_resistance(uint16_t frequency){
  //iterate through the measured_freqs array and check for when the frequency has gone over the desired frequency by one step.
  for(int i=JITTER+2; i<NUM_RESISTANCE_STEPS-JITTER-2; i++){
    //If the frequency if higher than the current note (less us) then set the midi_to_resistance value and increment the note:
    if(measured_freqs[i] < frequency){
      return i;
    }
  }
  //if none of the above matched, return the max value.
  return NUM_RESISTANCE_STEPS-JITTER;
}

//This adjusts a frequency to a pitch-shifted value from the base note. 
//Pitch shift can move uo to two MIDI notes away depending on value of the pitch shift variable.
uint32_t oMIDItone::pitch_adjusted_frequency(uint16_t note, uint16_t pitch_shift){
  //First the trivial case of CENTER_PITCH_SHIFT:
  if(pitch_shift == CENTER_PITCH_SHIFT){
    return midi_freqs[note];
  }
  else if(pitch_shift < CENTER_PITCH_SHIFT){
    //adjust the pitch down based on the difference between pitch_shift and CENTER_PITCH_SHIFT
    uint16_t adjustment_factor = abs(pitch_shift)/8192;
    float frequency_multiplier = 2^(-adjustment_factor/12);
    uint32_t adjusted_frequency = midi_freqs[note]*frequency_multiplier;
    return adjusted_frequency;
    
  }
  else if(pitch_shift > CENTER_PITCH_SHIFT){
    //adjust the pitch down based on the difference between pitch_shift and CENTER_PITCH_SHIFT
    uint16_t adjustment_factor = abs(pitch_shift)/8192;
    float frequency_multiplier = 2^(adjustment_factor/12);
    uint32_t adjusted_frequency = midi_freqs[note]*frequency_multiplier;
    return adjusted_frequency;
    
  }
  else{
    //this shouldn't happen, but return the default if nothing else worked for some reason.
    return midi_freqs[current_note];
  }
}

//this takes the frequency averaging code and puts it into a function to clean up the update function:
void oMIDItone::measure_frequency(){
  //this first bit is calculating the average continuously and storing it in current_freq
  if(is_rising_edge()){
    recent_freqs[freq_reading_index] = last_rising_edge;
    last_rising_edge = 0;
    freq_reading_index++;
  }
  //take more readings on init to get more accurate note values:
  if(freq_reading_index >= NUM_FREQ_READINGS && current_note != NO_NOTE){
    //calculate a new average frequency
    current_freq = average(recent_freqs, NUM_FREQ_READINGS);
    //and reset the counter
    freq_reading_index = 0;
    //also update the measured_freqs array to be correct for the current resistasnce.
    measured_freqs[current_resistance] = current_freq;
    for(int i=0; i<NUM_FREQ_READINGS; i++){
      Serial.print(i);
      Serial.print(": ");
      Serial.print(recent_freqs[i]);
      Serial.print(": ");
    }
    Serial.println();
  }
}

//This is a function that will change the current_resistance to a different value if it is too far off from the current_frequency.
void oMIDItone::adjust_frequency(){
  //the next bit will adjust the current jittered resistance value up or down depending on how close the current_freq is to the desired frequency of the current_note, and store it in the midi_to_resistance array
  if(last_adjust_time > TIME_BETWEEN_FREQUENCY_CORRECTIONS){
    //this determines the allowable range that the frequency can be in to avoid triggering a retune:
    
    //First adjust for pitch shift if any:
    uint32_t current_desired_freq = pitch_adjusted_frequency(current_note, current_pitch_shift);
    
    //this is the range of frequencies acceptable for the current pitch-shifted note being played.
    uint32_t max_allowable_freq = current_desired_freq*(100-ALLOWABLE_NOTE_ERROR)/100;
    uint32_t min_allowable_freq = current_desired_freq*(100+ALLOWABLE_NOTE_ERROR)/100;
    /*
    Serial.print("Current Frequency: ");
    Serial.println(current_freq);
    Serial.print("Max Frequency: ");
    Serial.println(max_allowable_freq);
    Serial.print("Min Frequency: ");
    Serial.println(min_allowable_freq);
    Serial.print("Current resistance: ");
    Serial.println(current_resistance);
    Serial.println();
    */
    if(current_freq >= max_allowable_freq && current_freq <= min_allowable_freq){
      //DOn't adjust anything.
    }
    else if(current_freq < max_allowable_freq){
      current_resistance--;
      if(current_resistance < JITTER){
        //prevent the value from overflowing:
        current_resistance = JITTER;
        //if it's bottoming out, increase the min note.
        min_note++;
        #ifdef DEBUG
          Serial.print("Note ");
          Serial.print(current_note);
          Serial.print("is too low for reliable use. min_note increased to ");
          Serial.println(min_note);
        #endif
      }
      last_adjust_time = 0;
      #ifdef DEBUG
        Serial.print("Note ");
        Serial.print(current_note);
        Serial.print(" resistance adjusted to ");
        Serial.println(current_resistance);
      #endif
    }
    else if(current_freq > min_allowable_freq){
      current_resistance++;
      if(current_resistance > (512-JITTER)){
        //prevent the value from overflowing:
        current_resistance = 512-JITTER;
        //if it's topping out, reduce the max note:
        max_note--;
        #ifdef DEBUG
          Serial.print("Note ");
          Serial.print(current_note);
          Serial.print("is too high for reliable use. max_note decreased to ");
          Serial.println(max_note);
        #endif
      }
      last_adjust_time = 0;
      #ifdef DEBUG
        Serial.print("Note ");
        Serial.print(current_note);
        Serial.print(" resistance adjusted to ");
        Serial.println(current_resistance);
      #endif
    }
  }
}

//This will play from 0 resistance value to 512 resistance value and note which resistances
//correspond to which notes in the note matrix:
bool oMIDItone::startup_test(){
  #ifdef DEBUG
    Serial.println("Startup Test Measured Frequencies Below:");
  #endif

  //Turn on the relay to generate sounds:
  digitalWrite(relay_pin, HIGH);
  //Start counting microseconds since a rising edge to calculate frequencies:
  last_rising_edge = 0;
  //iterate through all frequencies with jitter to determine the average frequency for that resistance.
  for(uint16_t resistance = JITTER; resistance <= NUM_RESISTANCE_STEPS-JITTER; resistance++){
    current_resistance = resistance;
    set_jitter_resistance(current_resistance, JITTER);
    
    uint32_t startup_start_time = millis();
    //measure the frequency NUM_FREQ_READINGS times:
    //wait for at least one rising edge before beginning:
    while(1){
      if(is_rising_edge()){
        break;
      }
      //If it doesn't detect a rising edge in time, return false so the rest of the controller can continue to function.
      if((startup_start_time + TIME_TO_WAIT_FOR_INIT) < millis()){
        digitalWrite(relay_pin, LOW);
        current_note = NO_NOTE;
        return false;
      }
    }
    freq_reading_index = 0;
    while(1){
      set_jitter_resistance(current_resistance, JITTER);
      if(is_rising_edge()){
        recent_freqs[freq_reading_index] = last_rising_edge;
        last_rising_edge = 0;
        freq_reading_index++;
      }
      if(freq_reading_index >= NUM_FREQ_READINGS){
        measured_freqs[current_resistance] = average(recent_freqs, NUM_FREQ_READINGS);
        #ifdef DEBUG
          Serial.print("Frequency at Resistance ");
          Serial.print(current_resistance);
          Serial.print(" is ");
          Serial.println(measured_freqs[current_resistance]);
        #endif
        break;
      }
      //If it doesn't detect a rising edge in time, return false so the rest of the controller can continue to function.
      if((startup_start_time + TIME_TO_WAIT_FOR_INIT) < millis()){
        digitalWrite(relay_pin, LOW);
        current_note = NO_NOTE;
        return false;
      }
    }
  }
  digitalWrite(relay_pin, LOW);

  //Set the max_note and min_note variables based on the frequencies measured:
  uint32_t max_measured_freq = midi_freqs[0]; //set default to longest time in us, and adjust below:
  uint32_t min_measured_freq = midi_freqs[NUM_MIDI_NOTES-1]; //set default to shortest time in us, and adjust below:
  for(uint16_t i = JITTER; i <= NUM_RESISTANCE_STEPS-JITTER; i++){
    if(measured_freqs[i] < max_measured_freq){
      max_measured_freq = measured_freqs[i];
    }
    if(measured_freqs[i] > min_measured_freq){
      min_measured_freq = measured_freqs[i];
    }
  }
  #ifdef DEBUG
    Serial.print("Min Measured Freq in us: ");
    Serial.println(min_measured_freq);
    Serial.print("Max Measured Freq in us: ");
    Serial.println(max_measured_freq);
  #endif

  //set min_note:
  for(uint16_t i=0; i<NUM_MIDI_NOTES; i++){
    if(midi_freqs[i] < min_measured_freq){
      //should go to the next note to leave room for variance
      min_note = i+1;
      break;
    }
  }

  //set max_note:
  for(uint16_t i=NUM_MIDI_NOTES; i>0; i--){
    if(midi_freqs[i] > max_measured_freq){
      //should go to the next note to leave room for variance
      max_note = i-1;
      break;
    }
  }

  #ifdef DEBUG
    Serial.print("Initial Min MIDI note is: ");
    Serial.println(min_note);
    Serial.print("Initial Max MIDI note is: ");
    Serial.println(max_note);
  #endif

  //if it makes it here the init was successful and we can move on to testing the available note range
  had_successful_init = true;

  //now I will play each of the MIDI notes until they stabilize on a resistance, as the initial readings all seem to come in higher than they should be.
  for(int i=min_note; i<=max_note; i++){
    //reset stabilization time
    last_stabilize_time = 0;
    //set the correct note to on, and update for as long as needed.
    note_on(i);
    while(last_stabilize_time < STABILIZATION_TIME){
      update();
    }
    #ifdef DEBUG
      Serial.print("MIDI note ");
      Serial.print(i);
      Serial.println(" stabilized.");
    #endif
    note_off(current_note);
  }

  #ifdef DEBUG
    Serial.print("Corrected Min MIDI note is: ");
    Serial.println(min_note);
    Serial.print("Corrected Max MIDI note is: ");
    Serial.println(max_note);
  #endif

  return true;
}
