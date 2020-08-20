// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "monitoring-dht11.firebaseio.com"
#define FIREBASE_AUTH "S7zLD1Y9zpWt4BTn0kuRZKLPXgkNaUupTXDjZOv8"
#define WIFI_SSID "Robotic_Garage"
#define WIFI_PASSWORD "robotic77"
#define indi_wifi D0
FirebaseData firebaseData;
//relay
#define relay_1 D6
#define relay_2 D7
//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D5
DHT dht(dht_dpin, DHTTYPE); 

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
  dht.begin();
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  pinMode(indi_wifi, OUTPUT);

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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.setInt(firebaseData,"Lampu_1",0);
  Firebase.reconnectWiFi(true);

}

void loop() {
if (WiFi.status() == WL_CONNECTED)
  {
    digitalWrite(indi_wifi,HIGH);
    delay(1000);
    digitalWrite(indi_wifi,LOW);
    delay(100);
  }
 float h = dht.readHumidity();
 float t = dht.readTemperature();  
 // set value
  Firebase.setFloat(firebaseData,"suhu",t);
  Firebase.setFloat(firebaseData,"kelembaban_udara",h);

  // lamp1 control
  String lamp1;
  Firebase.getString(firebaseData,"Lampu_1");
  lamp1=firebaseData.stringData();
  if(lamp1=="0"){
    digitalWrite(relay_1,LOW);;
  }
    if(lamp1=="1"){
      digitalWrite(relay_1,HIGH);
  }
  // lamp2 control
  String lamp2;
  Firebase.getString(firebaseData,"Lampu_2");
  lamp2=firebaseData.stringData();
  if(lamp2=="0"){
    digitalWrite(relay_2,LOW);
 }
    if(lamp2=="1"){
      digitalWrite(relay_2,HIGH);
 }
}
