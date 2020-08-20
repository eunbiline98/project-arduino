//sensor kekeruhan
int ldr = A1;
#define laser 13
float kekeruhan = 0;
//relay
#define pompa 6
#define heater 5
//suhu
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//lcd
#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
 //Serial monitor pada baut 9600bps
  Serial.begin(9600);
  lcd.begin();
  pinMode(pompa,OUTPUT);
  pinMode(heater,OUTPUT);
  pinMode(laser,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Monitoring Kolam");
  
  lcd.setCursor(0,1);
  lcd.print("Budidaya Ikan");
  delay(5000);
  sensors.begin();
  lcd.clear(); 
}
void loop()
{
  digitalWrite(laser,HIGH);
  sensor_kekeruhan();
  sensor_suhu();
}
void sensor_suhu(){
  float tempC = (sensors.getTempCByIndex(00));
  lcd.setCursor(0,0);
  lcd.print("Suhu : ");
  lcd.print(sensors.getTempCByIndex(00));
  lcd.print(" C");

  if (tempC >= 26.00){
    digitalWrite(heater,LOW);
  }
   if (tempC <= 24.00){
   digitalWrite(heater,HIGH); 
  }
  
//  Serial.print("Request pembacaan suhu..");
//  sensors.requestTemperatures(); 
//  Serial.println("Ok");
//  Serial.print("Pembacaan suhu : ");
//  Serial.print(sensors.getTempCByIndex(00));
//  delay(1000);
  
}
void sensor_kekeruhan(){
  int nilai= analogRead(ldr);
  Serial.println(nilai);
  Serial.print("Pembacaan kekeruhan : ");
  Serial.println(kekeruhan);
  delay(1000);
  kekeruhan = ( 100 - ( (nilai/1023.00) * 100 ) );
  lcd.setCursor(0,1);
  lcd.print("Kadar: ");
  lcd.print(kekeruhan);
  lcd.print(" % ");

  if ( kekeruhan >= 50.00){
    digitalWrite(pompa,HIGH);
  }
  else{
   digitalWrite(pompa,LOW); 
  }
}

