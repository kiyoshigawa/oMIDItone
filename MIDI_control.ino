/* 
This contains all the MIDI handling functions for the oMIDItone.
It has both the hardware MIDI interpreting, as well as the USB MIDI interpreting
Make sure to update the MICI_CCV values if new lighting modes are added to lighting control.h
Otherwise the new modes will not work with the oMIDItone, since they are not explicitly allowed in the handle_cc() function
*/

//these define the background lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_BG_TYPE_OFF 1
#define MIDI_CCV_BG_TYPE_SOLID 2
#define MIDI_CCV_BG_TYPE_SLOW_FADE 3
#define MIDI_CCV_BG_TYPE_RAINBOW 4
#define MIDI_CCV_BG_TYPE_RAINBOW_FIXED 5

//these define the foreground lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
//Note that the FG numbers will be bit shifted 8 bits over to comply with the lighting controller values. Verify that the numbers used correspond correctly.
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_FG_TYPE_NONE 0
#define MIDI_CCV_FG_TYPE_MARQUEE_SOLID_FIXED 1
#define MIDI_CCV_FG_TYPE_MARQUEE_SOLID 2
#define MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE_FIXED 3
#define MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE 4
#define MIDI_CCV_FG_TYPE_VU_METER 5

//These define which trigger to use when receiving a MICI_CC_OM#_TRIGGER command. They should be sent as the CC Value
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_TR_TYPE_BG 0
#define MIDI_CCV_TR_TYPE_FG 1
#define MIDI_CCV_TR_TYPE_COLOR_PULSE 2
#define MIDI_CCV_TR_TYPE_COLOR_PULSE_SLOW_FADE 3
#define MIDI_CCV_TR_TYPE_COLOR_PULSE_RAINBOW 4
#define MIDI_CCV_TR_TYPE_COLOR_SHOT 5
#define MIDI_CCV_TR_TYPE_COLOR_SHOT_SLOW_FADE 6
#define MIDI_CCV_TR_TYPE_COLOR_SHOT_RAINBOW 7
#define MIDI_CCV_TR_TYPE_FLASH 8
#define MIDI_CCV_TR_TYPE_FLASH_SLOW_FADE 9
#define MIDI_CCV_TR_TYPE_FLASH_RAINBOW 10

//These define the MIDI control codes to change lighting and animation effects:

//standard MIDI codes that may be useful to implement go here.
#define MIDI_CC_ALL_SOUND_OFF 120
#define MIDI_CC_RESET_ALL_CONTROLLERS 121
#define MIDI_CC_ALL_NOTES_OFF 123

//These control whether major features are enabled or disabled. A value of 0 disables, all other values enable
#define MIDI_CC_PITCH_CORRECTION_ENABLE 14
#define MIDI_CC_NOTE_TRIGGER_ENABLE 15
#define MIDI_CC_LIGHTING_ENABLE 20
#define MIDI_CC_SERVO_ENABLE 21

//bg rainbow selections will select from the bg_array[]
//the CC value corresponds to the index for the array. If it is too large, it goes to the max value.
#define MIDI_CC_OM1_BG_RAINBOW 22
#define MIDI_CC_OM2_BG_RAINBOW 23
#define MIDI_CC_OM3_BG_RAINBOW 24
#define MIDI_CC_OM4_BG_RAINBOW 25
#define MIDI_CC_OM5_BG_RAINBOW 26
#define MIDI_CC_OM6_BG_RAINBOW 27

//these allow you to change the background animation type
//the data needs to be set to one of the MIDI_CCV_BG_* types defined below
#define MIDI_CC_OM1_BG_CHANGE 28
#define MIDI_CC_OM2_BG_CHANGE 29
#define MIDI_CC_OM3_BG_CHANGE 30
#define MIDI_CC_OM4_BG_CHANGE 31
#define MIDI_CC_OM5_BG_CHANGE 46
#define MIDI_CC_OM6_BG_CHANGE 47

//fg rainbow selections will select from the fg_rainbow_array[]
//the CC value corresponds to the index for the array. If it is too large, it goes to the max value.
#define MIDI_CC_OM1_FG_RAINBOW 52
#define MIDI_CC_OM2_FG_RAINBOW 53
#define MIDI_CC_OM3_FG_RAINBOW 54
#define MIDI_CC_OM4_FG_RAINBOW 55
#define MIDI_CC_OM5_FG_RAINBOW 56
#define MIDI_CC_OM6_FG_RAINBOW 57

//these allow you to change the background animation type
//the data needs to be set to one of the MIDI_CCV_FG_* types defined below
#define MIDI_CC_OM1_FG_CHANGE 58
#define MIDI_CC_OM2_FG_CHANGE 59
#define MIDI_CC_OM3_FG_CHANGE 60
#define MIDI_CC_OM4_FG_CHANGE 61
#define MIDI_CC_OM5_FG_CHANGE 62
#define MIDI_CC_OM6_FG_CHANGE 63

//trigger rainbow selections will select from the tr_rainbow_array[]
//the CC value corresponds to the index for the array. If it is too large, it goes to the max value.
#define MIDI_CC_OM1_TR_RAINBOW 102
#define MIDI_CC_OM2_TR_RAINBOW 103
#define MIDI_CC_OM3_TR_RAINBOW 104
#define MIDI_CC_OM4_TR_RAINBOW 105
#define MIDI_CC_OM5_TR_RAINBOW 106
#define MIDI_CC_OM6_TR_RAINBOW 107

//these will force a trigger event of the type specified in the cc value when called, even if the MIDI_CC_NOTE_TRIGGER_ENABLE is set to false
//the data needs to be set to one of the MIDI_CCV_TR_TYPE values below to function
#define MIDI_CC_OM1_TRIGGER 108
#define MIDI_CC_OM2_TRIGGER 109
#define MIDI_CC_OM3_TRIGGER 110
#define MIDI_CC_OM4_TRIGGER 111
#define MIDI_CC_OM5_TRIGGER 112
#define MIDI_CC_OM6_TRIGGER 113

//these allow you to change the trigger event type that occurs on a note_on message per head
//the data needs to be set to one of the LC_TRIGGER_* types defined in lighting_control.h
#define MIDI_CC_OM1_NOTE_TR_CHANGE 114
#define MIDI_CC_OM2_NOTE_TR_CHANGE 115
#define MIDI_CC_OM3_NOTE_TR_CHANGE 116
#define MIDI_CC_OM4_NOTE_TR_CHANGE 117
#define MIDI_CC_OM5_NOTE_TR_CHANGE 118
#define MIDI_CC_OM6_NOTE_TR_CHANGE 119

//This will read the hardware MIDI and set or remove notes as needed.
void read_hardware_MIDI(){
  int type, note, velocity, channel, p1, p2, d1, d2, control_number, control_value;
  if (MIDI.read()) {                    // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case midi::NoteOn:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        channel = MIDI.getChannel();
        if (velocity > 0) {
          //Set the note on as usual:
          add_note(note, velocity, channel);
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
        current_pitch_shift[channel] = (p2<<7) + p1 - 8192;
        pitch_has_changed = true;
        #ifdef DEBUG
          Serial.print("Pitch Shift set to ");
          Serial.print(current_pitch_shift[channel]);
          Serial.print(" on channel ");
          Serial.print(channel);
          Serial.println(".");
        #endif
        break;
      case midi::ControlChange:
        control_number = MIDI.getData1();
        control_value = MIDI.getData2();
        channel = MIDI.getChannel();
        handle_cc(channel, control_number, control_value);
        break;
      default:
        //this just clears the buffers when the stuff isn't used
        d1 = MIDI.getData1();
        d2 = MIDI.getData2();
    }
  } 
}

//update the note_status_array to set the requested note as on.
void OnNoteOn(byte channel, byte note, byte velocity){
  if(velocity > 0){
    add_note(note, velocity, channel);
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
  current_pitch_shift[channel] = pitch;
  pitch_has_changed = true;
  #ifdef DEBUG
    Serial.print("Pitch Shift set to ");
    Serial.print(current_pitch_shift[channel]);
    Serial.print(" on channel ");
    Serial.print(channel);
    Serial.println(".");
  #endif
}

//this will run control commands for the heads, such as enabling/disabling lighting and servos, as well as changing animation types
void OnControlChange(byte channel, byte control_number, byte control_value){
  handle_cc(channel, control_number, control_value);
}

//this interprets the CC messages read by either hardware of USB midi and takes the appropriate action.
void handle_cc(byte channel, byte cc_number, byte cc_value){
  //first we change the things that don't need to iterate through each head to be set properly:

  //this will disable note trigger events:
  if(cc_number == MIDI_CC_NOTE_TRIGGER_ENABLE){
    //enable for any value other than 0
    if(cc_value){
      note_trigger_is_enabled = true;
      #ifdef DEBUG
        Serial.println("Trigger events occur on note_on messages.");
      #endif
      return;
    }
    else{
      note_trigger_is_enabled = false;
      #ifdef DEBUG
        Serial.println("Trigger events no longer occur on note_on messages.");
      #endif
      return;
    }
  }
  //this will enable or disable lighting animations. When disabled, lighting will remain in the last state it was in when disabled.
  else if(cc_number == MIDI_CC_LIGHTING_ENABLE){
    //enable for any value other than 0
    if(cc_value){
      lighting_is_enabled = true;
      #ifdef DEBUG
        Serial.println("Lighting updates are enabled.");
      #endif
      return;
    }
    else{
      lighting_is_enabled = false;
      #ifdef DEBUG
        Serial.println("Light updates are disabled.");
      #endif
      return;
    }
  }
  //this forces a software reset of the teensy board
  else if(cc_number == MIDI_CC_RESET_ALL_CONTROLLERS){
    #ifdef DEBUG
      Serial.print("Manual Reset of board triggered. Shutting down...");
      delay(500);
    #endif
    _softRestart();
  }

  //some cc_numbers are head specific, so this variable sets the head properly if it's one of those.
  uint8_t head;
  //head 1 specific CC messages
  if(
    cc_number == MIDI_CC_OM1_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM1_BG_CHANGE      ||
    cc_number == MIDI_CC_OM1_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM1_FG_CHANGE      ||
    cc_number == MIDI_CC_OM1_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM1_TRIGGER        ||
    cc_number == MIDI_CC_OM1_NOTE_TR_CHANGE ){
    head = 0;
  }
  //head 2 specific CC messages
  else if(
    cc_number == MIDI_CC_OM2_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM2_BG_CHANGE      ||
    cc_number == MIDI_CC_OM2_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM2_FG_CHANGE      ||
    cc_number == MIDI_CC_OM2_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM2_TRIGGER        ||
    cc_number == MIDI_CC_OM2_NOTE_TR_CHANGE ){
    head = 1;
  }
  //head 3 specific CC messages
  else if(
    cc_number == MIDI_CC_OM3_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM3_BG_CHANGE      ||
    cc_number == MIDI_CC_OM3_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM3_FG_CHANGE      ||
    cc_number == MIDI_CC_OM3_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM3_TRIGGER        ||
    cc_number == MIDI_CC_OM3_NOTE_TR_CHANGE ){
    head = 2;
  }
  //head 4 specific CC messages
  else if(
    cc_number == MIDI_CC_OM4_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM4_BG_CHANGE      ||
    cc_number == MIDI_CC_OM4_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM4_FG_CHANGE      ||
    cc_number == MIDI_CC_OM4_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM4_TRIGGER        ||
    cc_number == MIDI_CC_OM4_NOTE_TR_CHANGE ){
    head = 3;
  }
  //head 5 specific CC messages
  else if(
    cc_number == MIDI_CC_OM5_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM5_BG_CHANGE      ||
    cc_number == MIDI_CC_OM5_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM5_FG_CHANGE      ||
    cc_number == MIDI_CC_OM5_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM5_TRIGGER        ||
    cc_number == MIDI_CC_OM5_NOTE_TR_CHANGE ){
    head = 4;
  }
  //head 6 specific CC messages
  else if(
    cc_number == MIDI_CC_OM6_BG_RAINBOW     ||
    cc_number == MIDI_CC_OM6_BG_CHANGE      ||
    cc_number == MIDI_CC_OM6_FG_RAINBOW     ||
    cc_number == MIDI_CC_OM6_FG_CHANGE      ||
    cc_number == MIDI_CC_OM6_TR_RAINBOW     ||
    cc_number == MIDI_CC_OM6_TRIGGER        ||
    cc_number == MIDI_CC_OM6_NOTE_TR_CHANGE ){
    head = 5;
  }

  //Head specific statements:

  //Set the background rainbow
  if( cc_number == MIDI_CC_OM1_BG_RAINBOW ||
      cc_number == MIDI_CC_OM2_BG_RAINBOW ||
      cc_number == MIDI_CC_OM3_BG_RAINBOW ||
      cc_number == MIDI_CC_OM4_BG_RAINBOW ||
      cc_number == MIDI_CC_OM5_BG_RAINBOW ||
      cc_number == MIDI_CC_OM6_BG_RAINBOW ){
    if(cc_value < num_bg_rainbows){
      oms[head].animation->change_rainbow(LC_BG, bg_array[cc_value]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Background rainbow changed to ");
        Serial.println(cc_value);
      #endif
      return;
    }
    else{
      //set to the largest value if the received CC message has more rainbows than the bg_array
      oms[head].animation->change_rainbow(LC_BG, bg_array[num_bg_rainbows-1]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Background rainbow changed to ");
        Serial.println(num_bg_rainbows-1);
      #endif
      return;
    }
  }
  //Set the foreground rainbow
  else if(cc_number == MIDI_CC_OM1_FG_RAINBOW ||
          cc_number == MIDI_CC_OM2_FG_RAINBOW ||
          cc_number == MIDI_CC_OM3_FG_RAINBOW ||
          cc_number == MIDI_CC_OM4_FG_RAINBOW ||
          cc_number == MIDI_CC_OM5_FG_RAINBOW ||
          cc_number == MIDI_CC_OM6_FG_RAINBOW ){
    if(cc_value < num_fg_rainbows){
      oms[head].animation->change_rainbow(LC_FG, fg_array[cc_value]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Foreground rainbow changed to ");
        Serial.println(cc_value);
      #endif
      return;
    }
    else{
      //set to the largest value if the received CC message has more rainbows than the bg_array
      oms[head].animation->change_rainbow(LC_FG, fg_array[num_fg_rainbows-1]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Foreground rainbow changed to ");
        Serial.println(num_fg_rainbows-1);
      #endif
      return;
    }
  }
  //set the trigger rainbow
  else if(cc_number == MIDI_CC_OM1_TR_RAINBOW ||
          cc_number == MIDI_CC_OM2_TR_RAINBOW ||
          cc_number == MIDI_CC_OM3_TR_RAINBOW ||
          cc_number == MIDI_CC_OM4_TR_RAINBOW ||
          cc_number == MIDI_CC_OM5_TR_RAINBOW ||
          cc_number == MIDI_CC_OM6_TR_RAINBOW ){
    if(cc_value < num_fg_rainbows){
      oms[head].animation->change_rainbow(LC_TRIGGER, trigger_array[cc_value]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Trigger rainbow changed to ");
        Serial.println(cc_value);
      #endif
      return;
    }
    else{
      //set to the largest value if the received CC message has more rainbows than the bg_array
      oms[head].animation->change_rainbow(LC_TRIGGER, trigger_array[num_trigger_rainbows-1]);
      #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
        Serial.print(": Trigger rainbow changed to ");
        Serial.println(num_trigger_rainbows-1);
      #endif
      return;
    }
  }
  //change the background animation type
  else if(cc_number == MIDI_CC_OM1_BG_CHANGE ||
          cc_number == MIDI_CC_OM2_BG_CHANGE ||
          cc_number == MIDI_CC_OM3_BG_CHANGE ||
          cc_number == MIDI_CC_OM4_BG_CHANGE ||
          cc_number == MIDI_CC_OM5_BG_CHANGE ||
          cc_number == MIDI_CC_OM6_BG_CHANGE ){
    //TIM: add check to make sure it's valid, as well as a default.
    oms[head].animation->change_lighting_mode(cc_value);
    #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
      Serial.print(": Foreground lighting mode has changed to ");
      Serial.println(cc_value, HEX);
    #endif
    return;
  }
  //change the foreground animation type
  else if(cc_number == MIDI_CC_OM1_FG_CHANGE ||
          cc_number == MIDI_CC_OM2_FG_CHANGE ||
          cc_number == MIDI_CC_OM3_FG_CHANGE ||
          cc_number == MIDI_CC_OM4_FG_CHANGE ||
          cc_number == MIDI_CC_OM5_FG_CHANGE ||
          cc_number == MIDI_CC_OM6_FG_CHANGE ){
    //TIM: add check to make sure it's valid, as well as a default.
    oms[head].animation->change_lighting_mode(cc_value<<8); //bit shifted because of how the lighting mode definitions work. See lighting_control.h
    #ifdef DEBUG
        Serial.print("Head ");
        Serial.print(head+1);
      Serial.print(": Foreground lighting mode has changed to ");
      Serial.println(cc_value, HEX);
    #endif
    return;
  }
  //fire off a trigger animation, regardless of the note trigger status
  else if(cc_number == MIDI_CC_OM1_TRIGGER ||
          cc_number == MIDI_CC_OM2_TRIGGER ||
          cc_number == MIDI_CC_OM3_TRIGGER ||
          cc_number == MIDI_CC_OM4_TRIGGER ||
          cc_number == MIDI_CC_OM5_TRIGGER ||
          cc_number == MIDI_CC_OM6_TRIGGER ){
    //TIM: add check to make sure it's valid, as well as a default.
    #ifdef DEBUG
      Serial.print("Head ");
      Serial.print(head+1);
      Serial.print(": An event has been triggered of type ");
      Serial.println(cc_value, HEX);
    #endif
    return;
  }
  //change the type of animation to use for note triggered animations - varies per head
  else if(cc_number == MIDI_CC_OM1_NOTE_TR_CHANGE ||
          cc_number == MIDI_CC_OM2_NOTE_TR_CHANGE ||
          cc_number == MIDI_CC_OM3_NOTE_TR_CHANGE ||
          cc_number == MIDI_CC_OM4_NOTE_TR_CHANGE ||
          cc_number == MIDI_CC_OM5_NOTE_TR_CHANGE ||
          cc_number == MIDI_CC_OM6_NOTE_TR_CHANGE ){
    //TIM: add check to make sure it's valid, as well as a default.
    note_trigger_type[head] = cc_value;
    #ifdef DEBUG
      Serial.print("Head ");
      Serial.print(head+1);
      Serial.print(": Note Trigger mode has changed to ");
      Serial.println(cc_value, HEX);
    #endif
    return;
  }

  //then we do this for loop because some cc messages apply to all heads at once and will need to be iterated through:
  for(int h=0; h<NUM_OMIDITONES; h++){
    //all notes off.
    if(cc_number == MIDI_CC_ALL_NOTES_OFF || cc_number == MIDI_CC_ALL_SOUND_OFF){
      //since each head can only play one note at a time, we only need to turn off the currently_playing_note
      oms[h].note_off(oms[h].currently_playing_note());
      //only return on the last head
      if(h == (NUM_OMIDITONES-1)){
        #ifdef DEBUG
          Serial.println("All notes off CC message received.");
        #endif
        return;
      }
    }
    //this will disable servo movements:
    else if(cc_number == MIDI_CC_SERVO_ENABLE){
      //enable for any value other than 0
      if(cc_value){
        oms[h].enable_servos();
        //only return on the last head
        if(h == (NUM_OMIDITONES-1)){
          #ifdef DEBUG
            Serial.println("Servos have been enabled.");
          #endif
          return;
        }
      }
      else{
        oms[h].disable_servos();
        //only return on the last head
        if(h == (NUM_OMIDITONES-1)){
          #ifdef DEBUG
            Serial.println("Servos been disabled.");
          #endif
          return;
        }
      }
    }
    //this will disable pitch correction:
    else if(cc_number == MIDI_CC_PITCH_CORRECTION_ENABLE){
      //enable for any value other than 0
      if(cc_value){
        oms[h].enable_pitch_correction();
        if(h == (NUM_OMIDITONES-1)){
          #ifdef DEBUG
            Serial.println("Pitch Correction has been enabled.");
          #endif
          return;
        }
      }
      else{
        oms[h].disable_pitch_correction();
        if(h == (NUM_OMIDITONES-1)){
          #ifdef DEBUG
            Serial.println("Pitch Correction has been disabled.");
          #endif
          return;
        }
      }
    }
  }// head for loop
  //if it makes it here without returning, note that nothing happened.
  #ifdef DEBUG
    Serial.print("CC ");
    Serial.print(cc_number);
    Serial.println(" is unassigned. No action taken.");
  #endif
}