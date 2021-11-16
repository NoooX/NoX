void colorWipe(uint32_t cl, uint8_t wait) {
  uint8_t i;
  
  for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, cl);
      strip2.setPixelColor(i, cl);
      strip.show();
      strip2.show();
      delay(wait);
  }
}

void colorWipeRev(uint32_t cl, uint8_t wait) {
  int8_t i;
   
  for (i = strip.numPixels()-1; i >= 0; i--) {
      strip.setPixelColor(i, cl);
      strip2.setPixelColor(i, cl);
      strip.show();
      strip2.show();
      delay(wait);
  }
}
