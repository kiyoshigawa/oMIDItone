/* 
This list is the information for the MIDI CC messages that corresponds to its function in the code
It got too long, so I moved it to an h file
*/

//these define the background lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
//make sure to update as lighting controller is updated with new modes
#define MIDI_CCV_BG_TYPE_OFF 1
#define MIDI_CCV_BG_TYPE_SOLID 2
#define MIDI_CCV_BG_TYPE_SLOW_FADE 3
#define MIDI_CCV_BG_TYPE_RAINBOW 4
#define MIDI_CCV_BG_TYPE_RAINBOW_FIXED 5

//these define the foreground lighting modes which correspond to the MIDI note values. They should be sent as the CC Value
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
