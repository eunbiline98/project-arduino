//bluetooth
#include <SoftwareSerial.h> 
SoftwareSerial module_bluetooth(3, 2); // pin RX | TX
char data = 0; 

// rfid lib
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10 
#define RST_PIN 9 
MFRC522 mfrc522(SS_PIN, RST_PIN);
int count = 0;
int tapCount = 0;

//UID Rfid Yang mau di daftarakan
String Id_masuk = "7B 95 4E 0A";

#define kontak  7
#define starter 8
#define buzzer  2
            
void setup(){
  
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init(); 
          
  pinMode(kontak, OUTPUT);  
  pinMode(starter, OUTPUT); 
  pinMode(buzzer,OUTPUT); 

  digitalWrite(kontak, HIGH);
  digitalWrite(starter, HIGH);
  Serial.println("Silahkan Tapping...");
}
void loop(){
  digitalWrite(buzzer, LOW);
  //rfid setup
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
   }
  Serial.println();
  Serial.print("UID tag :");
  String content= "";
  byte letter;

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println("");

  if (content.substring(1) == Id_masuk) //sesuaikan id yang didaftarkan
  {
    tapCount++;
    count = 0;
  }
    if (tapCount == 1) {
      for(int tunggu=3; tunggu>=0; tunggu--){
        Serial.println("Waiting...");   
        digitalWrite(kontak, LOW);
        digitalWrite(starter, HIGH);
        delay(1000);
      }
      Serial.println("Kontak menyala, silakan tapp lagi untuk menyalakan mesin");
    }
    if (tapCount == 2){
      Serial.println("Menyalakan mesin");
      digitalWrite(kontak, LOW);
      digitalWrite(starter, LOW);
      delay(800);
      digitalWrite(starter, HIGH);
    }
    if (tapCount == 3){
      Serial.println("Mematikan mesin dan kontak");
      digitalWrite(kontak, HIGH);
      digitalWrite(starter, HIGH);
      delay(200);
    }
    if (tapCount >= 3){
       for(int tunggu=10; tunggu>=0; tunggu--){
        Serial.print("Tunggu Beberapa Detik Untuk Akses Kembali :");   
        Serial.println(tunggu);
        delay(1000);  
        tapCount = 0;
       }
    }   
  else{
    count++;
    delay(1000); 
   }
  
  //counter tapp kartu >= 3 maka masuk ke program akses ditolak
  Serial.print("Percobaan Tapp Ke :");
  Serial.println(count);
  //counter ketika akses sukses
  Serial.print("Masuk Tapp Ke :");
  Serial.println(tapCount);

  // akses ditolak
  if (count == 4){
    Serial.println("Akses Ditolak");
    for(int tunggu=10; tunggu>=0; tunggu--){
     digitalWrite(buzzer, HIGH);   
     delay(1500);                      
     digitalWrite(buzzer, LOW); 
     Serial.print("Gagal, Tunggu Beberapa Detik :");   
     Serial.println(tunggu);
     delay(500);                      
    count = 0;
    }
  }
  if (count== 0){
    Serial.println("Silahkan Tapping Kembali...");
  }
}
