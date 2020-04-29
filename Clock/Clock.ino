#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

int starttime;
int activetime;
int prevoustime = 0;

int hours = 0;
int mins = 0;

int ahours = 0;
int amins = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.clear();

  Serial.begin(9600);

  // Pin 13's internal pullup won't work! Use external 1 kΩ resistor.
  pinMode(13, INPUT);
  digitalWrite(13, HIGH);

  // Right momentary button connected to pin 11
  // Set as input and enable internal pullup
  pinMode(11, INPUT);
  digitalWrite(11, HIGH);

  // Right momentary button connected to pin 10
  // Set as input and enable internal pullup
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);

  // Toggle switch connected to pin 8
  // Set as input and enable internal pullup
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);
  
  // Buzzer and LED connected to pin 9
  // Set as output
  pinMode(9, OUTPUT);

  starttime = millis() / 1000;
}

void loop() {
  // Default toggle switch position
  while(digitalRead(8) == LOW) {

    lcd.setCursor(6,1);
    lcd.print("Alarm");
    lcd.setCursor(6,0);

    // If middle or left button pressed, adjust alarm time
    if (digitalRead(11) == LOW) {
      amins++;
    } else if (digitalRead(10) == LOW) {
      ahours++;
    }

    // Print hours
    lcd.setCursor(6,0);
    if (ahours < 10) {
      lcd.print("0");
      lcd.print(ahours);
    } else {
      lcd.print(ahours);
    }

    lcd.print(":");

    // Print minutes
    if (amins < 10) {
      lcd.print("0"); lcd.print(amins);
    } else {
      lcd.print(amins);
    }

    // Manage overflow of minutes (59 -> 0)
    if (amins > 59) {
      ahours++;
      amins = 0;
    }
    // Manage overflow of hours (23 -> 0)
    if (ahours > 23) {
      ahours = 0; 
    }

    delay(500);
    lcd.clear();
  }

  // If far right (set) button pressed, 
  if (digitalRead(13) == LOW) {
    lcd.setCursor(5,1);
    lcd.print("Set Time");
    lcd.setCursor(6,0);

    // If either of the left two buttons pressed, adjust current time
    if (digitalRead(11) == LOW) {)
      mins++;
    } else if (digitalRead(10) == LOW) {
      hours++;
    }
  }

  // Calculate time since start (seconds)
  activetime = (millis() / 1000) - starttime;

  if (prevoustime < (activetime - 59)) {
    mins++;
    prevoustime = activetime;
  } 

  if (mins > 59) {
    hours++;
    mins = 0;
  } 

  if (hours > 23) {
    hours = 0; 
  }

  // Print hours
  lcd.setCursor(6,0);
  if (hours < 10) {
    lcd.print("0");
    lcd.print(hours);
  } else {
    lcd.print(hours);
  }

  lcd.print(":");

  // Print minutes
  if (mins < 10) {
    lcd.print("0");
    lcd.print(mins);
  } else {
    lcd.print(mins);
  }

  // If alarm time equals current time
  if (ahours == hours && amins == mins && amins != 0) {
    tone(9, 1000, 200);
    delay(200);
    noTone(9);
    delay(200);
  } else {
    delay(300);
  }

  lcd.clear();

  // Debugging information
  Serial.println(mins);
  Serial.println(hours);
  Serial.println("");
  Serial.println(amins);
  Serial.println(ahours);
  Serial.println("");
  Serial.println(activetime);
  Serial.println(prevoustime);
  Serial.println(starttime);
  Serial.println("");
}
