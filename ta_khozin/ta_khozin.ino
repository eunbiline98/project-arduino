//http://dan.drown.org/stm32duino/package_STM32duino_index.json
#include <TinyGPS++.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>

#define RXBT   D25 //PA10
#define TXBT   D26 //PA9
#define RXsim  D8 //PA3
#define TXsim  D9 //PA2
#define RXgps  D0 //PB11
#define TXgps  D1 //PB10

#define swOnOff PB15
#define swCoil  PA8
#define inOnOff PB14
#define led     PC13

SoftwareSerial sim800(D8, D9);
SoftwareSerial serialBT(D25, D26);
SoftwareSerial serialGPS(D0, D1);

TinyGPSPlus gps;
double latitude, longitude;
char gpschar[60];
String a, gpsString;
uint8 statusCoil, statusSW, statusAlarm;

String textMessage;

void setup() {
  Serial1.begin(9600);
  sim800.begin(19200);
  delay(20000);
  sim800.print("AT+CMGF=1\r");  //Set SMS Text Mode
  delay(100);
  sim800.print("AT+CNMI=2,2,0,0,0\r");  //procedure, how to receive messages from the network
  delay(100);
  serialGPS.begin(9600);
  serialBT.begin(9600);
  pinMode(inOnOff, INPUT_PULLUP);
  pinMode(swOnOff, OUTPUT);
  pinMode(swCoil, OUTPUT);
  digitalWrite(swOnOff, HIGH);
  digitalWrite(swCoil, HIGH);
  EEPROM.init();
}

void readSMS() {
  if (textMessage.indexOf("UNLOCKC") >= 0) {
    EEPROM.write(1, 1);
    textMessage = "";
  }
  if (textMessage.indexOf("LOCKC") >= 0) {
    EEPROM.write(1, 0);
    textMessage = "";
  }
  if (textMessage.indexOf("UNLOCKP") >= 0) {
    EEPROM.write(2, 1);
    textMessage = "";
  }
  if (textMessage.indexOf("LOCKP") >= 0) {
    EEPROM.write(2, 0);
    textMessage = "";
  }
  if (textMessage.indexOf("LOK") >= 0) {
    sendSMS();
  }
}

void readGPS()
{
  while (serialGPS.available()) {
    gps.encode(serialGPS.read());
  }
  if (gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    gpsString = "http://www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6) ;
  }
}

void sendSMS()
{
  sim800.print("AT+CMGF=1\r\n"); //Prosedur penulisan oleh module sim800l
  delay(1000); //waktu jeda 1 detik

  sim800.print("AT+CMGS=\"081248691552\"\r\n");
  delay(1000);
  readGPS();
  //gpsString="http://www.google.com/maps/place/-7.938066,112.626326";
  int sLength = gpsString.length();
  gpsString.toCharArray(gpschar, sLength + 1);
  sim800.print(gpschar);
  delay(1000);

  sim800.print((char)26);
  delay(1000);

}

void smsAlarm()
{
  String warning;
  sim800.print("AT+CMGF=1\r\n"); //Prosedur penulisan oleh module sim800l
  delay(1000); //waktu jeda 1 detik

  sim800.print("AT+CMGS=\"081248691552\"\r\n");
  delay(1000);

  warning = "Awasss Motor Dihidupkan tanpa ijin....";
  int sLength = warning.length();
  warning.toCharArray(gpschar, sLength + 1);
  sim800.print(gpschar);
  delay(1000);

  sim800.print((char)26);
  delay(1000);
}

void Statuslock() {
  statusCoil = EEPROM.read(1);
  statusSW = EEPROM.read(2);
  if (statusCoil == 1)
  {
    digitalWrite(swCoil, LOW);
    statusAlarm = 0;
    Serial1.println("coil off");
  } else
  {
    digitalWrite(swCoil, HIGH);
    if (digitalRead(inOnOff) == 0 && statusAlarm == 0) //// sistem deteksi alarm SMS
    {
      smsAlarm();
      statusAlarm = 1;
      Serial1.println("coil on");
    }
  }
  if (statusSW == 1)
  {
    digitalWrite(swOnOff, HIGH);
    Serial1.println("sw off");
  } else
  {
    digitalWrite(swOnOff, LOW);
    Serial1.println("sw on");
  }
}

void readBT()
{
  a = serialBT.readString();
  if (a == "a")
  {
    EEPROM.write(1, 1);
  }
  if (a == "b")
  {
    EEPROM.write(1, 0);
  }
  if (a == "c")
  {
    EEPROM.write(2, 1);
  }
  if (a == "d")
  {
    EEPROM.write(2, 0);
  }
}

void loop() {
  Statuslock();
  readGPS();
  if (serialBT.available())
  {
    readBT();
  }
  if (sim800.available() > 0) {
    textMessage = sim800.readString();
    readSMS();
    delay(10);
  }
}
