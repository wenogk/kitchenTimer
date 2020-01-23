void ledSetup() { //setup of the adafruit neopixel library stuff
  strip.begin();
  strip.setBrightness(50); // brightness of led to 50%
  strip.show();
}

int timeDimmerIndex() {
  int current = (millis() / (1000 / 25)) % numOfRingLEDS; //this index represents the position of the led that circulates every second when the timer starts
  return current;
}
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setLEDhelperFunction(int startVal, int num, uint32_t c, bool countdownRotate) { //function that sets the led based on the, startVal as a reference point in the 25 LED array and then the num variable as the number of LED's to turn on from the reference point 
  uint32_t countdownRotateColor = strip.Color(255, 255, 255); // color of the led that circulates every second when the timer starts
  if (currentTimeLeftTotal < 10000) { //if the currentTimeLeftTotal it means that there is less than 10 seconds left in the timer
    countdownRotateColor = strip.Color(255, 0, 0); //sets the rotating led to red to indicate it is the last 10 seconds 
  }
  if (countdownRotate == true) { //if the rotating led is needed or not
    for (uint16_t i = startVal; i < (startVal + 5); i++) {

      if (i < (startVal + num)) {
        if ((timeDimmerIndex() == i) && (countdownRotate == true)) {
          strip.setPixelColor(prevLoaderLedIndex, c); 
          prevLoaderLedIndex = i;
          strip.setPixelColor(i, countdownRotateColor);
        } else {
          strip.setPixelColor(i, c);
        }
      } else if (((timeDimmerIndex() == i) && (countdownRotate == true)) && (startTimer)) {
        strip.setPixelColor(prevLoaderLedIndex, strip.Color(0, 0, 0));
        prevLoaderLedIndex = i;
        strip.setPixelColor(i, countdownRotateColor);
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0)); // switches off any leds that are supposed to be off
      }

    }
  } else {
    for (uint16_t i = startVal; i < (startVal + 5); i++) { 
      if (i < (startVal + num)) {
        strip.setPixelColor(i, c); 
      } else {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
      }
    }
  }

}


void set10SecLED(long num, bool countdownRotate) { //sets the first section of leds, the number of leds in the section is set by the num variable
  // num = constrain(num, 0, 5);
  setLEDhelperFunction(0, num, strip.Color(255, 0, 0), countdownRotate);
}

void set1MinLED(long num, bool countdownRotate) { //sets the second section of leds, the number of leds in the section is set by the num variable
  // num = constrain(num, 0, 5);
  setLEDhelperFunction(5, num, strip.Color(255, 255, 0), countdownRotate);
}

void set5MinLED(long num, bool countdownRotate) { //sets the third section of leds, the number of leds in the section is set by the num variable
  //num = constrain(num, 0, 5);
  setLEDhelperFunction(10, num, strip.Color(0, 255, 0), countdownRotate);
}

void set30MinLED(long num, bool countdownRotate) { //sets the fourth section of leds, the number of leds in the section is set by the num variable
  //num = constrain(num, 0, 5);
  setLEDhelperFunction(15, num, strip.Color(0, 0, 255), countdownRotate);
}
void set3HourLED(long num, bool countdownRotate) { //sets the fifth section of leds, the number of leds in the section is set by the num variable
  //num = constrain(num, 0, 5);
  setLEDhelperFunction(20, num, strip.Color(255, 0, 255), countdownRotate);
}

void setAllTimeLEDS(long nums[]) { //takes in an array of 5 values to set the leds of each of the 5 sections using the functions above
  set10SecLED(nums[0], true); //all second args are true because we need the rotating led as this function is used in the running timer mode
  set1MinLED(nums[1], true);
  set5MinLED(nums[2], true);
  set30MinLED(nums[3], true);
  set3HourLED(nums[4], true);
  strip.show();
}

void setAllTimeLEDS2(long nums[]) { //takes in an array of 5 values to set the leds of each of the 5 sections using the functions above
  set10SecLED(nums[0], false); //all second args are false because we do not need the rotating led as this function is used in the setup mode
  set1MinLED(nums[1], false);
  set5MinLED(nums[2], false);
  set30MinLED(nums[3], false);
  set3HourLED(nums[4], false);
  strip.show();
  //colorWipe(strip.Color(0,0,0),0);
}

void resetSection(int start, int num) { //function that resets a number of LEDS to off with a reference point (int start)
  for (uint16_t i = start; i < start + num; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
  }
}

void rainbowCycle() { //code taken from adafruit neopixel example code, this is used when the alarm starts, this is rather computationally expensive as it slows down all the other stuff
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) { 
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
  }
}

uint32_t Wheel(byte WheelPos) { //code taken from adafruit neopixel example code, this is used in the rainbow cycle function
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
