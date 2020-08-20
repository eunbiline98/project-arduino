//sensor kekeruhan
byte ldr = A1;
#define laser 13
float kekeruhan;
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

float dingin, hangat, panas;
float normal, agak_keruh, keruh;

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

  float tempC = (sensors.getTempCByIndex(00));
  lcd.setCursor(0,0);
  lcd.print("Suhu : ");
  lcd.print(sensors.getTempCByIndex(00));
  lcd.print(" C");

  int nilai= analogRead(ldr);
  Serial.println(ldr);
  kekeruhan = ( 100 - ( (nilai/1023.00) * 100 ) ); 
  lcd.setCursor(0,1);
  lcd.print("Kadar: ");
  lcd.print(kekeruhan);
  lcd.print(" %");
  
  //======================
   //   fuzzyfikasi suhu
   //======================
  
   if(tempC <= 20){
   dingin = 1;
   hangat = 0;
   panas = 0;
   }
   else if(tempC >= 30){
   panas = 1;
   hangat = 0;
   dingin = 0;
   }
   else if(tempC == 26){
   hangat = 1;
   dingin = 0;
   panas = 0;
   }
   else if((tempC > 20)&&(tempC < 26)){
   hangat = (tempC - 25)/5;
   dingin = 1-(tempC - 25)/5;
   panas = 0;
   }
   else if((tempC > 26)&&(tempC < 30)){
   panas = (tempC - 30)/5;
   hangat = 1-(tempC - 30)/5;
   dingin = 0;
   }
  
  
 //  Serial.print("panas= ");  
 //  Serial.println(panas);
 //  Serial.print("hangat= ");  
 //  Serial.println(hangat);
 //  Serial.print("dingin= ");  
 //  Serial.println(dingin);

 //===============================
   //       fuzzyfikasi kekeruhan
   //===============================
  
   if(nilai <= 30){
   normal = 1;
   agak_keruh = 0;
   keruh = 0;
   }
   else if(nilai >= 90){
   keruh = 1;
   agak_keruh = 0;
   normal = 0;
   }
   else if(nilai == 75){
   keruh = 0;
   agak_keruh = 1;
   normal = 0;
   }
   else if((nilai > 30)&&(nilai < 75)){
   agak_keruh = (nilai - 30)/50;
   normal = 1-(nilai - 30)/50;
   keruh = 0;
   }
   else if((nilai > 75)&&(nilai < 90)){
   agak_keruh = (nilai - 75)/100;
   keruh = 1-(nilai - 75)/100;
   normal = 0;
   }
  
 //  Serial.print("normal= ");  
 //  Serial.println(normal);
 //  Serial.print("agak_keruh= ");  
 //  Serial.println(agak_keruh);
 //  Serial.print("keruh= ");  
 //  Serial.println(keruh);

 //==================================
   //           fuzzy rule
   //===================================
     
   //============================================================================
   if((tempC <= 20 )&&(nilai <= 30)){ 
    if(dingin < normal){
    digitalWrite(heater,HIGH);
    digitalWrite(pompa,LOW);
    }
    else if(dingin > normal ){
    digitalWrite(heater,LOW);
    digitalWrite(pompa,HIGH);
    }
    else if(dingin == normal){
    digitalWrite(heater,HIGH);
    digitalWrite(pompa,HIGH);
    }
  }

  
}
  
