//This is just to offload color definitions to another file so I don't need to scroll so much in the main file.

#ifndef colors_h
#define colors_h

#include <Arduino.h>

//this is the most colors a single rainbow can have
#define MAX_RAINBOW_COLORS 20

//a structure for holding rainbows called rainbow:
struct rainbow {
  uint32_t colors[MAX_RAINBOW_COLORS];
  uint8_t num_colors;
};

//simple function for converting RGB into uint32_t used by Adafruit_NeoPixel
uint32_t Color(uint8_t r, uint8_t g, uint8_t b);

//this is a general purpose function for mapping a color transition between two colors (uint32_t) based on a desired position, a starting position, and an ending position.
uint32_t color_map(int x, int in_min, int in_max, uint32_t start_color, uint32_t end_color);

//named color definitions:
const uint32_t PROGMEM red = Color(255, 0, 0);
const uint32_t PROGMEM orange = Color(255, 127, 0);
const uint32_t PROGMEM yellow = Color(255, 255, 0);
const uint32_t PROGMEM yellow_green = Color(127, 255, 0);
const uint32_t PROGMEM green = Color(0, 255, 0);
const uint32_t PROGMEM green_blue = Color(0, 255, 127);
const uint32_t PROGMEM sky_blue = Color(0, 255, 255);
const uint32_t PROGMEM deep_blue = Color(0, 127, 255);
const uint32_t PROGMEM blue = Color(0, 0, 255);
const uint32_t PROGMEM purple_blue = Color(127, 0, 255);
const uint32_t PROGMEM purple = Color(255, 0, 255);
const uint32_t PROGMEM dark_purple = Color(255, 0, 127);

const uint32_t PROGMEM white = Color(255,255,255);
const uint32_t PROGMEM off = Color(0, 0, 0);

//a couple 'rainbows' for simple on/off functionality.
const rainbow PROGMEM r_off = {
  .colors = {
    off
  },
  .num_colors = 1
};

const rainbow PROGMEM r_on = {
  .colors = {
    white
  },
  .num_colors = 1
};

//recommended for use with LC_FG_VU_METER modes on a LightingControl setup, but not required.
//Adjust the ratio of green to yellow to red to your liking.
const rainbow PROGMEM r_vu = {
  .colors = {
    green,
    green,
    green,
    green,
    green,
    green,
    yellow,
    yellow,
    red
  },
  .num_colors = 9
};

//these are the various 'rainbows' that can be swapped between for color selection on rainbow functions
//rainbow1 is the traditional roygbiv rainbow pattern
const rainbow PROGMEM r1 = {
  .colors = {
    red,
    yellow,
    green,
    sky_blue,
    blue,
    purple
  },
  .num_colors = 6
};

//rainbow r2 is a double rainbow of r1
const rainbow PROGMEM r2 = {
  .colors = { 
    red,
    yellow,
    green,
    sky_blue,
    blue,
    purple,
    red,
    yellow,
    green,
    sky_blue,
    blue,
    purple
  },
  .num_colors = 12
};

//the primary colors red, blue and yellow:
const rainbow PROGMEM r3 = {
  .colors = { 
    red,
    off,
    yellow, 
    off,
    blue,
    off
  },
  .num_colors = 6
};

//the secondary colors orange, green and purple:
const rainbow PROGMEM r4 = {
  .colors = { 
    off,
    orange,
    off,
    green, 
    off,
    purple
  },
  .num_colors = 6
};

//red green and blue
const rainbow PROGMEM r5 = {
  .colors = { 
    off,
    red,
    off,
    green, 
    off,
    blue
  },
  .num_colors = 6
};

//blue and yellow
const rainbow PROGMEM r6 = {
  .colors = { 
    off,
    yellow,
    off,
    blue,
    off,
    yellow,
    off,
    blue
  },
  .num_colors = 8
};

//red and sky_blue
const rainbow PROGMEM r7 = {
  .colors = { 
    off,
    red,
    off,
    sky_blue,
    off,
    red,
    off,
    sky_blue
  },
  .num_colors = 8
};

//Orange and deep_blue
const rainbow PROGMEM r8 = {
  .colors = { 
    off,
    orange,
    off,
    deep_blue,
    off,
    orange,
    off,
    deep_blue
  },
  .num_colors = 8
};

//blue and white pattern
const rainbow PROGMEM r9 = {
  .colors = { 
    white,
    sky_blue,
    blue,
    deep_blue,
    white,
    sky_blue,
    blue,
    deep_blue
  },
  .num_colors = 8
};

//red and white pattern
const rainbow PROGMEM r10 = {
  .colors = { 
    white,
    Color(127,0,0),
    red,
    Color(127,0,0),
    white,
    Color(127,0,0),
    red,
    Color(127,0,0)
  },
  .num_colors = 8
};

//green and white pattern
const rainbow PROGMEM r11 = {
  .colors = { 
    white,
    Color(0,127,0),
    green,
    Color(0,127,0),
    white,
    Color(0,127,0),
    green,
    Color(0,127,0)
  },
  .num_colors = 8
};
#endif