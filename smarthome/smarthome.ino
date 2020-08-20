// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "monitoring-dht11.firebaseio.com"
#define FIREBASE_AUTH "S7zLD1Y9zpWt4BTn0kuRZKLPXgkNaUupTXDjZOv8"
#define WIFI_SSID "Robotic_Garage"
#define WIFI_PASSWORD "robotic77"
FirebaseData firebaseData, firebaseData1;

//relay
#define Relay1 D6
#define Relay2 D7
#define Relay3 D8

#define buzz D0

//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D5
DHT dht(dht_dpin, DHTTYPE);
//lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//bar loading
int progressBari = 0;
int i = 0;
//time
#include <NTPClient.h>
#include <WiFiUdp.h>
char daysOfTheWeek[7][12] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000);
// Kararter progress bar untuk LCD
byte progressBar[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};
// bar sinyal
byte sinyal[] = {
  0b00001,
  0b00001,
  0b00001,
  0b00101,
  0b00101,
  0b00101,
  0b10101,
  0b10101
};
byte antena[] = {
  0b11111,
  0b01110,
  0b01110,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  timeClient.begin();
  lcd.begin();
  dht.begin();
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(buzz, OUTPUT);
  lcd.createChar(0, progressBar);
  progressBari = 16;

  for (i = 0; i < progressBari; i++)
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
    lcd.setCursor(0, 0);
    lcd.print("Loading");
    delay(250);
  }
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("AtmDuino Product");
  lcd.setCursor(0, 1);
  lcd.print("Smart Home   ");
  delay(1500);
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print("Made In Malang");
  lcd.setCursor(2, 1);
  lcd.print("Local Pride");
  delay(1500);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Contact Us");
  lcd.setCursor(0, 1);
  lcd.print("08973234445");
  delay(1500);
  lcd.clear(); //menghapus data sebelumnya

  //connect to internet
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    lcd.setCursor(0, 0);
    lcd.print("Menunggu Koneksi");
  }
  //ip address status
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print("Ip Address       ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.setInt(firebaseData,"Lampu_1",0);
  Firebase.reconnectWiFi(true);

}
void loop() {
  // wifi status lcd show
  lcd.createChar(2, antena);//Sinyal
  lcd.setCursor(0, 0);
  lcd.write(2);
  int wifiStatus = WiFi.status();
  if (wifiStatus == 3) {

    lcd.createChar(1, sinyal);//Sinyal
    lcd.setCursor(1, 0);
    lcd.write(1);
  }
  else {
    lcd.setCursor(1, 0);
    lcd.print("X");
  }
  waktu();
  control_relay();
  temp_monitoring();
}
void waktu() {
  timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());
  String menit = String(timeClient.getMinutes());
  if (timeClient.getMinutes() < 10) {
    menit = "0" + menit;
  }
  // lcd show
  lcd.setCursor(3, 0);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(9, 0);
  lcd.print(",");
  lcd.setCursor(10, 0);
  lcd.print(timeClient.getHours());
  lcd.setCursor(12, 0);
  lcd.print(":");
  lcd.setCursor(13, 0);
  lcd.print(menit);


}
void control_relay() {
  // lamp1 control
  String lamp1;
  Firebase.getString(firebaseData, "lampu/1");
  lamp1 = firebaseData.stringData();
  if (lamp1 == "1") {
    digitalWrite(Relay1, LOW);
    Firebase.setString(firebaseData, "st1", "1");
  }
  if (lamp1 == "0") {
    digitalWrite(Relay1, HIGH);
    Firebase.setString(firebaseData, "st1", "0");
  }

  // lamp2 control
  String lamp2;
  Firebase.getString(firebaseData, "lampu/2");
  lamp2 = firebaseData.stringData();
  if (lamp2 == "1") {
    digitalWrite(Relay2, LOW);
    Firebase.setString(firebaseData, "st2", "1");
  }
  if (lamp2 == "0") {
    digitalWrite(Relay2, HIGH);
    Firebase.setString(firebaseData, "st2", "0");
  }

  // lamp2 control
  String lamp3;
  Firebase.getString(firebaseData, "lampu/3");
  lamp3 = firebaseData.stringData();
  if (lamp3 == "1") {
    digitalWrite(Relay3, LOW);
    Firebase.setString(firebaseData, "st3", "1");
  }
  if (lamp3 == "0") {
    digitalWrite(Relay3, HIGH);
    Firebase.setString(firebaseData, "st3", "0");
  }
  
  //buzzer
  Firebase.getString(firebaseData, "buzzer");
  String bz = firebaseData.stringData();
  int status_4 = 0;
  if (bz == "1") {
      tone(buzz, 2000);
      delay(1000);
      noTone(buzz);
      delay(200);
  }
  if (bz == "0") {
    noTone(buzz);
  }
}
void temp_monitoring() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.setCursor(0, 1);
  lcd.print("H= ");
  lcd.setCursor(2, 1);
  lcd.print(h);
  lcd.print("%  ");
  lcd.setCursor(8, 1);
  lcd.print("T= ");
  lcd.setCursor(10, 1);
  lcd.print(t);
  lcd.write(0b11011111);

  // set value
  Firebase.setFloat(firebaseData, "suhu", t);
  Firebase.setFloat(firebaseData, "kelembaban_udara", h);
}
