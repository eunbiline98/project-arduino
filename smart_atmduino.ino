//thinger.io
#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#define USERNAME "atmduino"
#define DEVICE_ID "Smart_kos"
#define DEVICE_CREDENTIAL "eunbiline98"

#define SSID "Pkm_asia"
#define SSID_PASSWORD "sistemkomputer"
ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
//pin lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Wire.h>
//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D5
DHT dht(dht_dpin, DHTTYPE); 
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

void setup(){
lcd.begin();
dht.begin();
pinMode(D4, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);

thing.add_wifi(SSID, SSID_PASSWORD);

  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thing["Stop Kontak"] << digitalPin(D6);
  thing["Lampu"] << digitalPin(D7);
  // resource output example (i.e. reading a sensor value)
  //thing["millis"] >> outputValue(millis());
  //pembacaan sensor dht11
  thing["dht11"] >>[](pson & out){
    out["humidity"] = dht.readHumidity();
    out["celsius"] = dht.readTemperature();
    out["fahrenheit"] = dht.readTemperature(true);  
  };
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
lcd.print("Smart Switch");
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
lcd.clear(); //menghapus data sebelumnya 

//connect to internet
  while (WiFi.status() != WL_CONNECTED) {           //wait till connected to WiFi
    delay(200);  
    lcd.setCursor(0,0); 
    lcd.print("Menunggu Koneksi"); 
    }
//ip address status
  lcd.setCursor(0,0);
  lcd.print("Ip Address       ");  
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());  
  delay(2000);
  lcd.clear();
}
 
void loop(){
thing.handle();

int wifiStatus = WiFi.status();
if(wifiStatus == 3){
    lcd.setCursor(0,1);
    lcd.print("Connected...       ");
}
  else{
      lcd.setCursor(0,1);
      lcd.print("Disconnected...       ");
}
float h = dht.readHumidity();
float t = dht.readTemperature();    
  lcd.setCursor(0,0);     
    lcd.print("H= ");
    lcd.setCursor(2,0);
    lcd.print(h);
    lcd.print("%  ");
    lcd.setCursor(8,0); 
    lcd.print("T= ");
    lcd.setCursor(10,0);
    lcd.print(t); 
    lcd.print("C  ");
   }
