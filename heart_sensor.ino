/*

This code gets the user's pulse information using the PulseSensor library, then outputs the BPM to an LCD display. 
A buzzer plays a tone once heartbeat is detected.

*/

#define USE_ARDUINO_INTERRUPTS true   
#include <PulseSensorPlayground.h>
#include <LiquidCrystal.h>

int thershold = 550;
int bpm;
const int INPUT_PIN = 0; // input of the sensor
const int BUZZER_PIN = 13; // input of the buzzer
const int DANGER_PIN = 8; // input of the LED; signifies that something is wrong with the user's bpm (too slow or too fast)
const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;

PulseSensorPlayground pulseSensor;

LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

void setup() {

  Serial.begin(9600);

  // set up the sensor
  pulseSensor.blinkOnPulse(LED_BUILTIN);
  pulseSensor.analogInput(INPUT_PIN);
  pulseSensor.setThreshold(thershold);
  pulseSensor.begin();

  lcd.begin(16,2);
  lcd.print("BPM: ");

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(DANGER_PIN, OUTPUT);
}

void loop() {

  lcd.setCursor(5,0);

  if (pulseSensor.sawStartOfBeat()) {

    tone(BUZZER_PIN, 2000); // if a pulse is detected, make a 2kHz sound 
    digitalWrite(DANGER_PIN, LOW);

    bpm = pulseSensor.getBeatsPerMinute();

    // output the bpm on the display
    lcd.print(bpm);
    Serial.print(bpm);
    Serial.println(" beats per minute");

    // if bpm is too fast or too slow, notify the user by turning on an LED
    if (bpm < 60) {
      Serial.println("\nBPM too low (<60)");
      digitalWrite(DANGER_PIN, HIGH);
    } else if (bpm > 120) {
      Serial.println("\nBPM too high (>120)");
      digitalWrite(DANGER_PIN, HIGH);
    }
  }

  // erase the previous bpm value 
  if (bpm < 100) {
    lcd.setCursor(7, 0);
    lcd.print("  ");
  }

  // make sure the sound plays long enough to be heard
  delay(50);
  noTone(BUZZER_PIN);
  delay(50);
}
