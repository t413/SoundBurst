#include "beats.h"

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(18, 3, NEO_GRB + NEO_KHZ800); // (#leds, pin, ..)
#define ADC_SPEED 6

#define LIN_OUT8 1
#define FHT_N 128
#include <FHT.h>
#define FREQS_N FHT_N/4
BeatDetector beats[FREQS_N];
BeatDetector lowBeats;

void setup() {
  Serial.begin(115200); // use the serial port
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  //TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe0; // set the adc to free running mode
  ADCSRA |= (ADC_SPEED & 0x7); //set conversion speed (0-7)
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

void loop() {
  beats[1].debug = 1;
  uint8_t intensity = 0;
  int16_t dominantRhythmFq = 0, mainPeriod = 0;
  int8_t baseColor = 0;

  while(1) {
    for (int i = 0 ; i < FHT_N ; i++) {
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5 | (ADC_SPEED & 0x7); // restart adc
      uint8_t m = ADCL, j = ADCH;
      int k = (j << 8) | m;
      fht_input[i] = (k - 0x0200) << 6; //center and scaled data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin8(); // take the output of the fht

    uint32_t current = millis();

    //rhythm recoginition and analytics
    int16_t totalf = 0, lowMag = 0;
    uint8_t beatcount = 0;
    int16_t bestBeatFq = 0, bestBeatQl = 0;
    for (uint8_t i = 0; i < FREQS_N; i++) {
      beatcount += (beats[i].beatDetect(fht_lin_out8[i], current));
      if (i < 5) { lowMag += beats[i].mag; }
      totalf += beats[i].fvalue;
      if (beats[i].beat_quality > bestBeatQl) { bestBeatFq = i; bestBeatQl = beats[i].beat_quality; }
    }
    lowBeats.beatDetect(lowMag, current);
    intensity = constrain((int16_t) intensity + (totalf - intensity) >> 3, 0, 100);
    dominantRhythmFq += (bestBeatFq - dominantRhythmFq) >> 2;
    mainPeriod = constrain((mainPeriod + (lowBeats.beat - mainPeriod) >> 4), 500, 6000);

    if (intensity < 1) { baseColor = random(0,255); }

    colorRadialArray(current, beats, &lowBeats, intensity, dominantRhythmFq, mainPeriod, baseColor);
  }
}

void colorRadialArray(uint32_t current, BeatDetector* beats, BeatDetector* lowBeats,
      uint8_t intensity, int16_t dominantRhythmFq, int16_t mainPeriod, int8_t baseColor) {

  uint8_t qfactor = constrain(lowBeats->beat_quality / 2, 0, 4); //beat quality factor

  int8_t rotational_offset[6] = {0};
  uint8_t r_cycle = ((current >> 2) % 255);
  for (uint8_t b = 0; b < 6; b++) { //each branch color offset
    rotational_offset[b] = 30 * sin((float)(r_cycle*2*PI)/255.0f + (b * 2));
    // = (roffset * 4) / (4 - qfactor); //scale down inverse of qfactor
  }

  uint16_t radial_cycle = current % mainPeriod;
  for (uint8_t r = 0; r < 3; r++) { //each radial magnitude
    int16_t radial_offset = 30 * sin((float)(radial_cycle*2*PI)/((float)mainPeriod) + (r * 2));
    for (uint8_t b = 0; b < 6; b++) { //each branch
      uint16_t pixelOffset = (radial_offset * qfactor + rotational_offset[b] * (4 - qfactor)) / 4;
      uint32_t c = Wheel(baseColor + (dominantRhythmFq >> 2) + pixelOffset);
      strip.setPixelColor(radialLed(b,r), dim(c, constrain(intensity * 2, 0, 255)));
    }
  }
  strip.show();
}

uint8_t radialLed(uint8_t branch, uint8_t pos) {
  uint8_t base = (branch < 3)? (branch * 6) : ((branch - 3) * 6 + 3);
  return (branch < 3)? ((base + 3 - 1) - pos) : (base + pos);
}








