/*
  Synth | Seeded Tones Example
  
  Synth has a peculiar function built in. Given a number as an input seed, it can generate a
  random wave shape for your tone! This can be smooth, gritty, unique or earpiercing. Give random
  numbers a try! Here a tone of 440Hz is seeded with the changing milliseconds since execution began.
*/

#include "Synth.h"
Synth syn(9);

void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t seed = millis();
  
  syn.tone_seeded(440, 255, seed);
  delay(200);
}
