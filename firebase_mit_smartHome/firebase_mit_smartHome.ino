// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "monitoring-dht11.firebaseio.com"
#define FIREBASE_AUTH "S7zLD1Y9zpWt4BTn0kuRZKLPXgkNaUupTXDjZOv8"
#define WIFI_SSID "Robotic_Garage"
#define WIFI_PASSWORD "robotic77"
FirebaseData firebaseData;
#define relay_1 D0
#define relay_2 D1
//servo
#include<Servo.h>
Servo servo;
//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE); 

int soilMoisture = A0;
int nilai_sens;

void setup() {
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  servo.attach(D2);
  servo.write(0);
  Serial.begin(9600);
  Serial.println("DHT11 test!");
  dht.begin();
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.setInt(firebaseData,"fan_on",0);
  Firebase.reconnectWiFi(true);
}
void loop() {
  nilai_sens = analogRead(soilMoisture);
  //Baca sensor suhu dan kelembaban
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  Serial.print(nilai_sens);
  Serial.println(" %\t ");
  
  // set value
  Firebase.setFloat(firebaseData,"suhu",t);
  Firebase.setFloat(firebaseData,"kelembaban_udara",h);
  Firebase.setFloat(firebaseData,"kelembaban_tanah",nilai_sens);

  // servo control
  String servo_data;
  Firebase.getString(firebaseData,"servo_open");
  servo_data=firebaseData.stringData();
  if(servo_data=="1"){
    for(int pos = 0; pos < 180; pos += 1){
      servo.write(pos);
      delay(15);
      break;
      }  
    }
    if(servo_data=="0"){
      for(int pos = 180; pos>=1; pos -= 1){
      servo.write(pos);
      delay(15);
      break;
      }  
    }
  
  // kipas control
  String kipas;
  Firebase.getString(firebaseData,"fan_on");
  kipas=firebaseData.stringData();
  if(kipas=="1"){
    digitalWrite(relay_1,LOW);
    }
    if(kipas=="0"){
      digitalWrite(relay_1,HIGH);
    }
  // pompa control
  String pompa;
  Firebase.getString(firebaseData,"pump_on");
  pompa=firebaseData.stringData();
  if(pompa=="1"){
    digitalWrite(relay_2,LOW);
    }
    if(pompa=="0"){
      digitalWrite(relay_2,HIGH);
    }
  }
