/*
This is the oMIDItone library, to send commands to a pair of MCP4151 50k digital
pots to change the tone on an otamatone.
It also has a pin for turning the sound on and off, and a pin for
analog readings to correct the frequency being played.

I should probably implement channel controls in the future.

*/

#include <Arduino.h>
#include <SPI.h>
#include "oMIDItone.h"
#include "ADC-master/ADC.h"

 //constructor function
oMIDItone::oMIDItone(uint16_t signal_enable_optoisolator, uint16_t speaker_disable_optoisolator, uint16_t cs1, uint16_t cs2, uint16_t feedback){
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

   //set pin variables based on constructor inputs:
  signal_enable_optoisolator_pin = signal_enable_optoisolator;
  speaker_disable_optoisolator_pin = speaker_disable_optoisolator;
  cs1_pin = cs1;
  cs2_pin = cs2;
  analog_feedback_pin = feedback;

   //set up an instanced ADC object for use in the code.
  ADC *adc = new ADC();
}

 //this will init the pin modes and set up Serial if it's not already running.
void oMIDItone::init(){
   //start a timer for tracking rising edges.
  last_rising_edge = 0;

  #ifdef OMIDITONE_DEBUG
     //init Serial to allow for manual note setting in debug mode:
    Serial.begin(9600);
    delay(5000); //wait for serial
    Serial.print("Debug is enabled for oMIDItone on relay pin ");
    Serial.print(signal_enable_optoisolator_pin);
    Serial.println(".");
  #endif

   //set pin modes
  pinMode(cs1_pin, OUTPUT);
  pinMode(cs2_pin, OUTPUT);
  pinMode(signal_enable_optoisolator_pin, OUTPUT);
  pinMode(speaker_disable_optoisolator_pin, OUTPUT);
  pinMode(analog_feedback_pin, INPUT);

   //set up ADC:
  adc->setAveraging(0);
  adc->setResolution(8);
  adc->setConversionSpeed(ADC_CONVERSION_SPEED::VERY_HIGH_SPEED);
  adc->setSamplingSpeed(ADC_SAMPLING_SPEED::VERY_HIGH_SPEED);

   //turn off relay and all CS pins
  digitalWrite(cs1_pin, HIGH);
  digitalWrite(cs2_pin, HIGH);
  digitalWrite(signal_enable_optoisolator_pin, LOW);
  digitalWrite(speaker_disable_optoisolator_pin, HIGH); //defaults to the speakers being enabled. The startup test will disable them, and then re-enable them when complete.

   //init SPI
  SPI.begin();

   //Play startup tone and save initial resistance to note values.
  bool startup_succeeded = startup_test();
  if(!startup_succeeded){
    #ifdef OMIDITONE_DEBUG
      Serial.print("Init for oMIDItone on relay pin ");
      Serial.print(signal_enable_optoisolator_pin);
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
    digitalWrite(signal_enable_optoisolator_pin, LOW);
  }
   //Otherwise, play the note.
  else{
     //turn on the noise.
    if(note_start_time > NOTE_WAIT_TIME){
      digitalWrite(signal_enable_optoisolator_pin, HIGH);
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
    update();
  }
  else{
    current_note = NO_NOTE;
    update();
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
    update();
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

 //this will take a uint16_t number and set the total resistance value to between 0 and 768 on the board.
 //Lots of hard coded values here, should probably fix later, but oh well.
void oMIDItone::set_resistance(uint16_t resistance){
   //The case where we need to oscillate the 50k pot to increase resolution:
  if(resistance >= 0 && resistance <= 512){
     //this is divided by 2, so it returns a number between 0 and 256.
    set_pot(cs1_pin, resistance/2);
     //this sets to either 0 or 1 depending on the modulus with 2.
    if(resistance % 2){
      set_pot(cs2_pin, 0);
    }
    else{
      set_pot(cs2_pin, 1);
    }
    
  }
   //The case where the steps are above 512 means to set the 100k pot to 256 and the other to the current resistance value - 512.
  else if(resistance > 512 && resistance <= NUM_RESISTANCE_STEPS){
    set_pot(cs1_pin, 256);
    set_pot(cs2_pin, resistance-512);
  }
  else if(resistance < 0){
    set_pot(cs1_pin, 0);
    set_pot(cs2_pin, 0);
  }
  else{
    set_pot(cs1_pin, 256);
    set_pot(cs2_pin, 256);
  }
}

 //This will constantly read the analog input and return true when it detects a rising edge signal.
 //It also updates the current and last rising edge time values.
bool oMIDItone::is_rising_edge(){
  uint16_t current_analog_read = adc->analogRead(analog_feedback_pin);
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

    if(current_freq >= max_allowable_freq && current_freq <= min_allowable_freq){
       //DOn't adjust anything.
    }
    else if(current_freq < max_allowable_freq){
      current_resistance--;
       //Only correct if the resistance is too low and the pitch is not being shifted downards.
      if((current_resistance < JITTER) && (current_pitch_shift >= 0)){
         //prevent the value from overflowing:
        current_resistance = JITTER;
         //if it's bottoming out, increase the min note.
        min_note++;
        #ifdef OMIDITONE_DEBUG
          Serial.print("Note ");
          Serial.print(current_note);
          Serial.print("bottomed out. min_note increased to ");
          Serial.print(min_note);
          Serial.print(" on oMIDItone on relay pin");
          Serial.println(signal_enable_optoisolator_pin);
        #endif
      }
      last_adjust_time = 0;
      #ifdef OMIDITONE_DEBUG
        /* reducing spam re-enable if needed.
        Serial.print("Note ");
        Serial.print(current_note);
        Serial.print(" resistance adjusted to ");
        Serial.println(current_resistance);
        */
      #endif
    }
    else if(current_freq > min_allowable_freq){
      current_resistance++;
       //Only correct if the resistance is too high and the pitch is not being shifted upwards.
      if((current_resistance > (NUM_RESISTANCE_STEPS-JITTER)) && (current_pitch_shift <= 0)){
         //prevent the value from overflowing:
        current_resistance = NUM_RESISTANCE_STEPS-JITTER;
         //if it's topping out, reduce the max note:
        max_note--;
        #ifdef OMIDITONE_DEBUG
          Serial.print("Note ");
          Serial.print(current_note);
          Serial.print("is too high for reliable use. max_note decreased to ");
          Serial.print(max_note);
          Serial.print("on oMIDItone on relay pin");
          Serial.println(signal_enable_optoisolator_pin);
        #endif
      }
      last_adjust_time = 0;
      #ifdef OMIDITONE_DEBUG
        /* reducing spam
        Serial.print("Note ");
        Serial.print(current_note);
        Serial.print(" resistance adjusted to ");
        Serial.println(current_resistance);
        */
      #endif
    }
  }
}

 //This will play from 0 resistance value to 512 resistance value and note which resistances
 //correspond to which notes in the note matrix:
bool oMIDItone::startup_test(){

  //the first part is all manual control of the resistance value and the signal_enable_optoisolator_pin.

  #ifdef OMIDITONE_DEBUG
    Serial.println("Startup Test Debug Beginning:");
  #endif

   //Turn on the relay to generate sounds:
  digitalWrite(signal_enable_optoisolator_pin, HIGH);

   //Turn off the speaker output to keep your sanity:
  digitalWrite(speaker_disable_optoisolator_pin, LOW);

   //Start counting microseconds since a rising edge to calculate frequencies:
  last_stabilize_time = 0;

   //run a stabilization note for a minute.
  while(last_stabilize_time < TIME_TO_WAIT_FOR_STARTUP_TEST_SOUND){
    set_jitter_resistance(JITTER, JITTER);
  }

   //Confirm the first rising edge before the timeout to make sure we are getting good data.
  last_rising_edge = 0;
  int num_rising_edges = 0;
  uint32_t startup_start_time = millis();

  while(1){
    if(is_rising_edge()){
      last_rising_edge = 0;
      last_frequency_measurement = 0;
      break;
    }
      //If it doesn't detect a first rising edge in time, return false so the rest of the controller can continue to function.
    if((startup_start_time + TIME_TO_WAIT_FOR_INIT) < millis()){
      digitalWrite(signal_enable_optoisolator_pin, LOW);
      current_note = NO_NOTE;
      return false;
    }
  }
  
  startup_start_time = millis();
   //iterate through all frequencies with jitter to determine the average frequency for that resistance.
  for(uint16_t resistance = JITTER; resistance <= NUM_RESISTANCE_STEPS-JITTER; resistance++){
    current_resistance = resistance;
    set_jitter_resistance(current_resistance, JITTER);

     //measure the frequency NUM_FREQ_READINGS times:
     //wait for NUM_FREQ_READINGS*INIT_MULTIPLIER rising edge before beginning:
    int num_rising_edges = 0;
    while(num_rising_edges < NUM_FREQ_READINGS*INIT_MULTIPLIER){
      if(is_rising_edge()){
        num_rising_edges++;
        last_rising_edge = 0;
      }
       //If it doesn't detect a rising edge in time mid frequency checking, set the value to the previous value and continue.
      if((startup_start_time + TIME_TO_WAIT_FOR_INIT) < millis()){
        measured_freqs[current_resistance] = measured_freqs[current_resistance-1];
        break;
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
         //This will break out of the for loop if the measured frequency is higher than MAX_FREQ (less us)
        if(measured_freqs[current_resistance] < MAX_FREQ){
           //Set resistance to a failure condition to stop iterating through the for loop.
          resistance = NUM_RESISTANCE_STEPS;
           //fill in the rest of the array with the MAX_FREQ to keep the rest of the code working.
          for(int i=resistance; i<NUM_RESISTANCE_STEPS-JITTER; i++){
            measured_freqs[current_resistance] = MAX_FREQ;
          }
        }

         //This is a check to see if the frequency is unreasonably large, and should be thrown out.
        if(current_resistance > 5){ //skip the first few, as there's nothing to compare it to.
          if(measured_freqs[current_resistance] > UNREASONABLY_LARGE_MULTIPLIER*measured_freqs[current_resistance-1]){
            measured_freqs[current_resistance] = measured_freqs[current_resistance-1];
          }
        }

        #ifdef OMIDITONE_DEBUG
          /* spammy - re-enable if needed. 
          Serial.print("Frequency at Resistance ");
          Serial.print(current_resistance);
          Serial.print(" is ");
          Serial.println(measured_freqs[current_resistance]);
          /**/
        #endif

        //reset the timeout when a new frequency measurement has occurred.
        last_frequency_measurement = 0;
        break; //break the while loop for this specific resistance value
      }
       //If it doesn't detect a rising edge in time mid frequency checking, set the value to the previous value and continue.
      if(last_frequency_measurement > NOTE_TIMEOUT || last_rising_edge > NOTE_TIMEOUT*1000){
        measured_freqs[current_resistance] = measured_freqs[current_resistance-1];
        //reset the timeout counter when breaking a loop for timeout.
        last_frequency_measurement = 0;
        break; //break the while loop for this specific resistance value
      }
    } //while (1)
  } //for resistance step loop
  digitalWrite(signal_enable_optoisolator_pin, LOW);

  //End manual control of the signal_enable_optoisolator_pin and resistance number - from here on in, use note_on() and note_off()

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
  #ifdef OMIDITONE_DEBUG
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

  #ifdef OMIDITONE_DEBUG
    Serial.print("Initial Min MIDI note is: ");
    Serial.println(min_note);
    Serial.print("Initial Max MIDI note is: ");
    Serial.println(max_note);
  #endif

   //if it makes it here the init was successful and we can move on to testing the available note range
  if(min_note <= max_note){
    had_successful_init = true;
  }
  else{
     //only continue when the min and max note values make sense.
    return false;
  }

   //This will only happen if nothing went wrong above and the oMIDItone is ready for use.
   //Turn the speaker output back on now that it's ready to work:
  digitalWrite(speaker_disable_optoisolator_pin, HIGH);
   //and finally return true.
  return true;
}