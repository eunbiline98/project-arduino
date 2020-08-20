#include <Servo.h>
Servo servo_katub; 
const int servo = 9;
const int sensor_kaki = 12;
const int indikator = 13;

#include <DS3231.h>

DS3231  rtc(SDA, SCL);
Time t;
void setup() {
  servo_katub.attach(servo);
  pinMode(sensor_kaki, INPUT);
  digitalWrite(sensor_kaki, HIGH);
  pinMode(indikator, OUTPUT);
  digitalWrite(indikator, LOW);
  servo_katub.write(180);
  delay(1000);
  servo_katub.detach();
}

void loop() {
  int buttonVal = digitalRead(sensor_kaki);
  if(buttonVal == LOW) {
    servo_katub.attach(servo);
    servo_katub.write(30);
    delay(575);
    servo_katub.write(180);
    delay(1500);
    servo_katub.detach();
    delay(5000);
  }
  t = rtc.getTime();
  Serial.print(t.hour);
  Serial.print(" hour(s), ");
  Serial.print(t.min);
  Serial.print(" minute(s)");
  Serial.println(" ");
  Serial.print(t.sec);
  Serial.print(" second(s)");
  Serial.println(" ");
  delay (1000);
  // jadwal pakan
  jam_makan_pagi();
  jam_makan_sore();
  //delay(13);
}
void jam_makan_pagi(){
  if ((t.hour == 8) && (t.min == 0) && (t.sec == 0)) {    
    for(int i =90; i>=0; i--){
    servo_katub.write(i);
    delay(575);
    servo_katub.detach();
    delay(5000);                  
  }
 } 
  if ((t.hour == 8) && (t.min == 0) && (t.sec == 2)) {    
   for(int i =90; i>=0; i--){
    servo_katub.write(i);
    delay(575);
    servo_katub.detach();
    delay(5000);                  
  }  
 }
}
void jam_makan_sore(){
   if ((t.hour == 16) && (t.min == 0) && (t.sec == 0)) {    
    for(int i =90; i>=0; i--){
    servo_katub.write(i);
    delay(575);
    servo_katub.detach();
    delay(5000);                  
  }
 } 
  if ((t.hour == 16) && (t.min == 0) && (t.sec == 2)) {    
   for(int i =90; i>=0; i--){
    servo_katub.write(i);
    delay(575);
    servo_katub.detach();
    delay(5000);                  
  } 
 }
}
