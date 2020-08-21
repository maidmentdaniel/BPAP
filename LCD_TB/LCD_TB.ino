#include <LiquidCrystal.h>
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  lcd.begin(20, 4);           // set up the LCD's number of columns and rows:
  lcd.clear();
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Hello World");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,4);
  lcd.print("Hello World");
  delay(5000);
  lcd.clear();
}
