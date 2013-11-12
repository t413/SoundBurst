#ifndef _beats_h
#define _beats_h

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif


void p(char *fmt, ... );

class BeatDetector {

 public:

   BeatDetector();

   uint8_t beatDetect(uint8_t magnitude, uint16_t current_time);

   uint8_t debug;
   uint8_t mag;
   int16_t fvalue;
   uint16_t beat;
   uint16_t beat_length;
   uint16_t last_beat;
   int8_t beat_quality;

 private:

   int8_t rhythmic();

   uint8_t last_mag;

   uint8_t beat_status;
   uint16_t beat_start;
   uint16_t last_beat_start;
   uint16_t beat_end;
};

#endif