/*
  Synth.cpp - Library for tone() with 8-bit volume control and multiple wave shapes.
  Created by Connor Nishijima, September 7th 2017.
  Released under the GPLv3 license.
*/

#include "synth.h"
#include "TimerOne.h"

uint8_t speaker = 9;
volatile uint8_t arr[200];
volatile uint8_t arr_temp[200];
const uint8_t sine_table[200] PROGMEM = {0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7, 9, 10, 12, 14, 15, 17, 20, 22, 24, 26, 29, 32, 34, 37, 40, 43, 46, 49, 52, 56, 59, 62, 66, 69, 73, 77, 80, 84, 88, 92, 96, 100, 103, 107, 111, 115, 119, 123, 128, 128, 132, 136, 140, 144, 148, 152, 155, 159, 163, 167, 171, 175, 178, 182, 186, 189, 193, 196, 199, 203, 206, 209, 212, 215, 218, 221, 223, 226, 229, 231, 233, 235, 238, 240, 241, 243, 245, 246, 248, 249, 250, 251, 252, 253, 254, 254, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 250, 248, 247, 246, 244, 242, 241, 239, 237, 234, 232, 230, 227, 225, 222, 219, 217, 214, 211, 208, 204, 201, 198, 194, 191, 187, 184, 180, 177, 173, 169, 165, 161, 157, 153, 150, 146, 142, 138, 134, 130, 125, 121, 117, 113, 109, 105, 102, 98, 94, 90, 86, 82, 78, 75, 71, 68, 64, 61, 57, 54, 51, 47, 44, 41, 38, 36, 33, 30, 28, 25, 23, 21, 18, 16, 14, 13, 11, 9, 8, 7, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0};

volatile bool arr_update = false;
volatile uint8_t arr_index = 0;
volatile uint8_t arr_length = 1;
volatile uint8_t arr_temp_length = 1;

volatile bool playing = false;
volatile bool halt = false;

volatile uint8_t divisor = 1;
volatile uint8_t divisor_temp = 1;
volatile uint8_t tick = 0;
	
uint8_t wave_shape = 0;

Synth::Synth(uint8_t pin){
	speaker = pin;
}

void Synth::tone_type(uint8_t tt){
  wave_shape = tt;
}

void Synth::stop(){
  halt = true;
}

void generate_random_wave_shape(uint32_t seed, uint8_t tone_vol) {
  randomSeed(seed);
  int16_t amp = 0;
  uint8_t max_amp = 0;
  uint8_t amp_dir = random(0, 2);
  float path_length = 0;
  float fade = 1;

  uint8_t shift_chance = random(5, 30);
  uint8_t amp_push = random(2, 4);
  float path_shape = random(2, 50) / 10.0;

  for (uint8_t i = 0; i < 200; i++) {
    if (i < 50) {
      fade = i / 50.0;
    }
    else if (i > 150) {
      fade = 1 - ((i - 150) / 50.0);
    }
    else {
      fade = 1;
    }

    if (random(0, 100) < shift_chance) {
      amp_dir = !amp_dir;
      path_length = 0;
      shift_chance = random(5, 30);
      amp_push = random(2, 4);
      path_shape = random(2, 50) / 10.0;
    }

    if (amp_dir == 1) {
      amp += random(0, amp_push) * path_length;
    }
    else if (amp_dir == 0) {
      amp -= random(0, amp_push) * path_length;
    }

    path_length += path_shape;

    amp = constrain(amp*fade, 0, 255);

    if (amp > max_amp) {
      max_amp = amp;
    }

    arr_temp[i] = amp;
  }

  float amplify = 255 / float(max_amp);
  
  float float_vol = tone_vol/255.0;

  for (uint8_t i = 0; i < 200; i++) {
    arr_temp[i] = arr_temp[i] * (amplify) * float_vol;
  }
}

void update_array() {
  arr_update = false;
  for (uint8_t i = 0; i < arr_temp_length; i++) {
    arr[i] = arr_temp[i];
  }
  arr_length = arr_temp_length;
  divisor = divisor_temp;
  arr_index = 0;
  tick = 0;
}

void build_square(uint16_t hz, uint8_t tone_vol) {
  byte half_length = arr_temp_length / 2;

  for (byte i = 0; i < arr_temp_length; i++) {
    if (i < half_length) {
      arr_temp[i] = 0;
    }
    else {
      arr_temp[i] = tone_vol;
    }
  }
}

void build_saw(uint16_t hz, uint8_t tone_vol) {
  for (byte i = 0; i < arr_temp_length; i++) {
    float progress = i / float(arr_temp_length);
    arr_temp[i] = tone_vol * progress;
  }
}

void build_tri(uint16_t hz, uint8_t tone_vol) {
  volatile byte half_length = arr_temp_length / 2;

  for (byte i = 0; i < arr_temp_length; i++) {
    volatile float progress = i / float(arr_temp_length);
    if (i <= half_length) {
      arr_temp[i] = tone_vol * (progress * 2);
    }
    else {
      arr_temp[i] = tone_vol * ((1 - progress) * 2);
    }
  }
}

void build_sine(uint16_t hz, uint8_t tone_vol) {
  uint8_t half_vol = tone_vol / 2;
  float vol_float = tone_vol / 255.0;

  for (byte i = 0; i < arr_temp_length; i++) {
    float progress = i / float(arr_temp_length);
    arr_temp[i] = pgm_read_byte_near(sine_table + (uint8_t((200 * progress)))) * vol_float;
  }
}

void cycle() {
  tick++;
  if (tick == divisor) {
    tick = 0;
    uint16_t level = arr[arr_index] * 4;
    Timer1.setPwmDuty(speaker, level);

    arr_index++;
    if (arr_index == arr_length - 1) {
      arr_index = 0;
      if (arr_update == true) {
        update_array();
      }
      if (halt == true) {
        halt = false;
        playing = false;
        Timer1.disablePwm(speaker);
        Timer1.detachInterrupt();
        Timer1.stop();
      }
    }
  }
}

void Synth::tone(uint16_t hz, uint8_t tone_vol) {
  if (arr_update == false) {
    float tuned = 125 * TUNING;

    if (hz <= tuned / 4) {
      divisor_temp = 8;
      hz *= 8;
    }
	if (hz <= tuned / 2) {
      divisor_temp = 4;
      hz *= 4;
    }
    else if (hz <= tuned) {
      divisor_temp = 2;
      hz *= 2;
    }

    arr_temp_length = 200 * (tuned / float(hz));

    if (wave_shape == SQUARE_WAVE) {
      build_square(hz, tone_vol);
    }
    else if (wave_shape == SAWTOOTH_WAVE) {
      build_saw(hz, tone_vol);
    }
    else if (wave_shape == TRIANGLE_WAVE) {
      build_tri(hz, tone_vol);
    }
    else if (wave_shape == SINE_WAVE) {
      build_sine(hz, tone_vol);
    }

    arr_update = true;

    if (playing == false) {
      playing = true;
	  divisor_temp = 1;
      update_array();
      Timer1.initialize(40);
      Timer1.pwm(speaker, 0);
      Timer1.setPwmDuty(speaker, 0);
      Timer1.attachInterrupt(cycle);
    }
  }
}

void Synth::tone_custom(uint16_t hz, uint8_t tone_vol, uint8_t tone_arr[]) {
  if (arr_update == false) {
    float tuned = 125 * TUNING;

    if (hz <= tuned / 2) {
      divisor_temp = 4;
      hz *= 4;
    }
    else if (hz <= tuned) {
      divisor_temp = 2;
      hz *= 2;
    }

    arr_temp_length = 200 * (tuned / float(hz));

	float vol_float = tone_vol / 255.0;

    for (byte i = 0; i < arr_temp_length; i++) {
      float progress = i / float(arr_temp_length);
      arr_temp[i] = tone_arr[uint8_t((200 * progress))] * vol_float;
    }

    arr_update = true;

    if (playing == false) {
      playing = true;
      update_array();
      Timer1.initialize(40);
      Timer1.pwm(speaker, 0);
      Timer1.setPwmDuty(speaker, 0);
      Timer1.attachInterrupt(cycle);
    }
  }
}

void Synth::tone_seeded(uint16_t hz, uint8_t tone_vol, uint16_t seed) {
  if (arr_update == false) {
    float tuned = 125 * TUNING;

    if (hz <= tuned / 2) {
      divisor_temp = 4;
      hz *= 4;
    }
    else if (hz <= tuned) {
      divisor_temp = 2;
      hz *= 2;
    }

    arr_temp_length = 200 * (tuned / float(hz));

	float vol_float = tone_vol / 255.0;

    generate_random_wave_shape(seed, tone_vol);

    arr_update = true;

    if (playing == false) {
      playing = true;
      update_array();
      Timer1.initialize(40);
      Timer1.pwm(speaker, 0);
      Timer1.setPwmDuty(speaker, 0);
      Timer1.attachInterrupt(cycle);
    }
  }
}

void Synth::manual(uint16_t level) {
	if (playing == false) {
		playing = true;
		Timer1.initialize(40);
		Timer1.pwm(speaker, 0);
	}
	Timer1.setPwmDuty(speaker, level);
}