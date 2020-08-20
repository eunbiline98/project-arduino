// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "pkm-asia.firebaseio.com"
#define FIREBASE_AUTH "hdF4mLVdCbVvcvmetSfUYaLT140fTiOkUPLaxvvX"
#define WIFI_SSID "Cv brilliant solution reseach"
#define WIFI_PASSWORD "Akucantik123"
FirebaseData firebaseData;
// pompa 
#define pompa_air D0
// sensor DHT
#include "DHT.h"
#define dht_pin D5
#define DHTTYPE DHT11
DHT dht (dht_pin, DHTTYPE);
// sensor soil moisture
int sensor_lembab = A0; 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  pinMode(pompa_air,OUTPUT);
  
Serial.print("Connecting to Wi-Fi");
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {           
  Serial.println("Menunggu");
  delay(200);  
  }
//ip address status
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
delay(2000);

Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
Firebase.reconnectWiFi(true);

}

void loop() {
  // put your main code here, to run repeatedly:
String mode;
Firebase.getString(firebaseData,"mode");
mode=firebaseData.stringData();
if(mode=="1"){
  sensor_dht();
  sensor_soil();
      Serial.println("mode auto");
    } 
  if(mode=="0"){
    control_pompa();
    sensor_dht();
    sensor_soil();
    Serial.println("mode manual");
  } 
  delay(1000);  
}

void control_pompa(){
 // pompa control
String pompa;
Firebase.getString(firebaseData,"pompa");
pompa=firebaseData.stringData();
if(pompa=="1"){
  digitalWrite(pompa_air,LOW);
  Serial.println("pompa aktif");
  
}
  if(pompa=="0"){
    digitalWrite(pompa_air,HIGH);
    Serial.println("pompa non aktif");
}
}
void sensor_dht(){
  float kelembaban_udara = dht.readHumidity();
  float t = dht.readTemperature(); 
  // set value
  Firebase.setFloat(firebaseData,"suhu",t);
  Firebase.setFloat(firebaseData,"rh_udara",kelembaban_udara);
  Serial.println(t);
  Serial.println(kelembaban_udara);
}

void sensor_soil(){
  int lembab = analogRead(sensor_lembab);
  float kelembaban_tanah = ( 100 - ( (lembab/1023) * 100 ) );
  Serial.print("Sensor 1 Kelembaban tanah = ");
  Serial.print(kelembaban_tanah);
  Serial.print("%\n\n");
  Firebase.setFloat(firebaseData,"rh_tanah",kelembaban_tanah);
}
