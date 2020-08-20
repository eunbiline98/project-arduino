// firebase
#include <WiFi.h>
#include <FirebaseESP32.h>
// Set these to run example.
#define FIREBASE_HOST "smartgarden-45f50.firebaseio.com"
#define FIREBASE_AUTH "8dQk0C1tIBRvuDLoMuus3eLws1Al12DiOWolOxBn"
#define WIFI_SSID "smart garden"
#define WIFI_PASSWORD "garden123"

FirebaseData firebaseData;
FirebaseJson json;
void printResult(FirebaseData &data);

#define pompa_air 2

#include "DHT.h"
//here we use pin IO14 of ESP32 to read data
#define DHTPIN 14
//our sensor is DHT22 type
#define DHTTYPE DHT22
//sensor soil
int sensorPin = 15; 
int sensorValue; 
//create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  pinMode(pompa_air,OUTPUT);
  dht.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
//  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  //soil moisture
  sensorValue = digitalRead(sensorPin); 
  Serial.print("Sensor Value : ");
  Serial.println(sensorValue);
  Firebase.setInt(firebaseData,"sensor/rh_tanah",sensorValue);
  Serial.println("mode manual");
  int Mode;
  Firebase.getInt(firebaseData,"mode");
  Mode=firebaseData.intData();
  if(Mode == 1){
    Dht22();
    soilMoisture();
//    Serial.println("mode auto");
  } 
    if(Mode == 0){
    control_pompa();
    Dht22();
   }   
}
void control_pompa(){
  // pompa control
  int pompa;
  Firebase.getInt(firebaseData,"pompa");
  pompa=firebaseData.intData();
  if(pompa == 0){
    digitalWrite(pompa_air,LOW);
    Firebase.setInt(firebaseData,"status_pompa",1);
    Serial.println("pompa non aktif");
    }
    if(pompa == 1){
      digitalWrite(pompa_air,HIGH);
      Firebase.setInt(firebaseData,"status_pompa",0);
      Serial.println("pompa aktif");
    }
}
void Dht22(){
   //DHT 22 
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  String temp = float(t) + String("°C");   
  String humd = float(h) + String("%"); 
  Firebase.setString(firebaseData,"sensor/suhu",temp);
  Firebase.setString(firebaseData,"sensor/rh_udara",humd);
}
void soilMoisture(){
 // Soil moisture
 int rh_tanah;
 Firebase.setInt(firebaseData,"sensor/rh_tanah",sensorValue);
 rh_tanah=firebaseData.intData();
 if(rh_tanah == 1){
  digitalWrite(pompa_air,HIGH);
  Firebase.setInt(firebaseData,"status_pompa",0);
  Serial.println("Kering");
 }
 if(rh_tanah == 0){
    digitalWrite(pompa_air,LOW);
    Firebase.setInt(firebaseData,"status_pompa",1);
    Serial.println("Lembab");
  }
}
