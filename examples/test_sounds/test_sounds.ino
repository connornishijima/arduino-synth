/*
   Synth || Test Sounds Example

   TINNITUS:      A SINE_WAVE fading in and out at 2000Hz
   GAMEBOY:       A familar TRIANGLE_WAVE
   WOLF WHISTLE:  An sometimes unwanted advance in a SAWTOOTH_WAVE
   R2D2:          With this SQUARE_WAVE, you’ll be malfunctioning within a day, you nearsighted scrap pile.
*/

#include "Synth.h"
Synth syn(9);

void setup() {
}

void loop() {
  tinnitus();
  delay(2000);
  gameboy();
  delay(1000);
  wolf_whistle();
  delay(1000);
  R2D2();
  delay(3000);
}

void tinnitus(){
  syn.tone_type(SINE_WAVE);
  for(byte i = 0; i < 255; i++){
    syn.tone(2000,i);
    delay(3);
  }
  for(byte i = 255; i > 0; i--){
    syn.tone(2000,i);
    delay(3);
  }
  syn.stop();
}

void gameboy() {
  syn.tone_type(TRIANGLE_WAVE);
  delay(100);
  syn.tone(1000, 255); // pa
  delay(70);
  for (byte v = 255; v > 0; v--) {
    syn.tone(1950, v); // ting!
    delay(4);
  }
  syn.stop();
}

void wolf_whistle() {
  syn.tone_type(SAWTOOTH_WAVE);
  int f = 122; // starting frequency
  int v = 0;   // starting volume
  while (f < 4000) {  // slide up to 4000Hz
    syn.tone(f, v);
    v = 255 * (f / 4000.00);
    f += 5;
    //delayMicroseconds(50);
  }
  syn.stop();
  delay(150); // wait a moment
  f = 122; // starting frequency
  v = 0;   // starting volume
  while (f < 2500) { // slide up to 3000Hz
    syn.tone(f, v);
    v = 255 * (f / 2500.00);
    f += 6;
    //delayMicroseconds(250);
  }
  while (f > 125) { // slide down to 125Hz
    syn.tone(f, v);
    v = 255 * (f / 2500.00);
    f -= 6;
    //delayMicroseconds(250);
  }
  syn.stop(); // end tone production
}

void R2D2() {
  syn.tone_type(SQUARE_WAVE);
  uint16_t beeps[] = {1933, 2156, 1863, 1505, 1816, 1933, 1729, 2291};
  uint8_t buzzVols[] = {144, 180, 216, 252, 252, 252, 252, 216, 180, 144};

  for (byte i = 0; i < 8; i++) {
    beeps[i] *= 0.9;
  }
  
  int i = 9;
  for (byte x = 0; x < 6; x++) {
    int i = 9;
    while (i >= 0) {
      syn.tone(990, buzzVols[i]);
      delayMicroseconds(250);
      syn.tone(990, buzzVols[i] / 8);
      delayMicroseconds(225);
      i--;
    }
  }

  delay(50);

  i = 0;
  while (i < 8) {
    int v = 0;
    while (v < 250) { // 12.5 mS fade up time
      syn.tone(beeps[i], v);
      v += 1;
      delayMicroseconds(25);
    }
    delay(16);
    v = 250;
    while (v > 0) { // 12.5 mS fade down time
      syn.tone(beeps[i], v);
      v -= 1;
      delayMicroseconds(25);
    }
    syn.stop();
    delay(28);
    i++;
  }

  int f = 2219;
  while (f < 2542) {
    Serial.println(f);
    syn.tone(f, 255);
    f += 2;
    delayMicroseconds(100);
  }
  f = 2542;
  int v = 255;
  while (f > 1400) {
    syn.tone(f, v);
    f -= 6;
    v -= 1;
    delay(1);
  }
  syn.stop();
  
  delay(15);

  for (byte x = 0; x < 6; x++) {
    int i = 9;
    while (i >= 0) {
      syn.tone(990, buzzVols[i]);
      delayMicroseconds(250);
      syn.tone(990, buzzVols[i] / 8);
      delayMicroseconds(225);
      i--;
    }
  }
  syn.stop();
}
