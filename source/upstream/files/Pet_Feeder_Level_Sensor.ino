#include <Adafruit_NeoPixel.h>

#define PIN 32

// Number of LEDs that are connected to the microcontroller
#define NUMPIXELS 18 

// Initialize the NeoPixel library. 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500  // Time (in milliseconds) to pause between pixels

// Variables for the joystick
int potPin = 35;
int potValue;
int levelValue;

// Variables for the millis() timing functions
unsigned long startMillis; 
unsigned long currentMillis;
unsigned long previousMillis = 0;  
const unsigned long interval = 1000; 

int ledState = 0;  // ledState used to toggle the LED for flashing

void setup() {
  startMillis = millis();  // Initial start time
  pixels.begin();          // Initialize the NeoPixel strip object
}

void loop() {
  pixels.clear();  // Set all pixel colors to 'off'

  // Gets the current time, reads the joystick positon, and maps the joystick position to 0-18 (number of pixels)
  unsigned long currentMillis = millis();
  potValue = analogRead(potPin);
  levelValue = map(potValue, 745, 0, 0, 18);

  // If the level is above 9, the LED strip is set to green and reflects the posiiton of the joystick level arm. 
  if (levelValue >= 9) {
    for (int i = 0; i <= levelValue; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();
    }

  // If the value is less than 9 but greater than one, the LEDs are set to yellow and reflect the position of the joystick level arm. 
  } else if (levelValue >= 1) {
    for (int i = 0; i <= levelValue; i++) {
      pixels.setPixelColor(i, pixels.Color(255, 255, 0));
      pixels.show();
    }

  //If the level is less than 1, it will flash the status bar read, indicating a refill is needed)
  } else {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      // If the LED is off, turn it on, and vice-versa:
      if (ledState == 0) {
        ledState = 1;
      } else {
        ledState = 0;
      }
      if (ledState == 1) {
        for (int i = 0; i <= NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        }
      } else {
        for (int i = 0; i <= NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        }
      }

      pixels.show();  // Send the updated pixel colors to the hardware.
    }
  }
}
