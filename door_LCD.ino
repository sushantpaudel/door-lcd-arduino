#include <LiquidCrystal.h>
#include <EEPROM.h>
int addr = 0;
int cursorPosition = 0;
boolean isInput = false;

LiquidCrystal lcd(12, 13, 4, 5, 6, 7); // sets the interfacing pins

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void clear() {
  lcd.clear();
}

void reset() {
  clear();
  addr = 0;
  cursorPosition = 0;
  lcd.print("Loading...");
  delay(2000);
  clear();
}

void relocateCursor() {
  if (cursorPosition == 1) {
    delay(2000);
    clear();
    cursorPosition = 0;
  } else {
    cursorPosition = 1;
  }
  lcd.setCursor(0, cursorPosition);
}

void loop() {
  if (Serial.available()) {
    char value = (char)Serial.read();
    if (value == '$') {
      addr = 0;
      isInput = true;
    }
    if (isInput) {
      EEPROM.write(addr, value);
      addr++;
      if (value == '/') {
        relocateCursor();
      } else if (value == '|') {
        reset();
      } else {
        if (value != '$') lcd.write(value);
      }
    }
  } else {
    addr++;
    isInput = false;
    char value = (char)EEPROM.read(addr);
    if (value == '/') {
      relocateCursor();
    } else if (value == '|') {
      delay(3000);
      reset();
    } else {
      lcd.write(value);
    }
  }
  delay(200);
}
