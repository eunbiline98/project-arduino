// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "app-door-lock.firebaseio.com"
#define FIREBASE_AUTH "VKy0Di7bOvaFsz2bluUSjQAw7BjNce5Fq9demCEz"
#define WIFI_SSID "mirul"
#define WIFI_PASSWORD "opoaewes"
FirebaseData firebaseData;
#define solenoid D0
#define sensor D5
#define sensorbuka D4
#define indi_wifi D8
int st =0;
int st1 =0;
int sp,sb;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(solenoid,OUTPUT);
  pinMode(indi_wifi,OUTPUT);
  pinMode(sensor,INPUT_PULLUP);
  pinMode(sensorbuka,INPUT_PULLUP);

   // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(indi_wifi,HIGH);
    delay(80);
    digitalWrite(indi_wifi,LOW);
    delay(80);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(indi_wifi,HIGH);
    delay(1000);
    digitalWrite(indi_wifi,LOW);
    delay(100);
  }
 String pintu;
  Firebase.getString(firebaseData,"pintu");
  pintu=firebaseData.stringData();
  if(pintu=="0"){
    digitalWrite(solenoid,LOW);
    }
    if(pintu=="1"){
      digitalWrite(solenoid,HIGH);
    }

  sp = digitalRead(sensor);
  //Serial.println(sp);
  Firebase.setInt(firebaseData,"sensor",sp);
  if (digitalRead(sp) == 1 && st == 0){
    digitalWrite(solenoid,LOW);
    Firebase.setString(firebaseData,"pintu","0");
    st = 1;
  }
  if (digitalRead(sp) == 0 && st == 1){
     Firebase.setString(firebaseData,"pintu","1");
    st = 0;
  }

  sb= digitalRead(sensorbuka);
  Serial.print(sb);
  if (digitalRead(sb)== 0 && st1 == 1){
    digitalWrite(solenoid,LOW);
    //Firebase.setString(firebaseData,"pintu","0");
    st1=0;
  }
  
  if (digitalRead(sb)== 1 && st1 == 0){
    digitalWrite(solenoid,HIGH);
    Firebase.setString(firebaseData,"pintu","1");
    
    st1=1;
  }
}
