/*
   Synth || Wave Shapes Example

   This sketch demonstrates the 4 wave shapes available to you!
   They are defined as these constants:

      SQUARE_WAVE
      SAWTOOTH_WAVE
      TRIANGLE_WAVE
      SINE_WAVE

   and are set with syn.tone_type(WAVE_SHAPE).

   You will hear 700Hz tones fading to silence in the four wave shapes!
*/

#include "Synth.h"
Synth syn(9);

void setup() {
}

void loop() {
  syn.tone_type(SQUARE_WAVE);
  for (byte volume = 255; volume > 0; volume--) {
    syn.tone(700, volume);
    delay(5);
  }
  syn.stop();
  delay(500);

  syn.tone_type(SAWTOOTH_WAVE);
  for (byte volume = 255; volume > 0; volume--) {
    syn.tone(700, volume);
    delay(5);
  }
  syn.stop();
  delay(500);

  syn.tone_type(TRIANGLE_WAVE);
  for (byte volume = 255; volume > 0; volume--) {
    syn.tone(700, volume);
    delay(5);
  }
  syn.stop();
  delay(500);

  syn.tone_type(SINE_WAVE);
  for (byte volume = 255; volume > 0; volume--) {
    syn.tone(700, volume);
    delay(5);
  }
  syn.stop();
  delay(1000);
}
