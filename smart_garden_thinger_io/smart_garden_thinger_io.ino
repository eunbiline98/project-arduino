#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

#include "DHT.h" 
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int analogPin = A0;
int sensor_soil = 0;
float kelembaban_tanah;

#define relay_pompa D0
#define indi_wifi D1

#define USERNAME "user"
#define DEVICE_ID "smart_garden"
#define DEVICE_CREDENTIAL "smart123"

#define SSID "Robotic_Garage"
#define SSID_PASSWORD "robotic77"

//#define SSID "Tugas embedded"
//#define SSID_PASSWORD "smartgarden"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);
  pinMode(relay_pompa, OUTPUT);
  pinMode(indi_wifi,OUTPUT);

  digitalWrite(relay_pompa,LOW);

//connect to internet
Serial.print("Connecting to Wi-Fi");
WiFi.begin(SSID, SSID_PASSWORD);
while (WiFi.status() != WL_CONNECTED) {           
  digitalWrite(indi_wifi,HIGH);
  delay(80);
  digitalWrite(indi_wifi,LOW);
  delay(80);
  }
//ip address status
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
delay(2000);

  thing.add_wifi(SSID, SSID_PASSWORD);

  thing["Pompa"] << digitalPin(relay_pompa);

  thing["Dht_11"] >> [](pson& out){
    float humidity_1 = dht.readHumidity();
    float celcius_1 = dht.readTemperature();
    // Add the values and the corresponding code
    out["Kelebaban_Udara"] = humidity_1;
    out["Temperature_Celcius"] = celcius_1;
  };

    thing["Soil Moisture"] >> [](pson& out){
    sensor_soil = analogRead(analogPin);
    kelembaban_tanah = ( 100 - ( (sensor_soil/1023.00) * 100 ) );
    // Add the values and the corresponding code
    out["Kelebaban_Tanah"] =  kelembaban_tanah;
  };
}
void loop() {
  if(WiFi.status() == WL_CONNECTED){
     digitalWrite(indi_wifi,HIGH);
  }
  float humidity_1 = dht.readHumidity();
  float celcius_1 = dht.readTemperature();
  thing.handle();
  Serial.print("Suhu : ");
  Serial.println(celcius_1); 
  Serial.print("kelembaban : ");
  Serial.println(humidity_1); 

  sensor_soil = analogRead(analogPin);
  //Serial.print(sensor_soil_1);
  kelembaban_tanah = ( 100 - ( (sensor_soil/1023.00) * 100 ) );
  Serial.print("Sensor 1 Kelembaban tanah = ");
  Serial.print(kelembaban_tanah);
  Serial.print("%\n\n");

  delay(1000);
}
