/*
   Synth || Housefly Example

   Sawtooth waves can sound like insect wings at the right frequency.
   
   By applying random movement to the volume and wiggling the frequency around,
   you can mimic the sound of an insect flying around your ears.
*/

#include "Synth.h"
Synth syn(9);

uint8_t fly_volume = 255;
float distance = 0;
uint16_t frequency = 150;
uint16_t f = frequency;
bool landed = false;

void setup() {
  // Change seed to produce unique performances
  randomSeed(413);
  syn.tone_type(SAWTOOTH_WAVE);
}

void loop() {

  if(random(0,100) <= 1){
    landed = !landed;
  }
  
  float push = random(0,6)/100.0;
  if(!landed){
    distance += push;
    if(distance > 0.0){
      distance -= 0.1*(0.1+distance/1.0);
    }
    distance = constrain(distance,0.05,1.0);
  }
  else{
    distance -= 0.01;
    distance = constrain(distance,0.00,1.0);
  }

  f+=random(-1,2);
  f = constrain(f,frequency,frequency+20);

  uint8_t volume = fly_volume*distance;
  
  syn.tone(f+(20*distance),volume); 
  delay(4);
}