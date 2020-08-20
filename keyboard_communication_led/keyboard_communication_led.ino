int databaca;
const int relay1 = 4;
const int relay2 = 5;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
Serial.begin(9600);
lcd.begin();
pinMode(relay1, OUTPUT);
pinMode(relay2, OUTPUT);
}
 
void loop()
{
if (Serial.available())
{
int bacadata = Serial.read();
if (bacadata =='a') {databaca=1;}
if (bacadata =='b') {databaca=2;}
if (bacadata =='c') {databaca=3;}
if (bacadata =='d') {databaca=4;}

 
if(databaca==1)
lamp1_on:
{
digitalWrite(relay1, LOW);
lcd.setCursor(0,0);
lcd.print("Kondisi Lampu :");
lcd.setCursor(0,1);
lcd.print("Lampu 1 Menyala ");
loop();
goto lamp1_on;
}
 
if(databaca==2)
lamp1_off:
{
digitalWrite(relay1, HIGH);
lcd.setCursor(0,0);
lcd.print("Kondisi Lampu :");
lcd.setCursor(0,1);
lcd.print("Lampu 1 Mati    ");
loop();
goto lamp1_off;
}

if(databaca==3)
lamp2_on:
{
digitalWrite(relay2, LOW);
lcd.setCursor(0,0);
lcd.print("Kondisi Lampu :");
lcd.setCursor(0,1);
lcd.print("Lampu 2 Menyala ");
loop();
goto lamp2_on;
}
 
if(databaca==4)
lamp2_off:
{
digitalWrite(relay2, HIGH);
lcd.setCursor(0,0);
lcd.print("Kondisi Lampu :");
lcd.setCursor(0,1);
lcd.print("Lampu 2 Mati    ");
loop();
goto lamp2_off;
}
}
}
