//this is a quick function to test the MCP4151 chips I am using on the oMIDItone.
//It iterates through both chips using the functions from the oMIDItone class so I can make sure the hardware is working.

#include <SPI.h>

 //Declare variables:

 //the pins are thus:
uint16_t optoisolator_pin = 15;
uint16_t cs1_pin = 17;
uint16_t cs2_pin = 16;
uint16_t output_pin = 26;

 //this is how many resistance steps can be used with the digital pots. The current hardware has 2 digital pots with 256 steps each, for a total of 512.
#define NUM_RESISTANCE_STEPS 768

void setup(){
     //start SPI
    SPI.begin();
     //pin modes
    pinMode(cs1_pin, OUTPUT);
    pinMode(cs2_pin, OUTPUT);
    pinMode(optoisolator_pin, OUTPUT);
    pinMode(output_pin, OUTPUT);
     //pin set high to disable SPI until needed.
    digitalWrite(cs1_pin, HIGH);
    digitalWrite(cs2_pin, HIGH);
    digitalWrite(optoisolator_pin, HIGH);
    digitalWrite(output_pin, HIGH);
}

void loop(){
     //some test values to make sure everything works right:
    set_pot(cs1_pin, 0);
    set_pot(cs2_pin, 0);
    delay(5000);
    set_pot(cs1_pin, 255);
    set_pot(cs2_pin, 0);
    delay(5000);
    set_pot(cs1_pin, 255);
    set_pot(cs2_pin, 255);
    delay(5000);
     //iterate through all steps slowly and set resistances:
    for(int i=0; i<NUM_RESISTANCE_STEPS; i++){
        set_resistance(i);
        delay(20);
    }
}

 //this will set the CS_pin digital pot's wiper to a value based on byte 1 and byte 2
void set_pot(uint16_t CS_pin, uint16_t command_byte){
  digitalWrite(CS_pin, LOW); //select chip
  uint16_t byte_high = command_byte >> 8;
  uint16_t byte_low = command_byte & 0xff;
  SPI.transfer(byte_high); //send command first
  SPI.transfer(byte_low); //send value second
  digitalWrite(CS_pin, HIGH); //de-select chip when done
}

 //this will take a uint16_t number and set the total resistance value to between 0 and 768 on the board.
 //Lots of hard coded values here, should probably fix later, but oh well.
void set_resistance(uint16_t resistance){
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
