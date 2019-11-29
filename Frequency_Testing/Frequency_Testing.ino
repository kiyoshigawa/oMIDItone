/*
2019-05-02 - This is a simple test program to test driving the otamatone speaker directly from a transistor driven by a teensy pin.
*/

#define MIN_TIME_BETWEEN_STATE_CHANGES 100
#define NUM_MIDI_NOTES 128
#define MAX_NOTE 128
//the EMUMIDItone is only HIGH for 1/ON_TIME_DIVISIONS of a cycle at the current frequency.
#define ON_TIME_DIVISIONS 25

int sound_pin = 2;
elapsedMicros last_rising_edge;
elapsedMicros last_state_change;

//This is an array that has converted the midi_freqs_Hz array into an array of integers representing us between rising edges for the note frequencies
const uint32_t midi_freqs[NUM_MIDI_NOTES] = {122309, 115446, 108968, 102848, 97077, 91633, 86490, 81632, 77053, 72727, 68643, 64792, 61154, 57723, 54484, 51427, 48538, 45814, 43243, 40816, 38525, 36363, 34322, 32396, 30578, 28861, 27242, 25712, 24270, 22907, 21622, 20408, 19262, 18181, 17161, 16198, 15289, 14430, 13621, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506, 477, 450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238, 225, 212, 200, 189, 178, 168, 159, 150, 142, 134, 126, 119, 112, 106, 100, 94, 89, 84, 79};


void setup(){
    pinMode(sound_pin, OUTPUT);
    last_rising_edge = 0;
    last_state_change = 0;
}

void loop(){
    int current_freq = midi_freqs[60];
    //make sure all digital writes are inside this if statement, or the speakers will destroy themselves from updating too fast.
    if(last_state_change > MIN_TIME_BETWEEN_STATE_CHANGES){
        if(last_rising_edge > current_freq){ //reset
            last_rising_edge = last_rising_edge - current_freq;
        }
        else if((last_rising_edge > 0) && (last_rising_edge < (current_freq/ON_TIME_DIVISIONS))){
            digitalWrite(sound_pin, HIGH);
            last_state_change = 0;
        }
        else {
            digitalWrite(sound_pin, LOW);
            last_state_change = 0;
        }
    }
}