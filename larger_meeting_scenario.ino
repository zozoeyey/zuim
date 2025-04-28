#include "Modulino.h"
#include <FastLED.h>

#define NUMPIXELS 8
#define PIN 6  //pin for LED stripe
ModulinoKnob encoder;
CRGB leds[NUMPIXELS];  //initialize LED stripe

//Delay timing for group temperature
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 5000;  //the value is a number of milliseconds (5 seconds); temperature changes faster for a bigger group

//Delay timing for questions pulsing
unsigned long LEDstartMillis;
unsigned long LEDcurrentMillis;
const unsigned long LEDperiod = 1000;
bool questionLED = false;

//Delay timing for updating random number of questions
unsigned long QstartMillis;
unsigned long QcurrentMillis;
const unsigned long Qperiod = 20000;
int randQuestions = 0;

// LED 1 (controlled by button)
const int redPin1 = 4;
const int greenPin1 = 3;
const int bluePin1 = 2;
int user1;

// LED 2 (controlled by knob blend)
const int redPin2 = 11;
const int greenPin2 = 10;
const int bluePin2 = 9;

//LED stripe
int buttonState = 0;
bool lastButton = false;

// --- Setup: Initialize all components ---
void setup() {

  // Setup RGB LEDs
  //LED 1 shows if user has a question
  pinMode(redPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  pinMode(bluePin1, OUTPUT);

  //LED 2 shows user's own temperature change
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
  QstartMillis = millis();
}


void loop() {

  // --- Read dial value and map it to a color ---
  int value = encoder.get();

  // === LED 2: Color blend based on knob ===
  int colorValue = encoder.get() * 2;
  colorValue = colorValue % 201;
  if (colorValue < 0) colorValue += 201;

  // Simulate influence of other people's inputs
  int numPeople = 50;
  int currentSum = 0;
  int currentUser = (colorValue + random(-50, 50)) * 49;
  int avgColor = (currentUser + colorValue) / numPeople;

  int r, g;
  int r1, g1;

  //transform into r,g value for user0
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

  //transform into r,g value for average
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

  //number of questions changes once in a while
  if (QcurrentMillis - QstartMillis >= Qperiod) {
    randQuestions = random(0, 3);
  }

  int numQuestions = randQuestions + buttonState;
  Serial.print("Number of questions:");
  Serial.println(numQuestions);

  // Calculate pulse period based on number of questions
  long pulsePeriod = 1000 / numQuestions;

  // --- LED Pulse Effect for Question Reminder ---
  if (numQuestions > 0) {
    if (LEDcurrentMillis - LEDstartMillis >= pulsePeriod)  //test whether the period has elapsed
    {
      LEDstartMillis = LEDcurrentMillis;
      if (questionLED) {
        // Turn OFF question LEDs
        leds[3] = CRGB(0, 0, 0);
        leds[4] = CRGB(0, 0, 0);
        FastLED.show();
        questionLED = false;
      } else {
        // Turn ON question LEDs
        leds[3] = CRGB(255, 0, 255);
        leds[4] = CRGB(255, 0, 255);
        questionLED = true;
        FastLED.show();
      }
    }
  }
  //update timing
  LEDcurrentMillis = millis();
  currentMillis = millis();

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
