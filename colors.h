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
const uint32_t red = Color(255, 0, 0);
const uint32_t orange = Color(255, 127, 0);
const uint32_t yellow = Color(255, 255, 0);
const uint32_t yellow_green = Color(127, 255, 0);
const uint32_t green = Color(0, 255, 0);
const uint32_t green_blue = Color(0, 255, 127);
const uint32_t sky_blue = Color(0, 255, 255);
const uint32_t deep_blue = Color(0, 127, 255);
const uint32_t blue = Color(0, 0, 255);
const uint32_t purple_blue = Color(127, 0, 255);
const uint32_t purple = Color(255, 0, 255);
const uint32_t dark_purple = Color(255, 0, 127);

const uint32_t white = Color(255,255,255);
const uint32_t off = Color(0, 0, 0);

//a couple 'rainbows' for simple on/off functionality.
const rainbow r_off = {
  .colors = {
    off
  },
  .num_colors = 1
};

const rainbow r_on = {
  .colors = {
    white
  },
  .num_colors = 1
};

//recommended for use with LC_FG_VU_METER modes on a LightingControl setup, but not required.
//Adjust the ratio of green to yellow to red to your liking.
const rainbow r_vu = {
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
const rainbow r1 = {
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
const rainbow r2 = {
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
const rainbow r3 = {
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
const rainbow r4 = {
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
const rainbow r5 = {
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
const rainbow r6 = {
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
const rainbow r7 = {
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
const rainbow r8 = {
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
const rainbow r9 = {
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
const rainbow r10 = {
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
const rainbow r11 = {
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

const rainbow om1_rb = {
  .colors = {
    red, 
    Color(64,0,0), 
    red, 
    Color(64,0,0),
    red, 
    Color(64,0,0),
    red, 
    Color(64,0,0),
    red 
  },
  .num_colors = 9
};

const rainbow om2_rb = {
  .colors = {
    yellow, 
    Color(64,64,0),
    yellow, 
    Color(64,64,0),
    yellow,
    Color(64,64,0),
    yellow,
    Color(64,64,0),
    yellow 
  },
  .num_colors = 9
};

const rainbow om3_rb = {
  .colors = {
    sky_blue, 
    Color(0,64,64),
    sky_blue, 
    Color(0,64,64),
    sky_blue, 
    Color(0,64,64),
    sky_blue, 
    Color(0,64,64),
    sky_blue 
  },
  .num_colors = 9
};

const rainbow om4_rb = {
  .colors = {
    purple, 
    Color(64,0,64),
    purple, 
    Color(64,0,64),
    purple, 
    Color(64,0,64),
    purple, 
    Color(64,0,64),
    purple 
  },
  .num_colors = 9
};

const rainbow om5_rb = {
  .colors = {
    green, 
    Color(0,64,0),
    green, 
    Color(0,64,0),
    green, 
    Color(0,64,0),
    green, 
    Color(0,64,0),
    green 
  },
  .num_colors = 9
};

const rainbow om6_rb = {
  .colors = {
    blue, 
    Color(0,0,64),
    blue, 
    Color(0,0,64),
    blue, 
    Color(0,0,64),
    blue, 
    Color(0,0,64),
    blue 
  },
  .num_colors = 9
};

const rainbow marquee_rb = {
  .colors = {
    Color(255,255,255),
    Color(64,64,64)
  },
  .num_colors = 2
};

const rainbow trigger_rb = {
  .colors = {
    Color(255,255,255),
    Color(64,64,64)
  },
  .num_colors = 2
};

#endif