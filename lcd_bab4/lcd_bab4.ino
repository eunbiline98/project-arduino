#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup() {
lcd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.setCursor(4,0);
lcd.print("UJI COBA");
lcd.setCursor(4 ,1);
lcd.print("LCD 16X2");
}
