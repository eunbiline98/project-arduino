/*
   visit www.anakkendali.com
   2018

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
WiFiClient client;

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN D3
#define RST_PIN D4

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

String request_string;
const char* host = "192.168.1.15"; // ip computer yang di install xampp (Windows + R ---> CMD ---->ketik ipconfig

//parsing data
int index1, index2;
String dat1, dat2;

HTTPClient http;

void setup() {
  pinMode(D8, OUTPUT);
  digitalWrite(D8, 1);
  lcd.begin();
  // put your setup code here, to run once:
  lcd.setCursor(0, 0);
  lcd.print("Conect to Wifi..");
  WiFi.disconnect();
  WiFi.begin("KOST_RAMA", "bayardulu500");
  while ((!(WiFi.status() == WL_CONNECTED))) {
    delay(300);
  }
  lcd.clear();
  lcd.print("Wifi Connected");
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  lcd.setCursor(0, 1);
  lcd.print("Siap Print..");
  Serial.println("I am waiting for card...");
}
String strID, val, data;

unsigned long timenow = 0;
int saldo;
int harga = 500;
int sendHarga = 0;
boolean menu = 0;

void baca_serial() {
  while (client.available() > 0) {
    delay(10);
    char c = client.read();
    data += c;
  }
  if (data.length() > 0) {
    Serial.println(data);
    index1 = data.indexOf('%');
    index2 = data.indexOf('%', index1 + 1);

    dat1 = data.substring(index1 + 1, index2);
    saldo = dat1.toInt();
    Serial.print("dat1: ");
    Serial.println(dat1);
    data = "";
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
      piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
      piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  //id kartu dan yang akan dikirim ke database
  strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
      (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
      String(rfid.uid.uidByte[i], HEX) +
      (i != 3 ? ":" : "");
  }

  strID.toUpperCase();
  Serial.println(strID);
  baca_database(strID);
  baca_serial();
  saldo -= harga;
  update_saldo(strID, saldo); 
//  //  Serial.println(strID);
  delay(1000);
}

//  val = "500"; // nilai kartu yang akan dikirim
//  update_saldo();
//  baca_database();

void update_saldo(String id, int nilai)
{
  if (!client.connect(host, 80)) {
    Serial.println("Gagal Konek");
    return;
  }

  request_string = "/esp8266/rfidupdate.php?idcard=";
  request_string += id;
  request_string += "&val=";
  request_string += nilai;

  Serial.print("requesting URL: ");
  Serial.println(request_string);
  client.print(String("GET ") + request_string + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}

void baca_database(String id)
{
  if (!client.connect(host, 80)) {
    Serial.println("Gagal Konek");
    return;
  }

  request_string = "/esp8266/rfidread.php?idcard=";
  request_string += id;

  Serial.print("requesting URL: ");
  Serial.print(request_string);
  client.print(String("GET ") + request_string + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}

void reg_kartu(String id)
{
  if (!client.connect(host, 80)) {
    Serial.println("Gagal Konek");
    return;
  }

  request_string = "/esp8266/rfidadd.php?idcard=";
  request_string += id;
  request_string += "&val=";
  request_string += 0;

  Serial.print("requesting URL: ");
  Serial.print(request_string);
  client.print(String("GET ") + request_string + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
}
