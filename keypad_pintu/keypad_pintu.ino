#include <Keypad.h>
#include <EEPROM.h>
#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define buzzer 2
#define pintu 3

const byte ROWS = 4;
const byte COLS = 4;
char password[4];
char pass[4],pass1[4];
int i=0;
int j=0;
char keys[ROWS][COLS] = {
{'1','2','3','a'}, 
{'4','5','6','b'}, 
{'7','8','9','c'}, 
{'#','0','*','d'}  
};

byte rowPins[ROWS] = {6, 5, 4, 3}; //pin untuk kontrol baris
byte colPins[COLS] = {9, 12, 11, 10}; //pin untuk kontrol kolom
Keypad keypad1 = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
pinMode(buzzer, OUTPUT);
pinMode(pintu, OUTPUT);
setLocked(true);
lcd.begin();
lcd.print(" Kunci ");
lcd.setCursor(0,1);
lcd.print(" Digital ");
delay(2000);
lcd.clear();
lcd.print(" Password ");
lcd.setCursor(0,1);
}

void loop()
{
char key = keypad1.getKey();
if (key == "*" || key == "#")
{
beep();
setLocked(true);
lcd.clear();
lcd.print(" Password ");
lcd.setCursor(0,1);
i=0;
}
if (key)
{
password[i++] = key;
lcd.print(key);
beep();
}

if (i == 4)
{
delay(200);
for(j=0; j<4; j++)
{
pass[j] = EEPROM.read(j);
}
if(!(strncmp(password,pass,4)))
{
lcd.clear();
lcd.print(" Password Benar ");
setLocked(false);
delay(2000);
lcd.clear();
lcd.print(" Password ");
lcd.setCursor(0,1);
i=0;
}
else
{
lcd.clear();
lcd.print(" Password Salah ");
beep();
beep();
delay(2000);
lcd.clear();
lcd.print(" Password ");
lcd.setCursor(0,1);
i=0;
}
}

delay(100);
}

void setLocked(int locked)
{
if (locked)
{
digitalWrite(pintu,LOW);

}
else
{
digitalWrite(pintu,HIGH);
}
}

void beep()
{
digitalWrite(buzzer, HIGH);
delay(20);
digitalWrite(buzzer, LOW);
}
