// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "smartgarden-45f50.firebaseio.com"
#define FIREBASE_AUTH "8dQk0C1tIBRvuDLoMuus3eLws1Al12DiOWolOxBn"
#define WIFI_SSID "smart garden"
#define WIFI_PASSWORD "garden123"
FirebaseData firebaseData;

#define pompa_air D1

//dht setup
#include "DHT.h"
#define DHTPIN D5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//sensor soil
#define DO_soil_moisture D6
int AO_soil_moisture = A0;
int batas_lembab = 75;

unsigned int getAnalog() {
  const int sampling = 100;
  unsigned int analog = 0;
  for (int i = 0; i < sampling; i++) {
    digitalWrite(DO_soil_moisture, HIGH);
    int nilaiSensor = analogRead(AO_soil_moisture);
    digitalWrite(DO_soil_moisture, LOW);
    analog += nilaiSensor;
  }
  analog = analog / sampling;
  return analog;
}

uint8_t getSoil() {
  // Batas atas
  const int val_max = 870;
  // Batas bawah
  const int val_min = 320;
  const int sampling = 100;

  unsigned long analog = 0;
  for (int i = 0; i < sampling; i++) {
    digitalWrite(DO_soil_moisture, HIGH);
    int nilaiSensor = analogRead(AO_soil_moisture);
    digitalWrite(DO_soil_moisture, LOW);
    analog += nilaiSensor;
  }
  analog = analog / sampling;
  if (analog > val_max) {
    analog = val_max;
  }
  else if (analog < val_min) {
    analog = val_min;
  }
  uint8_t persen = map(analog, val_max, val_min, 0, 100);
  return persen;
}

void setup() {
  Serial.begin(115200);
  Serial.println("DHT22 sensor!");
  pinMode(pompa_air, OUTPUT);
  dht.begin();

  //WiFi
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
  Dht22();
  soilMoisture();
  dataLogging();
  //soil moisture
  Serial.print("Kelembaban Tanah: ");
  Serial.println(getSoil());

  if (getSoil() >= batas_lembab) {
    Firebase.setInt(firebaseData, "sensor/st_tanah", 1);
    digitalWrite(pompa_air, LOW);
    Serial.println("Pompa OFF");
  }
  if (getSoil() <= batas_lembab) {
    Firebase.setInt(firebaseData, "sensor/st_tanah", 0);
    digitalWrite(pompa_air, HIGH);
    Serial.println("Pompa ON");
  }
}

void Dht22() {
  //DHT 22
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
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
  String temp = float(t) + String("°C");
  String humd = float(h) + String("%");
  Firebase.setString(firebaseData, "sensor/suhu", temp);
  Firebase.setString(firebaseData, "sensor/rh_udara", humd);
}
void soilMoisture() {
  // Soil moisture
  int st_tanah;
  st_tanah = firebaseData.intData();
  if (st_tanah == 1) {
    Firebase.setInt(firebaseData, "status_pompa", 0);
    Serial.print("status_tanah:");
    Serial.print("Kering");
    Serial.println("");
  }
  if (st_tanah == 0) {
    Firebase.setInt(firebaseData, "status_pompa", 1);
    Serial.print("status_tanah:");
    Serial.println("Lembab");
    Serial.println("");
  }
  String soil = getSoil() + String("%");
  Firebase.setString(firebaseData, "sensor/rh_tanah", soil);
}

void dataLogging() {
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  String temp = float(t) + String("°C");
  String humd = float(h) + String("%");
  // data logging to firebase
  Firebase.pushString(firebaseData, "data_log/DHT 22/suhu", temp);
  Firebase.pushString(firebaseData, "data_log/DHT 22/rh_udara", humd);

  String soil = getSoil() + String("%");
  Firebase.pushString(firebaseData, "data_log/soil moisture/rh_tanah", soil);

  delay(2000);
}
