#include <SoftwareSerial.h> //library default software ide arduino
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
SoftwareSerial SIM80L(9, 8); //RX, TX
 
// dapat dicek menggunakan i2c finder
 
void setup() {
  Serial.begin(9600); //Untuk ditampilkan di serial monitor
  lcd.begin(); //untuk ukuran lcd 16x2
  SIM80L.begin(9600);  
 
  lcd.setCursor(0, 0); 
  lcd.print("Atmduino Product");
  lcd.setCursor(0, 1); 
  lcd.print("Made In Malang");
 
  delay(5000);
 
  //perintah atau command untuk mengakses sim80l v.2
  SIM80L.println("AT+CMGF=1");
  Serial.println("Mulai mengakses SIM800L");
  lcd.setCursor(0, 0);
  lcd.print("Started SIM80L");
  delay(100);
 
  Serial.println("SIM80L Siap!");
  lcd.setCursor(0, 1);
  lcd.print("SIM80L Siap!");
  SIM80L.println("AT+CNMI=2,2,0,0,0"); //command dapat dicek di libraries
  SIM80L.println("AT+CMGF=1");   
  delay(100); //waktu tunda 1 detik
  lcd.clear();
 
  SIM80L.println("AT+CMGS=\"087811307225\"\r"); //No.hp tujuan
  lcd.setCursor(0, 0);
  lcd.print("no.:087811307225");
  delay(100);
  SIM80L.println("-> Test sim800L v.1");
  delay(10);
  lcd.setCursor(0, 1);
  lcd.print("Test sim80L v.1");
  SIM80L.println((char)26); // CTRL+Z symbol untuk mengakhiri isi teks SMS
  delay(100);
}
 
void loop() {
 
}
 
