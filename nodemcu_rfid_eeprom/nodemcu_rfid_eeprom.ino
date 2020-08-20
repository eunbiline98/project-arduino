/* ----------------------------------------------------------------------
   Pin layout should be as follows:
   Signal     Pin              Pin               Pin              Pin
            ESP8266          LCD I2C         MFRC522 board     Servo SG90
   ----------------------------------------------------------------------
   Reset      RST                                RST
   SPI SS     D4                                 SDA
   SPI MOSI   D5                                 MOSI
   SPI MISO   D7                                 MISO
   SPI SCK    D6                                 SCK
   SERVO      D3                                                 SIGNAL
   SDA        D2              SDA
   SCL        D1              SCL
   5V         VIN             VIN                                VIN
   3.3V       3V3                                3.3V
   GND        GND             GND                GND             GND
*/
                                                               
#include <Wire.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Servo.h>
#include <ESP8266WiFi.h>
#include <time.h>

#define UNLOCK_POS 10
#define LOCK_POS 100
#define BUTTON_PIN D3
#define SS_PIN D4
#define RST_PIN D8

Servo myservo;
LiquidCrystal_I2C lcd(0x27, 20, 4);
// NodeMCU Dev Kit => D1 = SCL, D2 = SDA

const char* WIFI_SSID = "SSID";
const char* WIFI_PWD = "passw0rd";

int timezone = 7;
char ntp_server1[20] = "3.th.pool.ntp.org";
char ntp_server2[20] = "1.asia.pool.ntp.org";
char ntp_server3[20] = "0.asia.pool.ntp.org";

int dst = 0;
bool displayTime = 0;

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long uidDec, uidDecTemp;
int ARRAYindexUIDcard;
int EEPROMstartAddr;
long adminID = 0001115558;
int LockSwitch;
unsigned long CardUIDeEPROMread[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  myservo.attach(D0);
  myservo.write(LOCK_POS);
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("................");
  SPI.begin();
  mfrc522.PCD_Init();
  EEPROM.begin(512);

  WiFi.begin(WIFI_SSID, WIFI_PWD);
  delay(1000);
  configTime(timezone * 3600, dst, ntp_server1, ntp_server2, ntp_server3);
  delay(1000);

  lcd.clear();
  DisplayWAiT_CARD();
  EEPROMreadUIDcard();
}

void loop() {

  time_t now = time(nullptr);
  struct tm* newtime = localtime(&now);

  if (displayTime == 0) {

    if (!time(nullptr)) {
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME     ");
    }
    else
    {
      lcd.setCursor(4, 0);
      if ((newtime->tm_hour) < 10)lcd.print("0");
      lcd.print(newtime->tm_hour);
      lcd.print(":");

      lcd.setCursor(7, 0);
      if ((newtime->tm_min) < 10)lcd.print("0");
      lcd.print(newtime->tm_min);
      lcd.print(":");

      lcd.setCursor(10, 0);
      if ((newtime->tm_sec) < 10)lcd.print("0");
      lcd.print(newtime->tm_sec);
    }
  }

  if ( digitalRead(BUTTON_PIN) == LOW ) {
    lcd.clear(), lcd.setCursor(0, 0), lcd.print(" BUTTON  UNLOCK");
    unlock();
    delay(2000);
    lock();
    DisplayWAiT_CARD();
  }

  //Look for new cards.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  //Select one of the cards.
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  //Read "UID".
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    uidDecTemp = mfrc522.uid.uidByte[i];
    uidDec = uidDec * 256 + uidDecTemp;
  }

  if (uidDec == adminID || LockSwitch > 0)EEPROMwriteUIDcard();

  if (LockSwitch == 0)
  {
    for (ARRAYindexUIDcard = 0; ARRAYindexUIDcard <= 9; ARRAYindexUIDcard++)
    {
      if (CardUIDeEPROMread[ARRAYindexUIDcard] > 0)
      {
        if (CardUIDeEPROMread[ARRAYindexUIDcard] == uidDec)
        {
          unlock();
          CommandsCARD();
          break;
        }
      }
    }
    if (ARRAYindexUIDcard == 10)lcd.setCursor(0, 0), lcd.print(" Card not Found"), lcd.setCursor(0, 1), lcd.print("                "), lcd.setCursor(0, 1), lcd.print("ID "); lcd.print(uidDec);
    delay(2000);
    lock();
    ARRAYindexUIDcard = 0;
    DisplayWAiT_CARD();
  }
}

// ******************************************

void EEPROMwriteUIDcard() {

  if (LockSwitch == 0)
  {
    displayTime = 1;
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 0);
    lcd.print("Start Record Card");
    lcd.display();
    delay(500);
  }

  if (LockSwitch > 0)
  {
    if (uidDec == adminID)
    {
      displayTime = 1;
      lcd.setCursor(0, 0);
      lcd.print("                 ");
      lcd.setCursor(0, 0);
      lcd.print("   SKIP RECORD   ");
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print("   Label : ");
      lcd.print(EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      delay(200);
    }
    else
    {
      EEPROM.write(EEPROMstartAddr, uidDec & 0xFF);
      EEPROM.write(EEPROMstartAddr + 1, (uidDec & 0xFF00) >> 8);
      EEPROM.write(EEPROMstartAddr + 2, (uidDec & 0xFF0000) >> 16);
      EEPROM.write(EEPROMstartAddr + 3, (uidDec & 0xFF000000) >> 24);
      EEPROM.commit();
      delay(10);
      displayTime = 1;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  RECORD  OK!  ");
      lcd.setCursor(0, 1);
      lcd.print("  MEMORY : ");
      lcd.print(EEPROMstartAddr / 5);
      EEPROMstartAddr += 5;
      delay(500);
    }
  }

  LockSwitch++;

  if (EEPROMstartAddr / 5 == 10)
  {
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("RECORD FINISH");
    EEPROMstartAddr = 0;
    uidDec = 0;
    ARRAYindexUIDcard = 0;
    EEPROMreadUIDcard();
    displayTime = 0;
  }
}

// -------------------------------------------

void EEPROMreadUIDcard()
{
  for (int i = 0; i <= 9; i++)
  {
    byte val = EEPROM.read(EEPROMstartAddr + 3);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 2);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr + 1);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;
    val = EEPROM.read(EEPROMstartAddr);
    CardUIDeEPROMread[ARRAYindexUIDcard] = (CardUIDeEPROMread[ARRAYindexUIDcard] << 8) | val;

    ARRAYindexUIDcard++;
    EEPROMstartAddr += 5;
  }

  ARRAYindexUIDcard = 0;
  EEPROMstartAddr = 0;
  uidDec = 0;
  LockSwitch = 0;
  displayTime = 0;
  DisplayWAiT_CARD();
}

// -------------------------------------------

void DisplayWAiT_CARD()
{
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Please Tag Card");
}

// -------------------------------------------

void CommandsCARD()
{
  lcd.clear(), lcd.setCursor(0, 0), lcd.print("Hi,  "), lcd.print(ARRAYindexUIDcard), lcd.print("        ");
  lcd.setCursor(0, 1), lcd.print("ID "), lcd.print(uidDec) ;

  if (ARRAYindexUIDcard == 0)
  {
    lcd.setCursor(5, 0), lcd.print("Alfa            ");
  }

  else if (ARRAYindexUIDcard == 1)
  {
    lcd.setCursor(5, 0), lcd.print("Bravo            ");
  }

  else if (ARRAYindexUIDcard == 2)
  {
    lcd.setCursor(5, 0), lcd.print("Charlie            ");
  }

  else if (ARRAYindexUIDcard == 3)
  {
    lcd.setCursor(5, 0), lcd.print("Delta            ");
  }

  else if (ARRAYindexUIDcard == 4)
  {
    lcd.setCursor(5, 0), lcd.print("Echo            ");
  }

  else if (ARRAYindexUIDcard == 5)
  {
    lcd.setCursor(5, 0), lcd.print("Foxtrot            ");
  }

  else if (ARRAYindexUIDcard == 6)
  {
    lcd.setCursor(5, 0), lcd.print("Golf            ");
  }

  else if (ARRAYindexUIDcard == 7)
  {
    lcd.setCursor(5, 0), lcd.print("Hotel            ");
  }

  else if (ARRAYindexUIDcard == 8)
  {
    lcd.setCursor(5, 0), lcd.print("India            ");
  }

  else if (ARRAYindexUIDcard == 9)
  {
    lcd.setCursor(5, 0), lcd.print("Juliett            ");
  }
  delay(1);
}

void lock() {
  myservo.write(LOCK_POS);
}

void unlock() {
  myservo.write(UNLOCK_POS);
}

