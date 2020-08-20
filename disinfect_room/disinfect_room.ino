//lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 12
DHT dht(dht_dpin, DHTTYPE);
//sesnor IR
#define sensor 7
int count_sensor = 0;
int tunggu;
boolean state = true;
//indikator
#define led_r 5
#define led_g 4
#define led_b 3
//pompa
#define pompa 8
//bar loading
int progressBari = 0;  
int i = 0;
// Kararter progress bar untuk LCD
byte progressBar[8] = {
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
B11111,
};
byte people[] = {
0b01110,
0b01110,
0b00100,
0b01110,
0b10101,
0b00100,
0b01010,
0b01010
};
void setup() {
// put your setup code here, to run once:
Serial.begin(9600);
dht.begin();
lcd.begin();
pinMode(sensor, INPUT);
pinMode(pompa, OUTPUT);
pinMode(led_r, OUTPUT);
pinMode(led_g, OUTPUT);
pinMode(led_b, OUTPUT);
lcd.createChar(0, progressBar);
progressBari= 16;

for (i=0; i<progressBari; i++)
{
  lcd.setCursor(i, 1);  
  lcd.write(byte(0)); 
  lcd.setCursor(0,0);
  lcd.print("Loading");
  delay(250);
}
lcd.clear();

lcd.setCursor(0,0);
lcd.print("AtmDuino Product");
lcd.setCursor(0,1);
lcd.print(" Disinfect Room");
delay(1500);
lcd.clear();

lcd.setCursor(1,0);
lcd.print("Made In Malang");
lcd.setCursor(2,1);
lcd.print("Local Pride");
delay(1500);
lcd.clear();

lcd.setCursor(0,0);
lcd.print("Contact Us");
lcd.setCursor(0,1);
lcd.print("08973234445");
delay(1500);
lcd.clear(); 
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(led_g,HIGH);
lcd.setCursor(0,0);
lcd.print(" Silahkan Masuk");
lcd.createChar(1, people);
lcd.setCursor(0,1);
lcd.write(1);
lcd.print(":");
lcd.print(count_sensor);           


float t = dht.readTemperature(); ;
lcd.setCursor(7,1);
lcd.print("T:");
lcd.print(t);
lcd.print("C");
lcd.write(0b11011111);

if (!digitalRead(sensor) && state){
      count_sensor++;
      lcd.setCursor(2,1);
      lcd.print(count_sensor);
      state = false;   
}
 if (!digitalRead(sensor)){
      digitalWrite(pompa,HIGH);
      digitalWrite(led_r,HIGH);
      digitalWrite(led_g,LOW);
        for(int tunggu=5; tunggu>=0; tunggu--){
        lcd.setCursor(0,0);
        lcd.print("Harap Antri:   ");
        lcd.setCursor(12,0);
        lcd.print(tunggu);
        digitalWrite(led_b,LOW);
        delay(1000);
        digitalWrite(led_b,HIGH);
     
      }
        if (tunggu == 0)
          digitalWrite(led_r,LOW);
          digitalWrite(led_b,LOW);
          digitalWrite(pompa,LOW);
}
if (digitalRead(sensor)){
    state = true;
    delay(100);
  }
}
