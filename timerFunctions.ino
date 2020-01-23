
void setupTimer(int z, uint32_t c, uint32_t c2) {

  bool tapped = false; 
  //Don't move this imu stuff to its own function, I tried and it messed up stuff.
  if ( imu.fifoAvailable() )
  {
    // DMP FIFO must be updated in order to update tap data
    imu.dmpUpdateFifo();
    // Check for new tap data by polling tapAvailable
    if ( imu.tapAvailable() )
    {
      // If a new tap happened, get the direction and count
      // by reading getTapDir and getTapCount
      unsigned char tapDir = imu.getTapDir();
      if (tapDir == TAP_Z_DOWN) {
        tapped = true;
      } else {
        tapped = false;
      }
    }
  }

 // Serial.println(setupOf10sec);
  float currentAngle = (float)z; 
  //z is an angle
  if ((z >= 54) && (z < 126)) { // 10 second section setup, adds 1 to the setupOf10sec at each tap, also wraps back to 0 when it reaches past 5 as each section only allows 5 addtions
    if (tapped) {
      setupOf10sec = setupOf10sec + 1;
      if (setupOf10sec > 5) {
        setupOf10sec = 0 ;
      }
    }
  } else if ((z >= 126) && (z < 198)) { // 1 min section setup, adds 1 to the setupOf1min at each tap, also wraps back to 0 when it reaches past 5 as each section only allows 5 addtions
    if (tapped) {
      setupOf1min = setupOf1min + 1;
      if (setupOf1min > 5) {
        setupOf1min = 0 ;
      }
    }
  } else if ((z >= 198) && (z < 270)) { // 5 min section setup, adds 1 to the setupOf5min at each tap, also wraps back to 0 when it reaches past 5 as each section only allows 5 addtions

    if (tapped) {
      setupOf5min = setupOf5min + 1;
      if (setupOf5min > 5) {
        setupOf5min = 0 ;
      }
    }
  } else if ((z >= 270) && (z < 342)) { // 30 min section setup, adds 1 to the setupOf10sec at each tap, also wraps back to 0 when it reaches past 5 as each section only allows 5 addtions

    if (tapped) {
      setupOf30min = setupOf30min + 1;
      if (setupOf30min > 5) {
        setupOf30min = 0 ;
      }
    }
  } else {  // 3 hour section setup, adds 1 to the setupOf3hour at each tap, also wraps back to 0 when it reaches past 5 as each section only allows 5 addtions
    if (tapped) {
      setupOf3hour = setupOf3hour + 1;
      if (setupOf3hour > 5) {
        setupOf3hour = 0 ;
      }
    }
  }
  long values[5] = {setupOf10sec, setupOf1min, setupOf5min, setupOf30min, setupOf3hour};
  timerSet = millis() + setupOf3hour * millisOf3hour + setupOf30min * millisOf30min + setupOf5min * millisOf5min + setupOf1min * millisOf1min + setupOf10sec * millisOf10sec; //calculation that returns the exact time in milli seconds for the timer that was set up

  setAllTimeLEDS2(values); //sets the LEDS as per the the values array
}

void getCountdownValues(long arrVals[5]) {
  
   bool tapped = false;
  //Don't move this imu stuff to its own function, I tried and it messed up stuff.
  if ( imu.fifoAvailable() )
  {
    // DMP FIFO must be updated in order to update tap data
    imu.dmpUpdateFifo();
    // Check for new tap data by polling tapAvailable
    if ( imu.tapAvailable() )
    {
      // If a new tap happened, get the direction and count
      // by reading getTapDir and getTapCount
      unsigned char tapDir = imu.getTapDir();
      if (tapDir == TAP_Z_DOWN) {
        tapped = true;
      } else {
        tapped = false;
      }
    }
  }
  if(tapped) {
    startTimer = true;
    timerOffset = millis();
    }
  if(startTimer) {
  timerSet = timerOffset + setupOf3hour * millisOf3hour + setupOf30min * millisOf30min + setupOf5min * millisOf5min + setupOf1min * millisOf1min + setupOf10sec * millisOf10sec;
  currentTimeLeft = timerSet - millis();
  currentTimeLeftTotal = currentTimeLeft;
  if (currentTimeLeft <= 0) { // if the current time left is 0 or less than it, it means the timer is finished and the alarm needs to be set off
    alarmOn = true; //changing this to true results in the rainbow cycle being turned on, this can be seen at the mode changes in the main kitchentimer ino file
    return;
  } else {
    alarmOn = false;
    }

  if (currentTimeLeft > millisOf3hour) { //Check if any 3 hour LEDs should be on
    numOf3hour = currentTimeLeft / millisOf3hour; //Number of 3 hour LEDs that are ON
    currentTimeLeft = currentTimeLeft - (millisOf3hour) * (numOf3hour); //Calculate amount of time left after accounting for the 3 hour LEDs 
  } else {
    numOf3hour = 0; 
    //resetSection(20);
  }


  if (currentTimeLeft > millisOf30min) { //Check if any 30 minute LEDs should be on
    numOf30min = currentTimeLeft / millisOf30min; //Number of 30 minute LEDs that are ON
    currentTimeLeft = currentTimeLeft - (millisOf30min) * (numOf30min); //Calculate amount of time left after accounting for the 30 minute LEDs
  } else {
    numOf30min = 0;
    //resetSection(15);
  }


  if (currentTimeLeft > millisOf5min) { //Refer to similar code for 3 hour and 30 minute LEDs
    numOf5min = currentTimeLeft / millisOf5min;
    currentTimeLeft = currentTimeLeft - (millisOf5min) * (numOf5min);
  } else {
    numOf5min = 0;
    //resetSection(10);
  }


  if (currentTimeLeft > millisOf1min) { //Refer to similar code for 3 hour and 30 minute LEDs
    numOf1min = currentTimeLeft / millisOf1min;
    currentTimeLeft = currentTimeLeft - (millisOf1min) * (numOf1min);
  } else {
    numOf1min = 0;
    //resetSection(5);
  }


  if (currentTimeLeft > millisOf10sec) { //Refer to similar code for 3 hour and 30 minute LEDs
    numOf10sec = currentTimeLeft / millisOf10sec;
  } else {
    numOf10sec = 0;
    // resetSection(0);
  }

  long test = 10 / 3;
  arrVals[0] = numOf10sec;
  arrVals[1] = numOf1min;
  arrVals[2] = numOf5min;
  arrVals[3] = numOf30min;
  arrVals[4] = numOf3hour;


}
}
void alarmMode() {

  rainbowCycle();
    
  }
void resetFullTimer() {
  timerSet = 0;
  startTimer = false;
  alarmOn = false;
  setupOf10sec = 0;
  setupOf1min = 0;
  setupOf5min = 0;
  setupOf30min = 0;
  setupOf3hour = 0;
  }
