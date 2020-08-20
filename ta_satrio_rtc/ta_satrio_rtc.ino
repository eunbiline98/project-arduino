#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include<Servo.h>

#define FIREBASE_HOST "pakan-ikan-nodemcu.firebaseio.com"
#define FIREBASE_AUTH "wofAS9n7yoQAo0hBN1msots5o0PNwrdVqFFz1zpr"
#define WIFI_SSID "Robotic_Garage"
#define WIFI_PASSWORD "robotic77"
#define indi_wifi D4

//time
#include <NTPClient.h>
#include <WiFiUdp.h>
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000);
//rtc
#include <RTClib.h>
#include <Wire.h>
 
RTC_DS3231 rtc;
 
char t[32];
//inisialisasi Servo
Servo servo;
int servoPin = D0;
int servoVal = 0;

//inisialisasi Pelontar
#define pelontar D3

//ultrasonic
int trig= D5;           
int echo= D6;             
long durasi, jarak;    

 int kondisi = 0;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

void setup()
{
  Serial.begin(115200);
  Wire.begin(5, 4);   //Setting wire (5 untuk SDA dan 4 untuk SCL)
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  timeClient.begin();
  pinMode(indi_wifi, OUTPUT);
  //Servo
  servo.attach(servoPin);
  servo.write(0);
  //pelontar
  pinMode(pelontar,OUTPUT);
  //ultrasonic
  pinMode(trig, OUTPUT);  
  pinMode(echo, INPUT);
  //WiFi
 //WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    digitalWrite(indi_wifi,HIGH);
    delay(80);
    digitalWrite(indi_wifi,LOW);
    delay(80);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setFloat(firebaseData,"servo",0);
}

void loop()
{
  
 timeClient.update();
 DateTime now = rtc.now(); 

  Serial.print("Tanggal : ");
  Serial.print(now.day());        //Menampilkan Tanggal
  Serial.print("/");
  Serial.print(now.month());      //Menampilkan Bulan
  Serial.print("/");
  Serial.print(now.year());       //Menampilkan Tahun
  Serial.print(" ");
  
  Serial.print("Jam : ");
  Serial.print(now.hour());       //Menampilkan Jam
  Serial.print(":");
  Serial.print(now.minute());     //Menampilkan Menit
  Serial.print(":");
  Serial.print(now.second());     //Menampilkan Detik
  Serial.println();
 
  delay(1000);
  status_pakan(); // status box pakan ikan   
// setting jam pakan ikan via app
  Firebase.getString(firebaseData,"jam");
  String fb_jam = firebaseData.stringData();
  String jam = String(timeClient.getHours());
  String menit = String(timeClient.getMinutes());
  if(timeClient.getMinutes() < 10){
      menit = "0"+menit;
    }  
  String jamBuka = jam+":"+menit;
  Serial.println(jamBuka);
  if (jamBuka == fb_jam){
     Firebase.setFloat(firebaseData,"servo",1);
     digitalWrite(pelontar,HIGH);
        for(int i =90; i>=0; i--){
        servo.write(i);
        delay(10);
        Serial.println("Buka jam Aplikasi");
        break;
     }
     kondisi = 1;
  }

  if(jamBuka != fb_jam && kondisi == 1){
    Firebase.setFloat(firebaseData,"servo",0);
    digitalWrite(pelontar,LOW);
      for(int i=0; i<=90;i++){
          servo.write(i);
          delay(10);
          Serial.println("tutup");
          break;
        }
        kondisi = 0;
    }
//jadwal pakan ikan pagi dan sore 
//pagi
    if ((now.hour() == 11) && (now.minute() == 30)&& (now.second() == 0)){
       Firebase.setFloat(firebaseData,"servo",1);
       digitalWrite(pelontar,HIGH);
        for(int i =90; i>=0; i--){
          servo.write(i);
          delay(10);
          break;  
      }
  }
  if ((now.hour() == 11) && (now.minute() == 30)&& (now.second() == 45)){
      Firebase.setFloat(firebaseData,"servo",0);
      digitalWrite(pelontar,LOW);
       for(int i=0; i<=90; i++){
          servo.write(i);
          delay(10);
          break;
      }
  }
  //sore
  if ((now.hour() == 14) && (now.minute() == 6)&& (now.second() == 0)){
      Firebase.setFloat(firebaseData,"servo",1);
       digitalWrite(pelontar,HIGH);
      for(int i =90; i>=0; i--){
        servo.write(i);
        delay(10);
        break;
     }
  }
    if ((now.hour() == 14) && (now.minute() == 6)&& (now.second() == 45)){
      Firebase.setFloat(firebaseData,"servo",0);
      digitalWrite(pelontar,LOW);
       for(int i=0; i<=90; i++){
          servo.write(i);
          delay(10);
          break;
      }
   } 
  // setup android and firebase
  Firebase.getInt(firebaseData,"servo");
  servoVal =firebaseData.intData();
  if(servoVal == 0){
    digitalWrite(pelontar,LOW);
      for(int i=0; i<=90; i++){
        servo.write(i);
        delay(10);
        break;
      }  
    }
      if(servoVal == 1){
        digitalWrite(pelontar,HIGH);
      for(int i =90; i>=0; i--){
        servo.write(i);
        delay(10);
        break;
      }
    }
 }
 void status_pakan(){
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  digitalWrite(trig, HIGH);
  delayMicroseconds(8);
  digitalWrite(trig, LOW);
  delayMicroseconds(8);
  
  durasi= pulseIn(echo, HIGH); 
  jarak= (durasi/2) / 29.1;     
  Serial.println(jarak);  

    if (jarak <= 20){
      Firebase.setFloat(firebaseData,"status_pakan",0);
    }
     else{
      Firebase.setFloat(firebaseData,"status_pakan",1);
    }
 }
