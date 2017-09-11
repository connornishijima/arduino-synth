#include "Synth.h"
Synth syn(9);

void setup() {
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t seed = millis();
  
  syn.tone_seeded(220, 255, seed);
  delay(200);
}