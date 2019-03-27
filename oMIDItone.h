/*
This is the oMIDItone library, to send commands to a pair of MCP4151 50k digital 
pots to change the tone on an otamatone. 
It also has a pin for turning the sound on and off, and a pin for 
analog readings to correct the frequency being played.
*/

#ifndef oMIDItone_h
#define oMIDItone_h

#include <Arduino.h>

//We will need the SPI library to communicate with the MCP4151 chips.
#include <SPI.h>

//We will also use the fancy faster ADC library from pedvide for Teensy:
#include "ADC-master/ADC.h"

//comment this to disable serial functions for testing notes.
//This will send debug messages over the USB serial port if it is defined.
//It should start the Serial if it is not yet running in the init function.
#ifndef DEBUG
  #define DEBUG
#endif

//This is a non-valid note number for MIDI to designate no note should be played.
#define NO_NOTE 128

//This is a non-valid resistance value to denote that the note should not be played.
#define NO_RESISTANCE -1

//This is the frequency of the note A - typically 440 Hz.
#define NOTE_A 440

//This is how many MIDI notes there are. It should always be 127
#define NUM_MIDI_NOTES 128

//this is how many resistance steps can be used with the digital pots. The current hardware has 2 digital pots with 256 steps each, for a total of 512.
#define NUM_RESISTANCE_STEPS 512

//This is the number of rising edges to read before computing a new current average frequency.
#define NUM_FREQ_READINGS 4

//this defines how long to test each note until it is stabilized in the strtup test in ms
#define STABILIZATION_TIME 4000

//This is the analog read threshold for a rising edge to count the frequency.
#define RISING_EDGE_THRESHOLD 50

//this is a jitter value to randomize the resistance in an attempt to counter the frequency variation around a specific resistance value. it is measured in resistance steps
#define JITTER 10

//this is the % difference that a note can be off to trigger correction, as a number from 0-100
#define ALLOWABLE_NOTE_ERROR 2

//this is to make sure that the rising edge isn't measured too often (in us):
#define MIN_TIME_BETWEEN_RISING_EDGE_MEASUREMENTS 1

//Time to wait between receiving a note and starting to play that note (in ms).
#define NOTE_WAIT_TIME 10

//this is to make sure frequency corrections are not too frequent (in us):
#define TIME_BETWEEN_FREQUENCY_CORRECTIONS 20

//This is how long to wait for initial frequency readings on init before declaring failure and marking the object as unavailable in ms
#define TIME_TO_WAIT_FOR_INIT 100

//this is the center value for pitch shifting - if the pitch_shift vlaue is set to this, it will play the note without any pitch bending.
//Teensy MIDI sends a number form -8192 to 8191, so I am using 0 as the center here.
#define CENTER_PITCH_SHIFT 0

class oMIDItone {
  public:
    //constructor function
    oMIDItone(uint16_t relay, uint16_t cs1, uint16_t cs2, uint16_t feedback);

    //this will init the pin modes and set up Serial if it's not already running.
    void init();

    //This should be called during the loop, and it will update the note frequencies and play notes as needed.
    void update();

    //This returns the note number that is currently playing or NO_NOTE if no note is currently playing.
    uint16_t currently_playing_note();

    //this returns the current pitch shift value
    uint16_t currently_playing_pitch_shift();

    //This will return true if init was successful AND there is no current note playing.
    bool is_running();

    //This will check if a frequency can be played by an initialized oMIDItone object.
    bool can_play_note(uint16_t note);

    //This will tell the oMIDItone to play the note. The note will continue to play until changed or until set to off.
    //note is a MIDI note number. If the note is out of the init value oMIDItone range, it will not play anything.
    void note_on(uint16_t note);

    //This will set the oMIDItone to stop playing all notes.
    void note_off(uint16_t note);

    //this will set the pitch shift value. This will apply to any notes played on the oMIDItone.
    void set_pitch_shift(int16_t pitch_shift_value);

  private:
    //this will set the CS_pin digital pot's wiper to a value based on byte 1 and byte 2
    void set_pot(uint16_t CS_pin, uint16_t command_byte);

    //this will take a uint16_t number and set the total resistance value to between 0 and 512 on the board.
    void set_resistance(uint16_t resistance);

    //This will constantly read the analog input and return true when it detects a rising edge signal.
    //It also updates the current and last rising edge time values.
    bool is_rising_edge();

    //a simple averaging function:
    uint32_t average(unsigned long * array, uint16_t num_elements);

    //this introduces jittered resistance settings, and should be called every loop to keep the jitter working:
    void set_jitter_resistance(uint16_t resistance, uint16_t jitter);

    //This will play from 0 resistance value to 512 resistance value and note which resistances
    //correspond to which notes in the note matrix:
    //Returns true if init was successful and false if it was not.
    bool startup_test();

    //this takes the frequency averaging code and puts it into a function to clean up the update function:
    void measure_frequency();

    //This is a function that will change the current_resistance to a different value if it is too far off from the current_frequency.
    void adjust_frequency();

    //This adjusts a frequency to a pitch-shifted value from the base note. 
    //Pitch shift can move uo to two MIDI notes away depending on value of the pitch shift variable.
    uint32_t pitch_adjusted_frequency(uint16_t note, uint16_t pitch_shift);

    //this function will find a resistance value that was measured as being very near the desired frequency.
    uint16_t frequency_to_resistance(uint16_t frequency);

    //This will be set to true if the startup_test was successful:
    bool had_successful_init;

    //This is a variable that tells the program when to check for a note value when the note is on.
    bool check_note_value;

    //current_note is the number of the MIDI note value that is being played.
    uint16_t current_note;

    //this will be set during the startup test to the lowest note registered.
    uint16_t min_note;

    //this will be set during the startup test to the highest note registered.
    uint16_t max_note;

    //This is an array of MIDI notes and the frequency they correspond to. Turns out it is not needed.
    //const float midi_Hz_freqs[NUM_MIDI_NOTES] = {8.176, 8.662, 9.177, 9.723, 10.301, 10.913, 11.562, 12.25, 12.978, 13.75, 14.568, 15.434, 16.352, 17.324, 18.354, 19.445, 20.602, 21.827, 23.125, 24.5, 25.957, 27.5, 29.135, 30.868, 32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 55, 58.27, 61.735, 65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.826, 110, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440, 466.164, 493.883, 523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880, 932.328, 987.767, 1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760, 1864.655, 1975.533, 2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 3520, 3729.31, 3951.066, 4186.009, 4434.922, 4698.636, 4978.032, 5274.041, 5587.652, 5919.911, 6271.927, 6644.875, 7040, 7458.62, 7902.133, 8372.018, 8869.844, 9397.273, 9956.063, 10548.08, 11175.3, 11839.82, 12543.85};

    //This is an array that has converted the midi_freqs_Hz array into an array of integers representing us between rising edges for the note frequencies
    const uint32_t midi_freqs[NUM_MIDI_NOTES] = {122309, 115446, 108968, 102848, 97077, 91633, 86490, 81632, 77053, 72727, 68643, 64792, 61154, 57723, 54484, 51427, 48538, 45814, 43243, 40816, 38525, 36363, 34322, 32396, 30578, 28861, 27242, 25712, 24270, 22907, 21622, 20408, 19262, 18181, 17161, 16198, 15289, 14430, 13621, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506, 477, 450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238, 225, 212, 200, 189, 178, 168, 159, 150, 142, 134, 126, 119, 112, 106, 100, 94, 89, 84, 79};

    //this is an array of the most recent measured rising edge average times in us that correspond to a resistance
    uint32_t measured_freqs[NUM_RESISTANCE_STEPS];

    //This is an array of the last NUM_FREQ_READINGS frequency readings for averaging purposes.
    uint32_t recent_freqs[NUM_FREQ_READINGS];

    //this is an index that will count up to NUM_FREQ_READINGS and reset to 0, triggering a frequency measurement average.
    uint16_t freq_reading_index;

    //This is a variable for storing the most recent frequency reading based on the average of the last NUM_FREQ_READINGS readings
    uint32_t current_freq;

    //this is to measure the frequency of rising edges produced by the output sound wave
    elapsedMicros last_rising_edge;

    //this is to make sure the frequency correction isn't happening faster than the digital pots can be set
    elapsedMillis last_adjust_time;

    //this is to make sure the frequency correction isn't happening faster than the digital pots can be set
    elapsedMillis last_stabilize_time;

    //This is a time tracker that will force a short pause between notes.
    elapsedMillis note_start_time;

    //this is a variable for globally storing the most recent analog reading
    uint16_t last_analog_read;

    //this tracks the current pitch shift for notes.
    uint16_t current_pitch_shift;

    //variable for saving the current resistance value of the digital pots.
    uint16_t current_resistance;

    //pin number tracking:
    uint16_t relay_pin;
    uint16_t cs1_pin;
    uint16_t cs2_pin;
    uint16_t analog_feedback_pin;

    //set up an instanced ADC object for use in the code.
    ADC *adc = new ADC();

};

#endif
