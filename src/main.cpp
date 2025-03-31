#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include "FastLED.h"

// PINS
#define LED_PIN 2
#define ORANGE_BUTTON 3
#define BLUE_BUTTON 4
#define DF_RX 10
#define DF_TX 11
#define NUM_LEDS 18

// Initialize software serial on pins 10 and 11
SoftwareSerial mySoftwareSerial(DF_RX, DF_TX);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Initialize LEDs
CRGB leds[NUM_LEDS];

CRGB currentColor = CRGB::Black; // Holds the last button-pressed color

void start_player() {
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Initializing DFPlayer module... Wait!"));
  delay(500);
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Not initialized:"));
    Serial.println(F("1. Check the DFPlayer Mini connections"));
    Serial.println(F("2. Insert an SD card"));
    while (true);
  }
  Serial.println(F("DFPlayer Mini module initialized!"));
  delay(500);
  myDFPlayer.volume(20);
  myDFPlayer.play(1); // Play file 0001.mp3 on startup
}

void start_leds() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void set_leds(CRGB color) {
  fill_solid(leds, NUM_LEDS, color);
  FastLED.show();
}

void pulse_effect(CRGB color) {
  for (int i = 50; i <= 255; i += 5) { // Brighten
    FastLED.setBrightness(i);
    set_leds(color);
    delay(20);
  }
  for (int i = 255; i >= 50; i -= 5) { // Dim
    FastLED.setBrightness(i);
    set_leds(color);
    delay(20);
  }
}

void idle_flicker() {
  int flickerBrightness = random(100, 150); // Random flicker range
  FastLED.setBrightness(flickerBrightness);
  set_leds(currentColor);
  delay(100);
}

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  pinMode(ORANGE_BUTTON, INPUT_PULLUP);
  pinMode(BLUE_BUTTON, INPUT_PULLUP);

  start_player();
  start_leds();
}

void loop() {
  if (digitalRead(ORANGE_BUTTON) == LOW) {
    myDFPlayer.play(4); // Play a different audio file when button 1 is pressed
    Serial.println("Button 1 Pressed");
    currentColor = CRGB::DarkOrange;
    pulse_effect(currentColor);
    

    // Wait until button is released
    while (digitalRead(ORANGE_BUTTON) == LOW);
    delay(100); // Debounce delay
  } 
  else if (digitalRead(BLUE_BUTTON) == LOW) {
    myDFPlayer.play(3); // Play a different audio file when button 2 is pressed
    Serial.println("Button 2 Pressed");
    currentColor = CRGB::CadetBlue;
    pulse_effect(currentColor);
    

    // Wait until button is released
    while (digitalRead(BLUE_BUTTON) == LOW);
    delay(100); // Debounce delay
  }

  // Idle flicker effect when no button is pressed
  if (digitalRead(ORANGE_BUTTON) == HIGH && digitalRead(BLUE_BUTTON) == HIGH) {
    idle_flicker();
  }
}
