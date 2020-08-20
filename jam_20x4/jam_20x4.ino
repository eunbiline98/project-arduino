#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"
#include <LiquidCrystal.h>
#define DHTPIN 9
#define DHTTYPE DHT11
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;
float temp, hum;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    rtc.begin();
    dht.begin();
    lcd.begin(20, 4);

   byte customCharL[] = {
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11111,
    0b11111
  };
  //C
  byte customCharC[] = {
    0b01111,
    0b11111,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11111,
    0b01111
  };
  //D
  byte customCharD[] = {
    0b11110,
    0b11111,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11111,
    0b11110
  };
  //O
  byte customCharO[] = {
    0b01110,
    0b11111,
    0b11011,
    0b11011,
    0b11011,
    0b11011,
    0b11111,
    0b01110
  };
  //K
  byte customCharK[] = {
    0b11011,
    0b11011,
    0b11110,
    0b11100,
    0b11100,
    0b11110,
    0b11011,
    0b11011
  };
  byte customCharY[] = {
    0b11111,
    0b01110,
    0b01110,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00100
  };
  byte customCharG[] = {
    0b00001,
    0b00001,
    0b00001,
    0b00101,
    0b00101,
    0b00101,
    0b10101,
    0b10101
  };
   byte customCharS[] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b10001,
    0b01110,
    0b00000,
    0b00000
  };
   byte customCharR[] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b00100,
    0b00100
  };


  lcd.createChar(0, customCharL);//huruf L
  lcd.createChar(1, customCharC);//huruf C
  lcd.createChar(2, customCharD);//hutuf D
  lcd.createChar(3, customCharO);//huruf O
  lcd.createChar(4, customCharK);//huruf K
  lcd.createChar(5, customCharY);//Antena
  lcd.createChar(6, customCharG);//Sinyal
  lcd.createChar(7, customCharR);//Bel
  

  lcd.setCursor(4,1);
  lcd.print(" "); // print space 
  lcd.write((uint8_t)0);  //L
  delay(400);
  lcd.write((uint8_t)1);  //C
  delay(400);
  lcd.write((uint8_t)2);  //D
  delay(400);
  lcd.print(" "); // print space 
  lcd.write((uint8_t)1);  //C
  delay(400);
  lcd.write((uint8_t)0);  //L
  delay(400);
  lcd.write((uint8_t)3);  //O
  delay(400);
  lcd.write((uint8_t)1);  //C
  delay(400);
  lcd.write((uint8_t)4);  //K
  delay(400);
  lcd.print("");
  delay(800);

  lcd.setCursor(3,2);
  lcd.print("AtmDuino 2019");
  delay(800); 
  lcd.clear();
  delay(800);

  lcd.setCursor(4,1);
  lcd.print(" "); // print space 
  lcd.write((uint8_t)0);  //L
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)2);  //D
  lcd.print(" "); // print space 
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)0);  //L
  lcd.write((uint8_t)3);  //O
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)4);  //K
  lcd.print("");

  lcd.setCursor(3,2);
  lcd.print("AtmDuino 2019");
  delay(800); 
  lcd.clear();
  delay(800);
  
  lcd.setCursor(4,1);
  lcd.print(" "); // print space 
  lcd.write((uint8_t)0);  //L
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)2);  //D
  lcd.print(" "); // print space 
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)0);  //L
  lcd.write((uint8_t)3);  //O
  lcd.write((uint8_t)1);  //C
  lcd.write((uint8_t)4);  //K
  lcd.print("");

  lcd.setCursor(3,2);
  lcd.print("AtmDuino 2019");
  delay(800); 
  lcd.clear();
  delay(800);
  
 
   if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
    }
   if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop () {
  temp = dht.readTemperature();
  hum  = dht.readHumidity();
  DateTime now = rtc.now();
  
    lcd.setCursor(1,0);  //Penampil Nama Jam
    lcd.write((uint8_t)7);//bell  
    lcd.setCursor(3,0);
    lcd.print(" "); // print space 
    lcd.write((uint8_t)0);  //L
    lcd.write((uint8_t)1);  //C
    lcd.write((uint8_t)2);  //D
    lcd.print(" "); // print space 
    lcd.write((uint8_t)1);  //C
    lcd.write((uint8_t)0);  //L
    lcd.write((uint8_t)3);  //O
    lcd.write((uint8_t)1);  //C
    lcd.write((uint8_t)4);  //K
    lcd.print(" v2 ");
    lcd.write((uint8_t)5);  //antena
    lcd.write((uint8_t)6);  //signal
  
    lcd.setCursor(0, 1); //Penampil JAM
    lcd.print(" -->> ");
    if (now.hour()<10)
    lcd.print('0');
    lcd.print(now.hour(), DEC);
    lcd.print(':');
     if (now.minute()<10)
    lcd.print('0');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second()<10)
    lcd.print('0');
    lcd.print(now.second(), DEC);
    lcd.print(" <<-- ");
   
    
    lcd.setCursor(0,2); //Penampil Hari
    lcd.print(">");
   int hari = now.dayOfTheWeek();
   switch(hari)
   {
     case 1:
     lcd.print("Senin");
     break;
     case 2:
     lcd.print("Selasa");
     break;
     case 3:
     lcd.print("  Rabu");
     break;
     case 4:
     lcd.print(" Kamis");
     break;
     case 5:
     lcd.print("jum'at");
     break;
     case 6:
     lcd.print(" Sabtu");
     break;
     case 0:
     lcd.print("Minggu");
     break;
    delay(1000);
}
    lcd.setCursor(9,2);  //Penampil Kalender
    lcd.print(">");
    lcd.setCursor(10,2);
    lcd.print(now.day(), DEC);
    lcd.print('-');
    lcd.print(now.month(), DEC);
    lcd.print('-');
    lcd.print(now.year(), DEC);
  
    lcd.setCursor(0, 3);  //Penampil Suhu
    lcd.print("T:");
    lcd.print(temp);
    lcd.write(0b11011111);
    lcd.print("C");

    lcd.setCursor(12, 3);  //Penampil Kelembapan
    lcd.print("H:");
    lcd.print(hum);
    lcd.print("%");



}
