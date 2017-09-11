/*
  Synth.h - Library for tone() with 8-bit volume control and multiple wave shapes.
  Created by Connor Nishijima, September 7th 2017.
  Released under the GPLv3 license.
*/

#ifndef synth_h
#define synth_h
#include "TimerOne.h"
#include "Arduino.h"

#define TUNING 1.020 // DEFAULT 1.020 to account for interrupt execution time, adjust if needed

// CONSTANTS FOR EASE OF USE
#define SQUARE_WAVE	0
#define SAWTOOTH_WAVE	1
#define TRIANGLE_WAVE	2
#define SINE_WAVE	3

class Synth
{
  public:
	Synth(uint8_t pin);
	void tone(uint16_t hz, uint8_t tone_vol);
	void tone_custom(uint16_t hz, uint8_t tone_vol, uint8_t tone_arr[]);
	void tone_seeded(uint16_t hz, uint8_t tone_vol, uint16_t seed);
	void tone_type(uint8_t tt);
	void manual(uint16_t level);
	void stop();
};

#endif
