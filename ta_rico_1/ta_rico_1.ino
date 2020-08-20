//suhu
#include <OneWire.h> 
#include <DallasTemperature.h> 
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorSuhu(&oneWire);

//sensor kekeruhan
byte ldr = A1;
float sensorKekeruhan;
#define laser 13
//relay
#define pompa 6
#define heater 5

//lcd
#include <Wire.h> 
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

float temp_sensor ,kekeruhan_sensor;
// nilai output sensor temp
float suhu;
// nilai output sensor kekeruhan
float kekeruhan;
//hasil defuzzy
float out;
//parameter temp sensor
float dingin,hangat,panas;
//parameter kekeruhan sensor
float jernih,agakkeruh,keruh;
//rule fuzzy
float rule1, rule2, rule3,rule4,rule5,rule6;

unsigned char suhuDingin(){
  if (suhu <= 24){
    dingin =1;
    }
  else if (suhu >=24 && suhu <=27){
    dingin=(27-suhu)/15;
    }
  else if (suhu >= 27)
  {dingin =0;
  }
  return dingin;
}
unsigned char suhuHangat(){
  if (suhu <= 24){
    hangat =0;
    }
  else if (suhu >=24 && suhu <=27){
    hangat=(suhu-24)/15;
    }
  else if (suhu >=27 && suhu <=30){
    hangat=(30-suhu)/15;
    }
  else if (suhu >= 30){
    hangat =0;
    }
  return hangat;
}
unsigned char suhuPanas (){
  if (suhu <=27 ){
    panas =0;
    }
  else if (suhu >=27 && suhu <=30){
    panas=(suhu-27)/15;
    }
  else if (suhu >= 30){
    panas =1;
    }
  return panas;
}
unsigned char kekeruhanJernih (){
  if (kekeruhan <=30 ){
    jernih =0;
    }
  else if (kekeruhan >=30 && kekeruhan <=60){
    jernih = (60-kekeruhan)/15;}
  else if (kekeruhan >= 60){
    jernih =0;
  }
  return jernih;
}
unsigned char kekeruhanagakkeruh(){
  if (kekeruhan <= 30){
    agakkeruh =0;
    }
  else if (kekeruhan >=30 && kekeruhan <=60){
    agakkeruh=(kekeruhan-30)/15;
    }
  else if (kekeruhan >=60 && kekeruhan <=90){
    agakkeruh=(90-kekeruhan)/15;
    }
  else if (kekeruhan >= 60){
    agakkeruh =0;
    }
  return agakkeruh;
}
unsigned char kekeruhankeruh (){
  if (kekeruhan <=60 ){
    keruh =0;
    }
  else if (kekeruhan >=60 && kekeruhan <=90){
    keruh=(kekeruhan-60)/15;
    }
  else if (kekeruhan >= 90){
    keruh =1;
    }
  return keruh;
}

//Fuzzifikasi
void fuzzifikasi(){
  suhuDingin();
  suhuHangat();
  suhuPanas();
  kekeruhanJernih;
  kekeruhanagakkeruh;
  kekeruhankeruh;
}
// Rule
void fuzzy_rule (){
  
// jika suhu dingin dan kekeruhan jernih maka heater on dan pompa off
  rule1;
  if ((dingin =1) || (hangat=1)){
    digitalWrite(heater,HIGH);
  }
  if ((jernih =1) || (agakkeruh=0)){
    digitalWrite(pompa,LOW);
  }
// jika suhu dingin dan kekeruhan agak keruh maka heater on dan pompa on
   rule2;
   if ((dingin =1) && (hangat=1)){
    digitalWrite(heater,HIGH);
  }
   if ((agakkeruh =1) || (keruh =1)){
    digitalWrite(pompa,HIGH);
  }
//  jika suhu dingin dan kekeruhan keruh maka heater on dan pompa on
    rule3;
   if ((dingin =1) || (hangat=1)){
    digitalWrite(heater,HIGH);
  }
  if (keruh =1){
    digitalWrite(pompa,HIGH);
  }
// jika suhu hangat dan kekeruhan jernih maka heater off dan pompa off
   rule4;
    if ((hangat =0) || (panas =1)){
      digitalWrite(heater,LOW);
    }
    if ((jernih =1) || (agakkeruh =0)){
      digitalWrite(pompa,HIGH);
    }
// jika suhu hangat dan kekeruhan agak keruh maka heater off dan pompa on
   rule5;
    if ((hangat =0) || (panas =1)){
      digitalWrite(heater,LOW);
    }
    if ((agakkeruh =0) || (keruh =1)){
      digitalWrite(pompa,HIGH);
    }
// jika suhu hangat dan kekeruhan keruh maka heater off dan pompa on
   rule6;
    if ((hangat =0) || (panas =1)){
      digitalWrite(heater,LOW);
    }
    if (keruh =1){
      digitalWrite(pompa,HIGH);
      
 //defuzifikasi
 out = ((rule1*dingin*jernih) + (rule2*dingin*agakkeruh) + (rule3*dingin*keruh) + (rule4*hangat*jernih) + (rule5*hangat*agakkeruh) + (rule6*hangat*keruh)) / 
 (dingin*jernih + dingin*agakkeruh + dingin*keruh+hangat*jernih + hangat*agakkeruh + hangat*keruh);
    } 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin();
  sensorSuhu.begin();
  pinMode(heater,OUTPUT);
  pinMode(laser,OUTPUT);
  pinMode(pompa,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Monitoring Kolam");
  
  suhu = temp_sensor; 
  kekeruhan = kekeruhan_sensor; 
  fuzzifikasi(); 
  Serial.print("dingin : ");
  Serial.print(dingin);
  Serial.print("t");
  Serial.print("hangat : ");
  Serial.print(hangat);
  Serial.print("t");
  Serial.print("panas : ");
  Serial.println(panas);
  Serial.println("jernih : ");
  Serial.println(jernih);
  Serial.println("t");
  Serial.println("agakkeruh : ");
  Serial.println(agakkeruh);
  Serial.println("t");
  Serial.println("keruh : ");
  Serial.println(keruh);
  Serial.println("Rule1 : ");
  Serial.println(rule1);
  Serial.println("Rule2 : ");
  Serial.println(rule2);
  Serial.println("Rule3 : ");
  Serial.println(rule3);
  Serial.println("Rule4 : ");
  Serial.println(rule4);
  Serial.println("Rule5 : ");
  Serial.println(rule5);
  Serial.println("Rule6 : ");
  Serial.println(rule6);
  Serial.print("Hasil DeFuzzy: ");
  Serial.println(out);
  delay(2000);
}
void loop() {
  temp_sensor = ambilSuhu();
  kekeruhan_sensor = ambilKekeruhan();
  fuzzifikasi(); 
  
}

float ambilSuhu(){
   sensorSuhu.requestTemperatures();
   float suhu = sensorSuhu.getTempCByIndex(0);
   return suhu;  
}

float ambilKekeruhan(){
   int kekeruhan= analogRead(ldr);
   Serial.println(kekeruhan);
   return kekeruhan;
}
