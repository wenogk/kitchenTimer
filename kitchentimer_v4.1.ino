#include <MPU6050_tockn.h>
#include <Adafruit_NeoPixel.h>

#include<Wire.h>
#include <SparkFunMPU9250-DMP.h>
MPU6050 mpu6050(Wire);
MPU9250_DMP imu;
const int MPU_addr = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double x;
double y;
double z;
#include <math.h>
#define PIN 5
int status;
int numOfRingLEDS = 25;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numOfRingLEDS, PIN, NEO_GRB + NEO_KHZ800);
long timer = 0;

float initialAngleX, initialAngleY, initialAngleZ;
int numOfLights;
float maxAngleZ = 9.81;
float minAngleZ = -9.81;
int prevLoaderLedIndex = 1;
int mode = 0, prevMode = 0; // 0 is setup and 1 is start timer 3 is reset timer
bool alarmOn = false;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////Counting down time //////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
long modeChangerTimer = 0;
long timerSet = 0;
bool startTimer = false;
long timerOffset = 0;
long numOf10sec, numOf1min,  numOf5min, numOf30min, numOf3hour;
int setupOf10sec = 0;
int setupOf1min = 0;
int setupOf5min = 0;
int setupOf30min = 0;
int setupOf3hour = 0;
long millisOf10sec = 10000;
long millisOf1min = 60000;
long millisOf5min = 300000;
long millisOf30min = 1800000;
long millisOf3hour = 10800000;
long prevTimeArray[5] = {0, 0, 0, 0, 0};
long currentTimeLeft, currentTimeLeftTotal;

void setup() {
  Serial.begin(9600); //for debugging purposes
  mpuSetup(); // mpu setup for gyro + tap detection
  timerSet = 0; //default value for timerSet
  mode = 0; //default mode, 0, which is the setup timer mode
  ledSetup(); //setup of the Adafruit neopixel library
}

void loop() {
  mpuLoopFunction(); // the mpu loop code, moved to a different file
  // the > 1500 part is to make sure there is a 1.5 second wait before any changes of modes to avoid any accidental mode changes
  if ((mpu6050.getAccZ() > 0.8) && ((millis() - modeChangerTimer) > 1500)) { // if the device is placed flat on the surface, the angle acceleration in the z direction is  < 0.8
    if (!alarmOn) {  
      mode = 1; //timer start
    } else { 
      mode = 2; //alarm, timer finished
    }
    modeChangerTimer = millis(); 
    
  }  else if (((millis() - modeChangerTimer) > 1500)) {  // the > 1500 part is to make sure there is a 1.5 second wait before any changes of modes to avoid any accidental mode changes
    mode = 0; // setup timer
    modeChangerTimer = millis();
  }


  if (mode != prevMode) { 
    resetSection(0, 25); //this is necessary to refresh all leds in case of a mode change
    if (prevMode == 2 && mode == 0) { //if alarm is set off and then device is tilted back, it turns to mode 1
      resetFullTimer(); // this sets the alarm off and resets all setup timer values to 0 to restart the setup timer process
    }
    if (prevMode == 1 && mode == 0) { //if alarm is set off and then device is tilted back, it turns to mode 1
      resetFullTimer(); // this sets the alarm off and resets all setup timer values to 0 to restart the setup timer process
    }
  }

  prevMode = mode;

  if (mode == 0) { //setup timer mode
    setupTimer(z, strip.Color(255, 0, 0), strip.Color(0, 0, 255)); // function, the second two arguments don't mean anything
  } else if (mode == 1) { //timer start mode
    long vals[5] = {0, 0, 0, 0, 0}; //each of the 5 values represents the number of LED's to turn on in each of the 5 sections of the device
    getCountdownValues(vals); // function that takes in the current timer and returns the number of leds to turn on in each section
    setAllTimeLEDS(vals);  // function that interfaces with the neopixel leds and sets them on according to the vals array
    if (!compareArrs(prevTimeArray, vals)) { // this comparison and reset is necessary to switch off any leds that need to be off
      resetSection(0, 25);
    }
    prevTimeArray[0] = vals[0];
    prevTimeArray[1] = vals[1];
    prevTimeArray[2] = vals[2];
    prevTimeArray[3] = vals[3];
    prevTimeArray[4] = vals[4];
  } else if (mode == 2) { //mpde for when alarm goes off
    alarmMode(); 
  }
}
