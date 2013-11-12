
#include "beats.h"
#include <stdarg.h> //for p()
void p(char *fmt, ... ) {
  char tmp[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(tmp, 128, fmt, args);
  va_end (args);
  Serial.println(tmp);
}

int16_t within(int16_t beat, int16_t beat2, int16_t tolerance);

enum beat_states {
  BEAT_EQUAL,
  BEAT_DOUBLE,
  BEAT_HALF,
  BEAT_PICKUP,
  BEAT_4X,
  BEAT_X4,
};



BeatDetector::BeatDetector() { }

uint8_t BeatDetector::beatDetect(uint8_t magnitude, uint16_t current_time) {
  last_mag = mag;
  mag = magnitude;
  fvalue = (mag*10 > fvalue)? mag*10 : max(fvalue - 4, 0);
  if (!beat_status && (mag > (last_mag + 1))) { //increase-> start of beat
    beat_status = 1; //marked beginning of beat
    last_beat_start = beat_start;
    beat_start = current_time;

    last_beat = beat;
    beat = beat_start - last_beat_start;
    int8_t type = rhythmic();
    if (debug) p("beat [%i] q=%i l=%i t=%i", beat, beat_quality, beat_length, type);
    return 1;
  } else if (beat_status && (mag < last_mag)) { //decline-> end of beat
    beat_status = 0; //marked beginning of beat
    beat_end = current_time;
    beat_length = beat_start - beat_end;
  }
  return 0;
}



int8_t BeatDetector::rhythmic() {
  if (beat_quality = within(beat, last_beat, 60)) { return BEAT_EQUAL; }
  if (beat_quality = within(beat / 2, last_beat, 60)) { return BEAT_DOUBLE; }
  if (beat_quality = within(beat, last_beat / 2, 60)) { return BEAT_HALF; }
  if (beat_quality = within(beat * 3 / 4, last_beat, 50)) { return BEAT_PICKUP; }
  if (beat_quality = within(beat / 4, last_beat, 60)) { return BEAT_X4; }
  if (beat_quality = within(beat, last_beat / 4, 60)) { return BEAT_4X; }
  return -1;
}


int16_t within(int16_t beat, int16_t beat2, int16_t tolerance) {
  return max(tolerance - abs(beat - beat2), 0);
}














































