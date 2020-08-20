// firebase
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define FIREBASE_HOST "pkm-kc-markas-stmik-asia.firebaseio.com"
#define FIREBASE_AUTH "GxpKugSr5MJULDb2mCw2hMNefsbW8o2MoN8elZcS"
#define WIFI_SSID "Pkm_asia" // sesuaikan
#define WIFI_PASSWORD "sistemkomputer" // sesuaikan
//Define FirebaseESP8266 data object
FirebaseData firebaseData;
// lcd lib
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// rfid lib
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4 
#define RST_PIN D3 
MFRC522 mfrc522(SS_PIN, RST_PIN);
int statuss = 0;
int count = 0;
//solenoid setup
#define solenoid D0
//sensor pintu
#define sensor D8
int sp;
//sensor getar
int sens_getar = A0;
int getar = 0;
// waktu milis
unsigned long interval=4000;
unsigned long awal=0;
bool kon_awal = false;
bool kon_status = false;

int st =0;
int wifiStatus,pintu;

void setup() {
// rfid start up 
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init();   
// solenoid setup
  pinMode(solenoid,OUTPUT);
  digitalWrite(solenoid,HIGH);
// sensor pintu setup
  pinMode(sensor,INPUT_PULLUP);
// sensor getar
  pinMode(sens_getar,INPUT);
// lcd start up 
  lcd.begin();
  lcd.setCursor(3,0);
  lcd.print("STMIK ASIA");
  lcd.setCursor(5,1);
  lcd.print("PKM-KC");
  delay(2500);
  
  lcd.setCursor(3,0);
  lcd.print("  MARKAS          ");
  lcd.setCursor(2,1);
  lcd.print("Smart Brankas   ");
  delay(3000);
  lcd.clear();
// indikator wifi    
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  
//connect to internet
  while (WiFi.status() != WL_CONNECTED) {           //wait till connected to WiFi
    delay(200);  
    lcd.setCursor(0,0); 
    lcd.print("Menunggu Koneksi"); 
    }
//ip address status
  lcd.setCursor(0,0);
  lcd.print("Ip Address       ");  
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);             //connect to Database
  Firebase.reconnectWiFi(true);

  Firebase.setInt(firebaseData,"selenoid",1);
  
  delay(1000);
  lcd.clear();
}

void loop() {
//firebase status  
//  if (Firebase.failed()) {
//      Serial.print("setting /number failed:");
//      Serial.println(Firebase.error());  
//      return;
//  }
unsigned long currentMillis = millis();  
Firebase.setInt(firebaseData,"sensor_getar",getar);
getar = analogRead(sens_getar);
Serial.println(getar);
if ((unsigned long)(currentMillis - awal) >= interval){
// menampilkan kondisi status brankas
kon_status != kon_awal;
wifiStatus = WiFi.status();
if(wifiStatus == 3){
    lcd.setCursor(0,0);
    lcd.print("Connected...       ");
}
  else{
      lcd.setCursor(0,0);
      lcd.print("Disconnected...       ");
}
// keadaan sensor getar 
lcd.setCursor(0,1);
lcd.print("Kondisi :               ");
             
if ( getar >=130) { 
    lcd.setCursor(10,1);
    lcd.print("Bahaya                  ");
    delay(1200);
  } 
  else {
    lcd.setCursor(10,1);
    lcd.print("Aman               ");        
    delay(1200);
  }
   awal = millis();
}
pintu=Firebase.getInt(firebaseData,"selenoid");
if(pintu==0){ 
    //Firebase.setInt(firebaseData,"sens_pintu",0);
    //Firebase.setInt(firebaseData,"kon_pintu",0);
    digitalWrite(solenoid,LOW);
    //lcd.setCursor(0,0);
    //lcd.print("Pintu Terbuka         ");
    //lcd.setCursor(0,1);
    //lcd.print("Via Smartphone        ");
    Serial.println("pintu buka firebase");
}
  if(pintu==1){
      //Firebase.setInt(firebaseData,"selenoid",1);
      //Firebase.setInt(firebaseData,"kon_pintu",1);
      digitalWrite(solenoid,HIGH);
      //lcd.setCursor(0,0);
      //lcd.print("Pintu Terkunci         ");
      //lcd.setCursor(0,1);
      //lcd.print("Via Smartphone         ");
      Serial.println("pintu tutup firebase");
}
Serial.println(sp);
Firebase.setInt(firebaseData,"sensor_pintu",sp);
sp = digitalRead(sensor); 
if(digitalRead (sp) == 1 && st == 0){ 
    //Firebase.setInt(firebaseData,"selenoid",0);
    //Firebase.setInt(firebaseData,"kon_pintu",0);
    digitalWrite(solenoid,LOW);
    st = 1;
    //delay(1000);
    Serial.println("sensor buka firebase");
  }
  if(digitalRead (sp) == 0 && st == 1){
      digitalWrite(solenoid,HIGH);
      Firebase.setInt(firebaseData,"selenoid",1);
      //Firebase.setInt(firebaseData,"kon_pintu",1);
      lcd.setCursor(0,0);
      lcd.print("Pintu Terkunci         ");
      lcd.setCursor(0,1);
      lcd.print("                       ");    
      st = 0;
      //delay(1000);
      Serial.println("sensor tutup firebase");
    }
     
if ((unsigned long)(currentMillis - awal) >= interval){
//menampilkan kondisi menu awal
kon_awal != kon_status;  
// menu awal
lcd.setCursor(0,0);
lcd.print("Silahkan Tapping");
lcd.setCursor(0,1);
lcd.print("  Kartu E-KTP       ");

  awal = millis();
}
// pembacaan rfid modul
if ( ! mfrc522.PICC_IsNewCardPresent()){
    return;
}
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
}
//Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Firebase.setString(firebaseData,"rfid",content);
  content.toUpperCase();
  Serial.println();
  
 // akses no id yang di izinkan masuk
  if (content.substring(1) == "04 2C 65 FA F6 52 80") // no tag kartu
  {
    Firebase.setInt(firebaseData,"akses",1);
    lcd.setCursor(0,0);
    lcd.print("  Akses sukses          ");
    lcd.setCursor(0,1);
    lcd.print("  Hi, Ariq          ");
    //statuss = 1;
    delay(2000);
    lcd.clear();
  
    while(Firebase.getInt(firebaseData,"sensor_pintu")==Firebase.getInt(firebaseData,"selenoid")){
    lcd.setCursor(5,0);
    lcd.print("Tunggu         ");
    lcd.setCursor(0,1);
    lcd.print("Konfirmasi Admin        ");
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kunci Terbuka         ");
    lcd.setCursor(0,1);
    lcd.print("                       ");  
    delay(1000);
    digitalWrite(solenoid,LOW);
}

  
  else if (content.substring(1) == "90 33 AA 36") // no tag kartu
  {
    Firebase.setInt(firebaseData,"akses",2);
    lcd.setCursor(0,0);
    lcd.print("  Akses sukses          ");
    lcd.setCursor(0,1);
    lcd.print("  Hi, Eunbi          ");
    //statuss = 2;
    delay(2000);
    lcd.clear();
    
    while(Firebase.getInt(firebaseData,"sensor_pintu")==Firebase.getInt(firebaseData,"selenoid")){
    lcd.setCursor(5,0);
    lcd.print("Tunggu         ");
    lcd.setCursor(0,1);
    lcd.print("Konfirmasi Admin        ");
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kunci Terbuka         ");
    lcd.setCursor(0,1);
    lcd.print("                       ");  
    delay(1000);
    digitalWrite(solenoid,LOW);
   
  }
   else if (content.substring(1) == "7B 9E 16 D3") // no tag kartu
  {
    Firebase.setInt(firebaseData,"akses",3);
    lcd.setCursor(0,0);
    lcd.print("  Akses sukses          ");
    lcd.setCursor(0,1);
    lcd.print("  Hi, Nadien          ");
    statuss = 3;
    delay(2000);
    lcd.clear();
    
    while(Firebase.getInt(firebaseData,"sensor_pintu")==Firebase.getInt(firebaseData,"selenoid")){
    lcd.setCursor(5,0);
    lcd.print("Tunggu         ");
    lcd.setCursor(0,1);
    lcd.print("Konfirmasi Admin        ");
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Kunci Terbuka         ");
    lcd.setCursor(0,1);
    lcd.print("                       ");  
    delay(1000);
    digitalWrite(solenoid,LOW);
  }
// akses di tolak
  else{
    lcd.setCursor(0,0);
    lcd.print("   Akses Gagal       ");
    lcd.setCursor(0,1);
    lcd.print("  tapping ulang       ");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Percobaan Ke:");
    count++;
    lcd.setCursor(14,0);
    lcd.print(count);
    delay(1000);
  }

  if (count == 3){
    for(int tunggu=30; tunggu>=0; tunggu--){
      lcd.setCursor(7,1);
    lcd.print(tunggu);
    delay(1000);
     lcd.setCursor(0,0);
    lcd.print("    Silahkan        ");
    lcd.setCursor(0,1);
    lcd.print("Tunggu    Detik");
    count = 0;
    }
  }
 }
