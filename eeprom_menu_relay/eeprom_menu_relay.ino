#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#define SS_PIN 10   
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
String content= "";
byte letter;

#include <EEPROM.h>

int btn_up = 2;
int btn_down = 3;
int btn_ok = 4;
int menu = 1;
byte kondisi;
#define relay 5
void setup() {
  Serial.begin(9600);
  SPI.begin();             
  mfrc522.PCD_Init(); 
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Pump Control");
  lcd.setCursor(0, 1);
  lcd.print(" EEPROM Arduino");
  delay(2000);
  pinMode(btn_up, INPUT_PULLUP);
  pinMode(btn_down, INPUT_PULLUP);
  pinMode(btn_ok, INPUT_PULLUP);
  pinMode (relay, OUTPUT);
  data_eeprom();
  akses();
  lcd.setCursor(0,0);
  lcd.print("Tapp Here...!!!");
  lcd.setCursor(0,1);
  lcd.print("Access Control  ");
}

void loop(){ 
  akses();     
  data_eeprom();
}
//}
void main_menu() {
  data_eeprom();
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Pompa On");
      lcd.setCursor(0, 1);
      lcd.print(" Pompa Off");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Pompa On");
      lcd.setCursor(0, 1);
      lcd.print(">Pompa Off");
      break;
    case 3:
      lcd.clear();
      lcd.print(" Pompa Off");
      lcd.setCursor(0, 1);
      lcd.print(">Exit");
      break;
    case 4:
      menu = 3;
      break;
  }
}

void executeAction() {
  data_eeprom();
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      st();
      break;
  }
}

void action1() {
  lcd.clear();
  lcd.print("Pompa Hidup");
  //digitalWrite(relay,LOW);
  EEPROM.write(0,1);
  delay (2000);
}
void action2() {
  lcd.clear();
  lcd.print("Pompa Mati   ");
  //digitalWrite(relay,HIGH);
  EEPROM.write(0,2);
  delay (2000);
}
void(*resetFunc)(void)=0;
void st(){
  lcd.clear();;
  lcd.print("Exit Success");
  delay(2000);
  resetFunc();
}
void data_eeprom() {
   Serial.println(kondisi);
  kondisi=EEPROM.read(0);
  if(kondisi == 1){
    digitalWrite(relay,HIGH);
  }
 if(kondisi==2){
    digitalWrite(relay,LOW);
  }
}

void akses(){
   // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

   while(content.substring(1) == "39 9D 24 A3"){
  main_menu();
  delay(250);
  if (!digitalRead(btn_down)){
    menu++;
    main_menu();
    delay(100);
    while (!digitalRead(btn_down));
  }
  if (!digitalRead(btn_up)){
    menu--;
    main_menu();
    delay(100);
    while(!digitalRead(btn_up));
  }
  if (!digitalRead(btn_ok)){
    executeAction();
    main_menu();
    delay(100);
    while (!digitalRead(btn_ok));
  }
}
}


