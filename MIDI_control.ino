/*
This contains all the MIDI handling functions for the oMIDItone.
It has both the hardware MIDI interpreting, as well as the USB MIDI interpreting
Make sure to update the MICI_CCV values if new lighting modes are added to lighting control.h
Otherwise the new modes will not work with the oMIDItone, since they are not explicitly allowed in the handle_cc() function
*/

#define MIDI_DEBUG

//these define the background lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_BG_TYPE_OFF 1
#define MIDI_CCV_BG_TYPE_SOLID 2
#define MIDI_CCV_BG_TYPE_SLOW_FADE 3
#define MIDI_CCV_BG_TYPE_RAINBOW_FIXED 4
#define MIDI_CCV_BG_TYPE_RAINBOW_SLOW_ROTATE 5

//this is used when an invalid code is sent:
#define DEFAULT_BG MIDI_CCV_BG_TYPE_RAINBOW_SLOW_ROTATE

//these define the foreground lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
//Note that the FG numbers will be bit shifted 8 bits over to comply with the lighting controller values. Verify that the numbers used correspond correctly.
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_FG_TYPE_NONE 0
#define MIDI_CCV_FG_TYPE_MARQUEE_SOLID_FIXED 1
#define MIDI_CCV_FG_TYPE_MARQUEE_SOLID 2
#define MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE_FIXED 3
#define MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE 4
#define MIDI_CCV_FG_TYPE_VU_METER 5

//this is used when an invalid code is sent:
#define DEFAULT_FG MIDI_CCV_FG_TYPE_NONE

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

//this is used when an invalid code is sent:
#define DEFAULT_TRIGGER MIDI_CCV_TR_TYPE_COLOR_PULSE

//These define the MIDI control codes to change lighting and animation effects:

//standard MIDI codes that may be useful to implement go here.
#define MIDI_CC_ALL_SOUND_OFF 120
#define MIDI_CC_RESET_ALL_CONTROLLERS 121
#define MIDI_CC_ALL_NOTES_OFF 123

//this is used as the default note value for pitsh bend when a CC 121 is sent.
#define DEFAULT_PITCH_BEND 0

//this is used for setting the number of semitones for MIDI pitch bending
#define MIDI_CC_PITCH_BEND_RANGE_COARSE 6
//this is used for setting the number of cents off of the semitones above for MIDI pitch bending
#define MIDI_CC_PITCH_BEND_RANGE_FINE 38

//the pitch bend range will only work if both CC100 and CC101 are set to 0
#define MIDI_CC_REGISTERED_PARAMETER_LSB 100
#define MIDI_CC_REGISTERED_PARAMETER_MSB 101

//these are defines used for MIDI RPN messages to determine what type of RPN message is being sent:
//so far only pitch bend adjust is implemented
#define MIDI_RPN_PITCH_BEND_ADJUST_MODE 0
#define MIDI_RPN_FINE_TUNING_MODE 1
#define MIDI_RPN_COARSE_TUNING_MODE 2
#define MIDI_RPN_TUNING_PROGRAM_CHANGE_MODE 3
#define MIDI_RPN_TUNING_BANK_SELECT_MODE 4
#define MIDI_RPN_MODULATION_DEPTH_RANGE_MODE 5

#define RPN_NO_MODE 127

//this resets the MCU and forces a new tune-up:
#define MIDI_CC_HARD_RESET 9

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

//these allow setting the servo position of all heads directly, even when MIDI_CC_SERVO_ENABLE is not on.
//Useful for opening mouths when servos are disabled to record audio.
#define MIDI_CC_OM1_SERVO_POSITION 41
#define MIDI_CC_OM2_SERVO_POSITION 85
#define MIDI_CC_OM3_SERVO_POSITION 86
#define MIDI_CC_OM4_SERVO_POSITION 87
#define MIDI_CC_OM5_SERVO_POSITION 89
#define MIDI_CC_OM6_SERVO_POSITION 90

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

//we need some MIDI global variables to handle regietered parameter messages

//when both RPN values are set to 0, channels 6 and 38 are used for explicitly setting the pitch bend range
//I have not implemented incrementing or decrementing pitch bending using notes 96 and 97 at this time.
//Other modes supported by MIDI but not this controller are:
	//1: fine tuning
	//2: coarse tuning
	//3: tuning program change
	//4: tuning bank select
	//5: modulation depth range
uint8_t MIDI_CC_RPN_MSB_value = RPN_NO_MODE;
uint8_t MIDI_CC_RPN_LSB_value = RPN_NO_MODE;

//this allows me to reset the teensy when it receives a MIDI CC121 reset command.
#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control location

void _softRestart()
{
	Serial.end();  //clears the serial monitor  if used
	SCB_AIRCR = 0x05FA0004;  //write value for restart
}

//these check to make sure imputs are valid:
//Head 1 Check:
bool is_head_1(uint8_t cc_number){
	if( cc_number == MIDI_CC_OM1_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM1_BG_CHANGE      ||
		cc_number == MIDI_CC_OM1_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM1_FG_CHANGE      ||
		cc_number == MIDI_CC_OM1_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM1_TRIGGER        ||
		cc_number == MIDI_CC_OM1_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM1_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//Head 2 Check:
bool is_head_2(uint8_t cc_number){
	if( cc_number == MIDI_CC_OM2_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM2_BG_CHANGE      ||
		cc_number == MIDI_CC_OM2_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM2_FG_CHANGE      ||
		cc_number == MIDI_CC_OM2_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM2_TRIGGER        ||
		cc_number == MIDI_CC_OM2_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM2_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//Head 3 Check:
bool is_head_3(uint8_t cc_number){
	if(	cc_number == MIDI_CC_OM3_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM3_BG_CHANGE      ||
		cc_number == MIDI_CC_OM3_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM3_FG_CHANGE      ||
		cc_number == MIDI_CC_OM3_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM3_TRIGGER        ||
		cc_number == MIDI_CC_OM3_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM3_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//Head 4 Check:
bool is_head_4(uint8_t cc_number){
	if(	cc_number == MIDI_CC_OM4_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM4_BG_CHANGE      ||
		cc_number == MIDI_CC_OM4_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM4_FG_CHANGE      ||
		cc_number == MIDI_CC_OM4_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM4_TRIGGER        ||
		cc_number == MIDI_CC_OM4_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM4_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//Head 5 Check:
bool is_head_5(uint8_t cc_number){
	if(	cc_number == MIDI_CC_OM5_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM5_BG_CHANGE      ||
		cc_number == MIDI_CC_OM5_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM5_FG_CHANGE      ||
		cc_number == MIDI_CC_OM5_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM5_TRIGGER        ||
		cc_number == MIDI_CC_OM5_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM5_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//Head 6 Check:
bool is_head_6(uint8_t cc_number){
	if(	cc_number == MIDI_CC_OM6_BG_RAINBOW     ||
		cc_number == MIDI_CC_OM6_BG_CHANGE      ||
		cc_number == MIDI_CC_OM6_FG_RAINBOW     ||
		cc_number == MIDI_CC_OM6_FG_CHANGE      ||
		cc_number == MIDI_CC_OM6_TR_RAINBOW     ||
		cc_number == MIDI_CC_OM6_TRIGGER        ||
		cc_number == MIDI_CC_OM6_NOTE_TR_CHANGE ||
		cc_number == MIDI_CC_OM6_SERVO_POSITION	){
		return true;
	} else {
		return false;
	}
}

//checks for valid bg animation modes
bool is_valid_bg(uint8_t cc_value){
	if(	cc_value == MIDI_CCV_BG_TYPE_OFF ||
		cc_value == MIDI_CCV_BG_TYPE_SOLID ||
		cc_value == MIDI_CCV_BG_TYPE_SLOW_FADE ||
		cc_value == MIDI_CCV_BG_TYPE_RAINBOW_FIXED ||
		cc_value == MIDI_CCV_BG_TYPE_RAINBOW_SLOW_ROTATE ){
		return true;
	} else {
		return false;
	}
}

//checks for valid fg animation modes
bool is_valid_fg(uint8_t cc_value){
	if(	cc_value == MIDI_CCV_FG_TYPE_NONE ||
		cc_value == MIDI_CCV_FG_TYPE_MARQUEE_SOLID ||
		cc_value == MIDI_CCV_FG_TYPE_MARQUEE_SOLID_FIXED ||
		cc_value == MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE ||
		cc_value == MIDI_CCV_FG_TYPE_MARQUEE_SLOW_FADE_FIXED ||
		cc_value == MIDI_CCV_FG_TYPE_VU_METER ){
		return true;
	} else {
		return false;
	}
}

//checks for valid trigger animation modes
bool is_valid_trigger(uint8_t cc_value){
	if(	cc_value == MIDI_CCV_TR_TYPE_BG ||
		cc_value == MIDI_CCV_TR_TYPE_FG ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_PULSE ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_PULSE_SLOW_FADE ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_PULSE_RAINBOW ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_SHOT ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_SHOT_SLOW_FADE ||
		cc_value == MIDI_CCV_TR_TYPE_COLOR_SHOT_RAINBOW ||
		cc_value == MIDI_CCV_TR_TYPE_FLASH ||
		cc_value == MIDI_CCV_TR_TYPE_FLASH_SLOW_FADE ||
		cc_value == MIDI_CCV_TR_TYPE_FLASH_RAINBOW ){
		return true;
	} else {
		return false;
	}
}

//This will read the hardware MIDI and set or remove notes as needed.
void read_hardware_MIDI(){
	uint8_t type, note, velocity, channel, p1, p2, control_number, control_value;
	if (MIDI.read()) {                    // Is there a MIDI message incoming ?
		type = MIDI.getType();
		switch (type) {
			case midi::NoteOn:
				note = MIDI.getData1();
				velocity = MIDI.getData2();
				channel = MIDI.getChannel();
				if (velocity > 0) {
					//Set the note on as usual:
					add_note(note, velocity, channel);
					note_has_changed = true;
					#ifdef MIDI_DEBUG
						print_current_note_array();
					#endif
				} else {
					//Set the note off.
					remove_note(note);
					note_has_changed = true;
				#ifdef MIDI_DEBUG
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
				#ifdef MIDI_DEBUG
					print_current_note_array();
				#endif
				break;
			case midi::PitchBend:
				p1 = MIDI.getData1();
				p2 = MIDI.getData2();
				channel = MIDI.getChannel();
				//shift the bits so it's a single number from -8192 to 8192.
				current_pitch_bend[channel] = (p2<<7) + p1 - 8192;
				pitch_has_changed = true;
				//also update any currently playing notes when changing pitch
				if(num_current_notes > 0){
					note_has_changed = true;
				}
				#ifdef MIDI_DEBUG
					Serial.print("Bend: ");
					Serial.print(current_pitch_bend[channel]);
					Serial.print(" Chan: ");
					Serial.println(channel);
				#endif
				break;
			case midi::ControlChange:
				control_number = MIDI.getData1();
				control_value = MIDI.getData2();
				channel = MIDI.getChannel();
				handle_cc(channel, control_number, control_value);
				break;
			default:
				//do nothing
				break;
		}
	}
}

//update the note_status_array to set the requested note as on.
void OnNoteOn(uint8_t channel, uint8_t note, uint8_t velocity){
	if(velocity > 0){
		add_note(note, velocity, channel);
	} else {
		remove_note(note);
	}
	note_has_changed = true;
	#ifdef MIDI_DEBUG
		print_current_note_array();
	#endif
}

//This updated the note_status_array to set the requested note to off:
void OnNoteOff(uint8_t channel, uint8_t note, uint8_t velocity){
	remove_note(note);
	note_has_changed = true;
	#ifdef MIDI_DEBUG
		print_current_note_array();
	#endif
}

//this sets the pitch_bend value to be used by the oMIDItones:
void OnPitchChange(uint8_t channel, int pitch){
	current_pitch_bend[channel] = pitch;
	pitch_has_changed = true;
	//also update any currently playing notes when changing pitch
	if(num_current_notes > 0){
		note_has_changed = true;
	}
	#ifdef MIDI_DEBUG
		Serial.print("Bend: ");
		Serial.print(current_pitch_bend[channel]);
		Serial.print(" Chan: ");
		Serial.println(channel);
	#endif
}

//this will run control commands for the heads, such as enabling/disabling lighting and servos, as well as changing animation types
void OnControlChange(uint8_t channel, uint8_t control_number, uint8_t control_value){
	handle_cc(channel, control_number, control_value);
}

//this interprets the CC messages read by either hardware of USB midi and takes the appropriate action.
void handle_cc(uint8_t channel, uint8_t cc_number, uint8_t cc_value){
	//first we change the things that don't need to iterate through each head to be set properly:

	//this will set the globals for the Registered Parameters (RPNs)
	if(cc_number == MIDI_CC_REGISTERED_PARAMETER_LSB){
		MIDI_CC_RPN_LSB_value = cc_value;
		return;
	} else if(cc_number == MIDI_CC_REGISTERED_PARAMETER_MSB){
		MIDI_CC_RPN_MSB_value = cc_value;
		return;
	} else if(cc_number == MIDI_CC_PITCH_BEND_RANGE_COARSE){
		//this changes the pitch bend range when the RPNs are both set to MIDI_RPN_PITCH_BEND_ADJUST_MODE.
		if( (MIDI_CC_RPN_MSB_value == MIDI_RPN_PITCH_BEND_ADJUST_MODE) && (MIDI_CC_RPN_LSB_value == MIDI_RPN_PITCH_BEND_ADJUST_MODE) ){
			MIDI_max_pitch_bend_num_semitones = cc_value;
			pitch_bend_range_has_changed = true;
			#ifdef MIDI_DEBUG
				Serial.print("Max Bend: ");
				Serial.print(MIDI_max_pitch_bend_num_semitones*100+MIDI_max_pitch_bend_num_cents);
				Serial.println(" cents.");
			#endif
			return;
		} else {
			#ifdef MIDI_DEBUG
				Serial.println("CC100+CC101 Unknown Config");
			#endif
			return;
		}
	} else if(cc_number == MIDI_CC_PITCH_BEND_RANGE_FINE){
		if( (MIDI_CC_RPN_MSB_value == MIDI_RPN_PITCH_BEND_ADJUST_MODE) && (MIDI_CC_RPN_LSB_value == MIDI_RPN_PITCH_BEND_ADJUST_MODE) ){
			MIDI_max_pitch_bend_num_cents = cc_value;
			pitch_bend_range_has_changed = true;
			#ifdef MIDI_DEBUG
				Serial.print("Max Bend: ");
				Serial.print(MIDI_max_pitch_bend_num_semitones*100+MIDI_max_pitch_bend_num_cents);
				Serial.println(" cents.");
			#endif
			return;
		} else {
			#ifdef MIDI_DEBUG
				Serial.println("CC100+CC101 Unknown Config");
			#endif
			return;
		}
	} else if(cc_number == MIDI_CC_NOTE_TRIGGER_ENABLE){
		//this will disable note trigger events:
		//enable for any value other than 0
		if(cc_value){
			note_trigger_is_enabled = true;
			#ifdef MIDI_DEBUG
				Serial.println("NoteOn Triggers Enabled");
			#endif
			return;
		} else {
			note_trigger_is_enabled = false;
			#ifdef MIDI_DEBUG
				Serial.println("NoteOn Triggers Disabled");
			#endif
			return;
		}
	} else if(cc_number == MIDI_CC_LIGHTING_ENABLE){
		//this will enable or disable lighting animations. When disabled, lighting will remain in the last state it was in when disabled.
		//enable for any value other than 0
		if(cc_value){
			lighting_is_enabled = true;
			#ifdef MIDI_DEBUG
				Serial.println("Lighting Updates Enabled");
			#endif
			return;
		} else {
			lighting_is_enabled = false;
			#ifdef MIDI_DEBUG
				Serial.println("Light Updates Eisabled");
			#endif
			return;
		}
	} else if(cc_number == MIDI_CC_HARD_RESET){
		//this forces a software reset of the teensy board
		#ifdef MIDI_DEBUG
			Serial.println("Manual Reset of board triggered. Shutting down...");
			delay(1000);
		#endif
		_softRestart();
	}

	//some cc_numbers are head specific, so this variable sets the head properly if it's one of those.
	uint8_t head = 0;
	//head 1 specific CC messages
	if(is_head_1(cc_number)){
		head = 0;
	} else if(is_head_2(cc_number)){
		//head 2 specific CC messages
		head = 1;
	} else if(is_head_3(cc_number)){
		//head 3 specific CC messages
		head = 2;
	} else if(is_head_4(cc_number)){
		//head 4 specific CC messages
		head = 3;
	} else if(is_head_5(cc_number)){
		//head 5 specific CC messages
		head = 4;
	} else if(is_head_6(cc_number)){
		//head 6 specific CC messages
		head = 5;
	}

	//Head specific statements:

	//Set the background rainbow
	if(	cc_number == MIDI_CC_OM1_BG_RAINBOW ||
		cc_number == MIDI_CC_OM2_BG_RAINBOW ||
		cc_number == MIDI_CC_OM3_BG_RAINBOW ||
		cc_number == MIDI_CC_OM4_BG_RAINBOW ||
		cc_number == MIDI_CC_OM5_BG_RAINBOW ||
		cc_number == MIDI_CC_OM6_BG_RAINBOW ){
		if(cc_value < num_rainbows){
			oms[head].animation->change_rainbow(LC_BG, rb_array[cc_value]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" BG rainbow: ");
				Serial.println(cc_value);
			#endif
			return;
		} else {
			//set to the largest value if the received CC message has more rainbows than the bg_array
			oms[head].animation->change_rainbow(LC_BG, rb_array[0]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" BG rainbow: ");
				Serial.println(0);
			#endif
			return;
		}
		} else if(	cc_number == MIDI_CC_OM1_FG_RAINBOW ||
					cc_number == MIDI_CC_OM2_FG_RAINBOW ||
					cc_number == MIDI_CC_OM3_FG_RAINBOW ||
					cc_number == MIDI_CC_OM4_FG_RAINBOW ||
					cc_number == MIDI_CC_OM5_FG_RAINBOW ||
					cc_number == MIDI_CC_OM6_FG_RAINBOW ){
		//Set the foreground rainbow
		if(cc_value < num_rainbows){
			oms[head].animation->change_rainbow(LC_FG, rb_array[cc_value]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" FG rainbow: ");
				Serial.println(cc_value);
			#endif
			return;
		} else {
			//set to the largest value if the received CC message has more rainbows than the bg_array
			oms[head].animation->change_rainbow(LC_FG, rb_array[0]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" FG rainbow: ");
				Serial.println(0);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_TR_RAINBOW ||
				cc_number == MIDI_CC_OM2_TR_RAINBOW ||
				cc_number == MIDI_CC_OM3_TR_RAINBOW ||
				cc_number == MIDI_CC_OM4_TR_RAINBOW ||
				cc_number == MIDI_CC_OM5_TR_RAINBOW ||
				cc_number == MIDI_CC_OM6_TR_RAINBOW ){
		//set the trigger rainbow
		if(cc_value < num_rainbows){
			oms[head].animation->change_rainbow(LC_TRIGGER, rb_array[cc_value]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trig Rainbow: ");
				Serial.println(cc_value);
			#endif
			return;
		} else {
			//set to the largest value if the received CC message has more rainbows than the bg_array
			oms[head].animation->change_rainbow(LC_TRIGGER, rb_array[0]);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trig Rainbow: ");
				Serial.println(0);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_BG_CHANGE ||
				cc_number == MIDI_CC_OM2_BG_CHANGE ||
				cc_number == MIDI_CC_OM3_BG_CHANGE ||
				cc_number == MIDI_CC_OM4_BG_CHANGE ||
				cc_number == MIDI_CC_OM5_BG_CHANGE ||
				cc_number == MIDI_CC_OM6_BG_CHANGE ){
		//change the background animation type
		//this checks to make sure that a valid lighting mode is used, otherwise the default is selected:
		if(is_valid_bg(cc_value)){
			//the next line is bitwise or-ed with the current bg animation to make sure not to override it.
			oms[head].animation->change_lighting_mode(cc_value | oms[head].animation->current_fg_mode());
			#ifdef MIDI_DEBUG
					Serial.print("Head: ");
					Serial.print(head+1);
				Serial.print(" BG Mode: ");
				Serial.println(cc_value, HEX);
			#endif
			return;
		} else {
			//the next line is bitwise or-ed with the current bg animation to make sure not to override it.
			oms[head].animation->change_lighting_mode(DEFAULT_BG | oms[head].animation->current_fg_mode());
			#ifdef MIDI_DEBUG
					Serial.print("Head: ");
					Serial.print(head+1);
				Serial.print(" BG Mode: ");
				Serial.println(DEFAULT_BG, HEX);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_FG_CHANGE ||
				cc_number == MIDI_CC_OM2_FG_CHANGE ||
				cc_number == MIDI_CC_OM3_FG_CHANGE ||
				cc_number == MIDI_CC_OM4_FG_CHANGE ||
				cc_number == MIDI_CC_OM5_FG_CHANGE ||
				cc_number == MIDI_CC_OM6_FG_CHANGE ){
		//change the foreground animation type
		//this checks to make sure that a valid lighting mode is used, otherwise the default is selected:
		if(is_valid_fg(cc_value)){
			//the next line is bitwise or-ed with the current bg animation to make sure not to override it.
			oms[head].animation->change_lighting_mode(cc_value<<8 | oms[head].animation->current_bg_mode()); //bit shifted because of how the lighting mode definitions work. See lighting_control.h
			#ifdef MIDI_DEBUG
					Serial.print("Head: ");
					Serial.print(head+1);
				Serial.print(" FG Mode: ");
				Serial.println(cc_value, HEX);
			#endif
			return;
		} else {
			//the next line is bitwise or-ed with the current bg animation to make sure not to override it.
			oms[head].animation->change_lighting_mode(DEFAULT_FG<<8 | oms[head].animation->current_bg_mode()); //bit shifted because of how the lighting mode definitions work. See lighting_control.h
			#ifdef MIDI_DEBUG
					Serial.print("Head: ");
					Serial.print(head+1);
				Serial.print(" FG Mode: ");
				Serial.println(DEFAULT_FG, HEX);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_TRIGGER ||
				cc_number == MIDI_CC_OM2_TRIGGER ||
				cc_number == MIDI_CC_OM3_TRIGGER ||
				cc_number == MIDI_CC_OM4_TRIGGER ||
				cc_number == MIDI_CC_OM5_TRIGGER ||
				cc_number == MIDI_CC_OM6_TRIGGER ){
		//fire off a trigger animation, regardless of the note trigger status
		if(is_valid_trigger(cc_value)){
			oms[head].animation->trigger_event(cc_value);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trigger Type: ");
				Serial.println(cc_value, HEX);
			#endif
			return;
		} else {
			oms[head].animation->trigger_event(DEFAULT_TRIGGER);
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trigger Type: ");
				Serial.println(DEFAULT_TRIGGER, HEX);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_NOTE_TR_CHANGE ||
				cc_number == MIDI_CC_OM2_NOTE_TR_CHANGE ||
				cc_number == MIDI_CC_OM3_NOTE_TR_CHANGE ||
				cc_number == MIDI_CC_OM4_NOTE_TR_CHANGE ||
				cc_number == MIDI_CC_OM5_NOTE_TR_CHANGE ||
				cc_number == MIDI_CC_OM6_NOTE_TR_CHANGE ){
		//change the type of animation to use for note triggered animations - varies per head
		if(is_valid_trigger(cc_value)){
			note_trigger_type[head] = cc_value;
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trigger Mode: ");
				Serial.println(cc_value, HEX);
			#endif
			return;
		} else {
			note_trigger_type[head] = DEFAULT_TRIGGER;
			#ifdef MIDI_DEBUG
				Serial.print("Head: ");
				Serial.print(head+1);
				Serial.print(" Trigger Mode: ");
				Serial.println(DEFAULT_TRIGGER, HEX);
			#endif
			return;
		}
	} else if(	cc_number == MIDI_CC_OM1_SERVO_POSITION ||
				cc_number == MIDI_CC_OM2_SERVO_POSITION ||
				cc_number == MIDI_CC_OM3_SERVO_POSITION ||
				cc_number == MIDI_CC_OM4_SERVO_POSITION ||
				cc_number == MIDI_CC_OM5_SERVO_POSITION ||
				cc_number == MIDI_CC_OM6_SERVO_POSITION ){
		//manually set the servo position regardless of if servos are enabled or disabled:
		//the 0-127 MIDI cc_value range works with the set_servos() function of the heads:
		oms[head].set_servos(cc_value);
		#ifdef MIDI_DEBUG
			Serial.print("Head ");
			Serial.print(head+1);
			Serial.print(": servo position set to ");
			Serial.println(cc_value);
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
				#ifdef MIDI_DEBUG
					Serial.println("All Notes/Sound Off.");
				#endif
				return;
			}
		} else if(cc_number == MIDI_CC_SERVO_ENABLE){
			//this will disable servo movements:
			//enable for any value other than 0
			if(cc_value){
				oms[h].enable_servos();
				//only return on the last head
				if(h == (NUM_OMIDITONES-1)){
					#ifdef MIDI_DEBUG
						Serial.println("Servos Enabled");
					#endif
					return;
				}
			} else {
				oms[h].disable_servos();
				//only return on the last head
				if(h == (NUM_OMIDITONES-1)){
					#ifdef MIDI_DEBUG
						Serial.println("Servos Disabled");
					#endif
					return;
				}
			}
		} else if(cc_number == MIDI_CC_PITCH_CORRECTION_ENABLE){
			//this will disable pitch correction:
			//enable for any value other than 0
			if(cc_value){
				oms[h].enable_pitch_correction();
				if(h == (NUM_OMIDITONES-1)){
					#ifdef MIDI_DEBUG
						Serial.println("Pitch Correction Enabled");
					#endif
					return;
				}
			} else {
				oms[h].disable_pitch_correction();
				if(h == (NUM_OMIDITONES-1)){
					#ifdef MIDI_DEBUG
						Serial.println("Pitch Correction Disabled");
					#endif
					return;
				}
			}
		} else if(cc_number == MIDI_CC_RESET_ALL_CONTROLLERS){
			//this resets all notes and pitch bend to default values on each head
			for(int i=0; i<16; i++){
				oms[h].set_pitch_bend(DEFAULT_PITCH_BEND, i);
				oms[h].set_max_pitch_bend(DEFAULT_MIDI_PITCH_BEND_SEMITONES, DEFAULT_MIDI_PITCH_BEND_CENTS);
			}
			oms[h].note_off(oms[h].currently_playing_note());
			oms[h].update();
			if(h == (NUM_OMIDITONES-1)){
				#ifdef MIDI_DEBUG
					Serial.println("CC121: Reset to Defaults");
				#endif
				return;
			}
		}
	}// head for loop
	//if it makes it here without returning, note that nothing happened.
	#ifdef MIDI_DEBUG
		Serial.print("CC");
		Serial.print(cc_number);
		Serial.println(" Unassigned");
	#endif
}