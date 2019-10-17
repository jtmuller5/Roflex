//Theatre-style crawling lights.
void spinLights(int g, int b, uint8_t wait) {
  for (uint16_t i = 0; i < 8; i++) {
    strip.setPixelColor(i, 0, g, b);  //turn each pixel on in sequence

    strip.show();

    delay(wait);

    strip.setPixelColor(i, 0);        //turn every third pixel off
    strip.show();

    delay(wait / 2);
  }
}


void rotate(boolean rotateCW, int g, int b, uint8_t wait){
  if (rotateCW == true){
    for (uint16_t i = 0; i < 8; i++) {
      strip.setPixelColor(i, 0, g, b);  //turn each pixel on in sequence

      strip.show();

      delay(wait);

      strip.setPixelColor(i, 0);        //turn every third pixel off
      strip.show();

      delay(wait / 2);
    }
  }
  else if (rotateCW == false){
    for (uint16_t i = 7; i > 0; i--) {
      strip.setPixelColor(i, 0, g, b);  //turn each pixel on in sequence

      strip.show();

      delay(wait);

      strip.setPixelColor(i, 0);        //turn every third pixel off
      strip.show();

      delay(wait / 2);
    }
  }
}
