

uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(int wheelPos) {
  uint8_t pos = wheelPos % 255;
  if(pos < 85) {
    return Color(pos * 3, 255 - pos * 3, 0);
  }
  else if(pos < 170) {
    pos -= 85;
    return Color(255 - pos * 3, 0, pos * 3);
  }
  else {
    pos -= 170;
    return Color(0, pos * 3, 255 - pos * 3);
  }
}

uint32_t dim(uint32_t color, uint8_t bright_r, uint8_t bright_g, uint8_t bright_b) {
  uint32_t r = map((color >> 16 & 0xFF), 0, 255, 0, bright_r);
  uint16_t g = map((color >> 8  & 0xFF), 0, 255, 0, bright_g);
  uint16_t b = map((color & 0xFF), 0, 255, 0, bright_b);
  return (r << 16) + (g << 8) + b;
}

uint32_t dim(uint32_t color, uint8_t b) {
  return dim(color, b, b, b);
}


