#include "Modulino.h"
#include <FastLED.h>

#define NUMPIXELS 8
#define PIN 6  //pin for LED stripe
ModulinoKnob encoder;
CRGB leds[NUMPIXELS];  //initialize LED stripe

//Delay timing for group temperature
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;  //the value is a number of milliseconds (10 seconds)

//Delay timing for questions
unsigned long LEDstartMillis;
unsigned long LEDcurrentMillis;
const unsigned long LEDperiod = 1000;
bool questionLED = false;


// LED 1 (controlled by button)
const int redPin1 = 4;
const int greenPin1 = 3;
const int bluePin1 = 2;
int user1;

// LED 2 (controlled by knob)
const int redPin2 = 11;
const int greenPin2 = 10;
const int bluePin2 = 9;

//Question button variables
int buttonState = 0;
bool lastButton = false;

// --- Setup: Initialize all components ---
void setup() {

  // Setup RGB LEDs
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);

  pinMode(redPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(bluePin2, OUTPUT);

  //Set up LED stripe and modulino knob
  Serial.begin(9600);
  Modulino.begin();
  FastLED.addLeds<WS2812, PIN, GRB>(leds, NUMPIXELS);
  FastLED.setBrightness(50);
  encoder.begin();
  encoder.set(0);  // explicitly reset to 0 during setup

  // Initialize timers
  startMillis = millis();
  LEDstartMillis = millis();
}


void loop() {

  // --- Read dial value and map it to a color ---
  int value = encoder.get();
  int colorValue = encoder.get() * 2;
  colorValue = colorValue % 201;
  if (colorValue < 0) colorValue += 201;


  //Replace led color with simulated average temperature
  //user 1 is the imaginary "group" in the meeting
  user1 = colorValue + random(-50, 50);
  int avgColor = (user1 + colorValue) / 2;

  int r, g;
  int r1, g1;

  //transform into r,g value for user0 (You)
  if (colorValue <= 30) {
    // Hold pure red longer (0–50)
    r = 255;
    g = 0;
  } else if (colorValue <= 100) {
    // Red → Yellow (green increases)
    r = 255;
    g = map(colorValue, 31, 100, 0, 255);
  } else {
    // Yellow → Green (red decreases)
    r = map(colorValue, 101, 200, 255, 0);
    g = 255;
  }

  //transform into r,g value for average (Group)
  if (avgColor <= 30) {
    // Hold pure red longer (0–50)
    r1 = 255;
    g1 = 0;
  } else if (avgColor <= 100) {
    // Red → Yellow (green increases)
    r1 = 255;
    g1 = map(avgColor, 31, 100, 0, 255);
  } else {
    // Yellow → Green (red decreases)
    r1 = map(avgColor, 101, 200, 255, 0);
    g1 = 255;
  }

  bool currentButton = encoder.isPressed();
  // If the button was just pressed (rising edge detection)
  if (currentButton && !lastButton) {
    // Toggle the button state (0 → 1 or 1 → 0)
    buttonState = !buttonState;

    if (buttonState == 1) {
      analogWrite(redPin1, 255);
      analogWrite(greenPin1, 0);
      analogWrite(bluePin1, 255);
      FastLED.show();
    } else {
      analogWrite(redPin1, 0);
      analogWrite(greenPin1, 0);
      analogWrite(bluePin1, 0);
      leds[3] = CRGB(0, 0, 0);
      leds[4] = CRGB(0, 0, 0);
      questionLED = false;
      FastLED.show();
    }
  }
  lastButton = currentButton;

  // --- If the question button is pressed, handle blinking of question LEDs ---
  if (buttonState == 1) {
    if (LEDcurrentMillis - LEDstartMillis >= LEDperiod)  //test whether the period has elapsed
    {
      LEDstartMillis = LEDcurrentMillis;
      if (questionLED) {
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        FastLED.show();
        questionLED = false;
      } else {
        leds[3] = CRGB(255, 0, 255);
        leds[4] = CRGB(255, 0, 255);
        questionLED = true;
        FastLED.show();
      }
    }
  }
  // --- Update timing trackers ---
  currentMillis = millis();
  LEDcurrentMillis = millis();

  // --- Every 10 seconds, update the LED stripe color to reflect the average "temperature" ---
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    //mimic gradual change but not blink
    leds[0] = CRGB(r1, g1, 0);
    leds[1] = CRGB(r1, g1, 0);
    leds[2] = CRGB(r1, g1, 0);
    leds[5] = CRGB(r1, g1, 0);
    leds[6] = CRGB(r1, g1, 0);
    leds[7] = CRGB(r1, g1, 0);

    FastLED.show();
    startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
  }

  // --- Update the second physical RGB LED (LED2) to reflect user's personal temperature color ---
  analogWrite(redPin2, r);
  analogWrite(greenPin2, g);
}
