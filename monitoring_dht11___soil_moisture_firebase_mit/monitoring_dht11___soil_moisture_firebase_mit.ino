// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
// Set these to run example.
#define FIREBASE_HOST "monitoring-dht11.firebaseio.com"
#define FIREBASE_AUTH "S7zLD1Y9zpWt4BTn0kuRZKLPXgkNaUupTXDjZOv8"
#define WIFI_SSID "Robotic_Garage"
#define WIFI_PASSWORD "robotic77"
FirebaseData firebaseData;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
//relay
#define relay_1 D5
#define relay_2 D6
//dht11
#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin D4
DHT dht(dht_dpin, DHTTYPE); 
//sensor voltage
int sens_dc = A0;
int nilai_sens;
//bar loading
int progressBari = 0;  
int i = 0;

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

void setup() {
  Serial.begin(9600);
  lcd.begin();
  pinMode(relay_1,OUTPUT);
  pinMode(relay_2,OUTPUT);
  Serial.println("DHT11 test!");
  dht.begin();
 lcd.createChar(0, progressBar);
 progressBari= 16;
 
 for (i=0; i<progressBari; i++)
  {
    lcd.setCursor(i, 1);  
    lcd.write(byte(0)); 
    lcd.setCursor(0,0);
    lcd.print("Loading");
    delay(250);
  }
  lcd.clear();

lcd.setCursor(0,0);
lcd.print("AtmDuino Product");
lcd.setCursor(0,1);
lcd.print("Smart Switch");
delay(1500);
lcd.clear();

lcd.setCursor(1,0);
lcd.print("Made In Malang");
lcd.setCursor(2,1);
lcd.print("Local Pride");
delay(1500);
lcd.clear();

lcd.setCursor(0,0);
lcd.print("Contact Us");
lcd.setCursor(0,1);
lcd.print("08973234445");
delay(1500);
lcd.clear(); //menghapus data sebelumnya 

//connect to internet
  while (WiFi.status() != WL_CONNECTED) {       
    delay(200);  
    lcd.setCursor(0,0); 
    lcd.print("Menunggu Koneksi"); 
    }
//ip address status
lcd.setCursor(0,0);
lcd.print("Ip Address       ");  
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());  
  delay(2000);
  lcd.clear();
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
void loop() {
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
